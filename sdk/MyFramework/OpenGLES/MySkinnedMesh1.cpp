
#include "MySkinnedMesh1.h"
#include "FrmMesh.h"

#pragma region Helpers

//===============================================================================================================
//
// Helpers
//
//===============================================================================================================

INT32 GetPropertyIndexFromName(Adreno::Mesh* pMesh, char* propertyName);

// Desc: Get final transform for a bone
// Transform of a bone is relative to its parent. When rendering, we'll need world transform of bone.
// This function compute world transform of a bone.
static void GetBoneTransform(
	Adreno::Model* pModel, 
	Adreno::Animation* pAnim, 
	INT32 boneIndex, 
	INT32 frameIndex, 
	Adreno::Transform& transform)
{
	Adreno::Joint* pJoint = pModel->Joints + boneIndex;
	Adreno::AnimationTrack* pTrack = pAnim->Tracks + boneIndex;

	if (frameIndex >= 0 && frameIndex < pTrack->NumKeyframes)
	{
		transform = *(pTrack->Keyframes + frameIndex);
	}
	else
	{
		transform = pJoint->Transform;
	}

	if (pJoint->ParentIndex != -1)
	{
		Adreno::Transform parentTransform;
		GetBoneTransform(pModel, pAnim, pJoint->ParentIndex, frameIndex, parentTransform);

		FRMMATRIX4X4 parentRotation = FrmMatrixRotate(parentTransform.Rotation);
		transform.Position = FrmVector3TransformCoord(transform.Position, parentRotation);
		transform.Rotation = FrmQuaternionMultiply(transform.Rotation, parentTransform.Rotation);
		transform.Position += parentTransform.Position;
	}
}

// Desc: Pre-process all bone transforms and flatten the bones hierarchy
// An animation is considered as a matrix of transforms. This transforms is relative to its parent.
// To render, we must have world transform. If we compute world transform when render, it will ineffective.
// Therefore, we pre-compute world transform for animation matrix.
static BOOL SetupBoneTransform(Adreno::Model* pModel, Adreno::Animation* pAnim)
{
	// If there is a mismatch, return false
	if (pModel->NumJoints != pAnim->NumTracks)
	{
		return FALSE;
	}

	// Process each of the bones
	for (INT32 boneIndex = 0; boneIndex < pModel->NumJoints; ++boneIndex)
	{
		Adreno::AnimationTrack* pTrack = pAnim->Tracks + boneIndex;

		// If there are frames, process them, in addition to the default transform
		if (pTrack->NumKeyframes > 0)
		{
			Adreno::Transform transform;

			// For each frame, calculate and save the final transform for the bone
			for (INT32 frameIndex = 0; frameIndex < pAnim->NumFrames; ++frameIndex)
			{
				GetBoneTransform(pModel, pAnim, boneIndex, frameIndex, transform);
				pTrack->Keyframes[frameIndex] = transform;
			}

			// Do the same for the default transform
			GetBoneTransform(pModel, pAnim, boneIndex, -1, transform);
			pModel->Joints[boneIndex].Transform = transform;
		}
		else
		{
			// Calculate and save the final default transform
			Adreno::Transform transform;
			GetBoneTransform(pModel, pAnim, boneIndex, -1, transform);
			pModel->Joints[boneIndex].Transform = transform;
		}

		// Flatten the bone hierarchy
		pModel->Joints[boneIndex].ParentIndex = -1;
	}

	return TRUE;
}

// Get rid of unused bones through remapping
static BOOL RemapBoneIndices(Adreno::Model* pModel, UINT32* boneRemap, UINT32& boneRemapCount)
{
	// Return false if number of total bones is greater than supported
	if (pModel->NumJoints > SkinnedMesh1::TOTAL_BONES)
	{
		return FALSE;
	}

	// Temporary buffer
	UINT8 processedBone[SkinnedMesh1::TOTAL_BONES];
	memset(processedBone, 0xFF, sizeof(processedBone));

	// The number of bones remapped
	boneRemapCount = 0;

	// Process each mesh
	for (INT32 meshIndex = 0; meshIndex < pModel->NumMeshes; ++meshIndex)
	{
		Adreno::Mesh* pMesh = pModel->Meshes + meshIndex;

		// Find the index for skinindex and skinweight properties
		INT32 skinIndexProperty = GetPropertyIndexFromName(pMesh, "skinindex");
		INT32 skinWeightProperty = GetPropertyIndexFromName(pMesh, "skinweight");

		if (skinIndexProperty < 0 || skinWeightProperty < 0)
		{
			continue;
		}

		// The number of bones that can influence a vertex
		UINT32 numberOfBones = pMesh->Vertices.Format.Properties[skinIndexProperty].NumValues();

		// Look at each vertex of the mesh
		for (UINT32 vertexIndex = 0, vertexOffset = 0; vertexIndex < pMesh->Vertices.NumVerts; ++vertexIndex, vertexOffset += pMesh->Vertices.Format.Stride)
		{
			UINT32* pBoneIndex = (UINT32*)(pMesh->Vertices.Buffer + vertexOffset + pMesh->Vertices.Format.Properties[skinIndexProperty].Offset);
			FLOAT32* pBoneWeight = (FLOAT32*)(pMesh->Vertices.Buffer + vertexOffset + pMesh->Vertices.Format.Properties[skinWeightProperty].Offset);

			// Add each bone that transform vertices to the table
			for (UINT32 boneCount = 0; boneCount < numberOfBones; ++boneCount)
			{
				UINT32 boneIndex = *(pBoneIndex + boneCount);
				FLOAT32 boneWeight = *(pBoneWeight + boneCount);

				// Ignore bones that have no influence (weight is zero)
				if (boneWeight != 0.0f)
				{
					if (processedBone[boneIndex] == 0xFF)
					{
						if (boneRemapCount >= SkinnedMesh1::MAX_BONES)
						{
							return FALSE;
						}

						processedBone[boneIndex] = boneRemapCount;
						boneRemap[boneRemapCount++] = boneIndex;
					}

					*(pBoneIndex + boneCount) = processedBone[boneIndex];
				}
			}
		}
	}

	return TRUE;
}

#pragma endregion

//===============================================================================================================
//
// SkinnedMesh1 class
//
//===============================================================================================================

SkinnedMesh1::SkinnedMesh1()
	: m_anim(nullptr),
	m_boneRemapCount(0),
	m_leftFrame(0),
	m_rightFrame(0),
	m_frameWeight(0),
	m_speedFactor(1.0f),
	m_totalTicks(0)
{
}

SkinnedMesh1::~SkinnedMesh1()
{
}

void SkinnedMesh1::init(
	Adreno::Model* model,
	Adreno::Animation* anim,
	Texture** modelTexture,
	Shader& shader,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	Material* material,
	std::map<MyString, AnimAction>* animActions,
	FLOAT32 speedFactor)
{
	m_anim = anim;
	m_speedFactor = speedFactor;
	m_animActions.clear();
	if (animActions != nullptr)
	{
		m_animActions = (*animActions);
	}

	throwIfFailed(TRUE == SetupBoneTransform(model, anim), "ERROR: Fail to setup skinned mesh type 1");
	throwIfFailed(TRUE == RemapBoneIndices(model, m_boneRemap, m_boneRemapCount), "ERROR: Fail to setup skinned mesh type 1");

	//FileMesh1::init(model, modelTexture, shader, pos, rot, scale, material);
	FileMesh1::init(model, modelTexture, shader, material);
}

void SkinnedMesh1::update(Timer& timer)
{
	m_totalTicks += (UINT32)(timer.getElapsedTime() * FRM_ANIMATION_TICKS_PER_SEC);

	if (m_speedFactor > 0.0f)
	{
		UINT32 ticksPerFrame = (UINT32)(TICKS_PER_FRAME / m_speedFactor);

		UINT32 frameStart, frameLength;

		if (!m_animActions.empty())
		{
			AnimAction action = getAction(m_currentAction);
			frameStart = action.FrameStart;
			frameLength = action.FrameLength;

			if (frameStart >= m_anim->NumFrames)
			{
				frameStart = 0;
			}
			if (frameLength > m_anim->NumFrames - frameStart)
			{
				frameLength = m_anim->NumFrames - frameStart;
			}
		}
		else
		{
			frameStart = 0;
			frameLength = m_anim->NumFrames;
		}

		frameLength = (frameLength == 0) ? m_anim->NumFrames - frameStart : frameLength;
		UINT32 totalFrames = m_totalTicks / ticksPerFrame;

		m_leftFrame = frameStart + totalFrames % frameLength;
		m_rightFrame = (m_leftFrame != frameStart + frameLength - 1) ? m_leftFrame + 1 : frameStart;
		m_frameWeight = (FLOAT32)(m_totalTicks - totalFrames * ticksPerFrame) / ticksPerFrame;
	}

	FileMesh1::update(timer);
}

void SkinnedMesh1::foreachSubmesh(int index)
{
	// Prepare this frame's transforms for each of the bones
	FRMMATRIX4X3 matWorldMatrixArray[MAX_BONES];
	for (UINT32 boneIndex = 0; boneIndex < m_boneRemapCount; ++boneIndex)
	{
		FRMMATRIX4X4 matBoneMatrix = FrmMatrixIdentity();

		// Get the current and last transforms
		Adreno::Joint* pJoint = m_model->Joints + m_boneRemap[boneIndex];
		Adreno::AnimationTrack* pTrack = m_anim->Tracks + m_boneRemap[boneIndex];

		Adreno::Transform* pCurTransform;
		Adreno::Transform* pLastTransform;

		if (pTrack->NumKeyframes >= 0)
		{
			pCurTransform = pTrack->Keyframes + m_rightFrame;
			pLastTransform = pTrack->Keyframes + m_leftFrame;
		}
		else
		{
			pCurTransform = &pJoint->Transform;
			pLastTransform = &pJoint->Transform;
		}

		// Interpolate between the last and current transforms
		Adreno::Transform transform;
		transform.Position = FrmVector3Lerp(pLastTransform->Position, pCurTransform->Position, m_frameWeight);
		transform.Rotation = FrmVector4SLerp(pLastTransform->Rotation, pCurTransform->Rotation, m_frameWeight);

		// Apply inverse bind transform to final matrix
		FRMMATRIX4X4 matInverseBindPosition = FrmMatrixTranslate(pJoint->InverseBindPose.Position);
		FRMMATRIX4X4 matInverseBindRotation = FrmMatrixRotate(pJoint->InverseBindPose.Rotation);

		matBoneMatrix = matInverseBindRotation;
		matBoneMatrix = FrmMatrixMultiply(matBoneMatrix, matInverseBindPosition);

		// Apply interpolated transform to final matrix
		FRMMATRIX4X4 matBonePosition = FrmMatrixTranslate(transform.Position);
		FRMMATRIX4X4 matBoneRotation = FrmMatrixRotate(transform.Rotation);

		matBoneMatrix = FrmMatrixMultiply(matBoneMatrix, matBoneRotation);
		matBoneMatrix = FrmMatrixMultiply(matBoneMatrix, matBonePosition);

		// Place it in a 4x3 matrix
		matBoneMatrix = FrmMatrixTranspose(matBoneMatrix);
		memcpy(&matWorldMatrixArray[boneIndex], &matBoneMatrix, sizeof(FRMMATRIX4X3));
	}

	// Set bone transforms
	m_shader->setUniform(
		"u_worldArray",
		(FLOAT32*)&matWorldMatrixArray[0],
		MAX_BONES * 3,
		4);
}

MyString SkinnedMesh1::getCurrentAction()const
{
	return m_currentAction;
}

void SkinnedMesh1::setCurrentAction(const MyString& name)
{
	m_currentAction = name;
}

SkinnedMesh1::AnimAction SkinnedMesh1::getAction(const MyString& name)const
{
	auto i = m_animActions.find(name);
	if (i != m_animActions.end())
	{
		return i->second;
	}
	else
	{
		return{ 0, 0 };
	}
}