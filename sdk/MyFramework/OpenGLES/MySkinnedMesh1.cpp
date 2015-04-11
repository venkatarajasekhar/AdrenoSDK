
#include "MySkinnedMesh1.h"
#include "FrmMesh.h"

#pragma region Helpers

//===============================================================================================================
//
// Helpers
//
//===============================================================================================================

INT32 GetPropertyIndexFromName(Adreno::Mesh* pMesh, char* propertyName);

SkinnedMesh1::Instance* SkinnedMesh1::buildSkinnedMeshInstance(
	const MyVec3& pos, 
	const MyVec3& rot, 
	const MyVec3& scale, 
	const MyString& action)
{
	SkinnedMesh1::Instance* instance = new SkinnedMesh1::Instance;

	instance->Position = pos;
	instance->Rotation = rot;
	instance->Scale = scale;

	instance->CurrentAction = action;

	return instance;
}

/*
bool SkinnedMesh1::mergeAnimFile(
	AnimFile* animFiles,
	int numAnimFiles,
	Adreno::Animation*& mergedAnim,
	std::map<MyString, AnimAction>& actionsMap)
{
	INT32 numTracks(0), numFrames(0);
	Adreno::Animation* firstAnim(nullptr);
	std::map<MyString, Adreno::Animation*> animsMap;

	mergedAnim = new Adreno::Animation;

	if (animFiles == nullptr || numAnimFiles == 0)
	{
		smartLog("ERROR: List of animation file is empty.");
		return false;
	}

	// Init animation files
	for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
	{
		auto pAnimFile = animFiles + iAnim;

		auto ite = animsMap.find(pAnimFile->FileName);
		if (ite == animsMap.end())
		{
			auto anim = Adreno::FrmLoadAnimationFromFile(pAnimFile->FileName.c_str());
			animsMap[pAnimFile->FileName] = anim;
			pAnimFile->Anim = anim;
		}
		else
		{
			pAnimFile->Anim = ite->second;
		}
	}

	// Computing numTracks and numFrames
	firstAnim = animFiles[0].Anim;
	numTracks = firstAnim->NumTracks;

	for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
	{
		auto pAnimFile = animFiles + iAnim;

		if (pAnimFile->Anim->NumTracks != numTracks)
		{
			smartLog("ERROR: Number of bones of animation files is not the same.");
			return false;
		}

		UINT32 frameStart = pAnimFile->Range.FrameStart;
		UINT32 frameLength = pAnimFile->Range.FrameLength;

		if (frameStart >= pAnimFile->Anim->NumFrames)
		{
			frameStart = 0;
		}
		if (frameLength > pAnimFile->Anim->NumFrames - frameStart)
		{
			frameLength = pAnimFile->Anim->NumFrames - frameStart;
		}

		frameLength = (frameLength == 0) ? pAnimFile->Anim->NumFrames - frameStart : frameLength;

		pAnimFile->Range.FrameStart = frameStart;
		pAnimFile->Range.FrameLength = frameLength;

		numFrames += frameLength;
	}

	// Merging animation files
	mergedAnim->ResizeTracks(numTracks);
	mergedAnim->NumFrames = numFrames;

	for (int iTrack = 0; iTrack < numTracks; iTrack++)
	{
		auto mergedAnimTrack = mergedAnim->Tracks + iTrack;

		mergedAnimTrack->SetName(firstAnim->Tracks[iTrack].Id.Name);
		mergedAnimTrack->ResizeKeyframes(numFrames);
		
		int count(0);
		
		for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
		{
			auto pAnimFile = animFiles + iAnim;
			auto animTrack = pAnimFile->Anim->Tracks + iTrack;

			for (int iFrame = pAnimFile->Range.FrameStart; iFrame < pAnimFile->Range.FrameStart + pAnimFile->Range.FrameLength; iFrame++)
			{
				if (iFrame >= 0 && iFrame < animTrack->NumKeyframes)
				{
					mergedAnimTrack->Keyframes[count++] = animTrack->Keyframes[iFrame];
				}
			}
		}
	}

	// Destroy animation files
	for (auto i = animsMap.begin(); i != animsMap.end(); ++i)
	{
		Adreno::FrmDestroyLoadedAnimation(i->second);
	}

	// Build action map
	UINT32 frameStart(0);
	for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
	{
		auto pAnimFile = animFiles + iAnim;

		actionsMap[pAnimFile->Name] = { frameStart, pAnimFile->Range.FrameLength };
		frameStart += pAnimFile->Range.FrameLength;
	}

	return true;
}
/**/

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
		smartLog("ERROR: Number of bones in model and animation is missmatch");
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
			FLOAT32 maxBoneWeight = -1;
			UINT32 maxBoneIndex = 0;

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
							smartLog("ERROR: Number of bones in model exceeds maximum " + toString(SkinnedMesh1::MAX_BONES) + " bones");
							return FALSE;
						}

						processedBone[boneIndex] = boneRemapCount;
						boneRemap[boneRemapCount++] = boneIndex;
					}

					*(pBoneIndex + boneCount) = processedBone[boneIndex];
				}

				// Determine max bone weight
				if (boneWeight > maxBoneWeight)
				{
					maxBoneWeight = boneWeight;
					maxBoneIndex = *(pBoneIndex + boneCount);
				}
			}

			// It's only support that a vertex is affected by a bone
			(*pBoneWeight) = 1.0;
			(*pBoneIndex) = maxBoneIndex;
		}
	}

	return TRUE;
}

#pragma endregion

#pragma region Structs

//===============================================================================================================
//
// SkinnedMesh1::Instance struct
//
//===============================================================================================================

SkinnedMesh1::Instance::Instance()
	: LeftFrame(0), 
	RightFrame(0), 
	FrameWeight(0.0f), 
	LoopedAction(true), 
	TotalTicks(0)
{}

void SkinnedMesh1::Instance::setAction(const MyString& action, const MyString& nextAction, bool looped)
{
	CurrentAction = action;
	LoopedAction = looped;
	NextAction = nextAction;

	if (!looped)
	{
		TotalTicks = 0;
	}
}

//===============================================================================================================
//
// SkinnedMesh1::AnimData struct
//
//===============================================================================================================

SkinnedMesh1::AnimData::AnimData()
	: Anim(nullptr)
{}

SkinnedMesh1::AnimData::~AnimData()
{
	Adreno::FrmDestroyLoadedAnimation(Anim);
}

void SkinnedMesh1::AnimData::init(const MyString& filename, std::map<MyString, AnimAction>* animActions)
{
	Anim = Adreno::FrmLoadAnimationFromFile(filename.c_str());
	if (animActions != nullptr)
	{
		AnimActions = (*animActions);
	}
}

void SkinnedMesh1::AnimData::init(AnimFile* animFiles, int numAnimFiles)
{
	INT32 numTracks(0), numFrames(0);
	Adreno::Animation* firstAnim(nullptr);
	std::map<MyString, Adreno::Animation*> animsMap;

	Anim = new Adreno::Animation;

	// Init animation files
	for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
	{
		auto pAnimFile = animFiles + iAnim;

		auto ite = animsMap.find(pAnimFile->FileName);
		if (ite == animsMap.end())
		{
			auto anim = Adreno::FrmLoadAnimationFromFile(pAnimFile->FileName.c_str());
			animsMap[pAnimFile->FileName] = anim;
			pAnimFile->Anim = anim;
		}
		else
		{
			pAnimFile->Anim = ite->second;
		}
	}

	// Computing numTracks and numFrames
	firstAnim = animFiles[0].Anim;
	numTracks = firstAnim->NumTracks;

	for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
	{
		auto pAnimFile = animFiles + iAnim;

		UINT32 frameStart = pAnimFile->Range.FrameStart;
		UINT32 frameLength = pAnimFile->Range.FrameLength;

		if (frameStart >= pAnimFile->Anim->NumFrames)
		{
			frameStart = 0;
		}
		if (frameLength > pAnimFile->Anim->NumFrames - frameStart)
		{
			frameLength = pAnimFile->Anim->NumFrames - frameStart;
		}

		frameLength = (frameLength == 0) ? pAnimFile->Anim->NumFrames - frameStart : frameLength;

		pAnimFile->Range.FrameStart = frameStart;
		pAnimFile->Range.FrameLength = frameLength;

		numFrames += frameLength;
	}

	// Merging animation files
	Anim->ResizeTracks(numTracks);
	Anim->NumFrames = numFrames;

	for (int iTrack = 0; iTrack < numTracks; iTrack++)
	{
		auto mergedAnimTrack = Anim->Tracks + iTrack;

		mergedAnimTrack->SetName(firstAnim->Tracks[iTrack].Id.Name);
		mergedAnimTrack->ResizeKeyframes(numFrames);

		int count(0);

		for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
		{
			auto pAnimFile = animFiles + iAnim;
			auto animTrack = pAnimFile->Anim->Tracks + iTrack;

			for (int iFrame = pAnimFile->Range.FrameStart; iFrame < pAnimFile->Range.FrameStart + pAnimFile->Range.FrameLength; iFrame++)
			{
				if (iFrame >= 0 && iFrame < animTrack->NumKeyframes)
				{
					mergedAnimTrack->Keyframes[count++] = animTrack->Keyframes[iFrame];
				}
			}
		}
	}

	// Destroy animation files
	for (auto i = animsMap.begin(); i != animsMap.end(); ++i)
	{
		Adreno::FrmDestroyLoadedAnimation(i->second);
	}

	// Build action map
	UINT32 frameStart(0);
	for (int iAnim = 0; iAnim < numAnimFiles; ++iAnim)
	{
		auto pAnimFile = animFiles + iAnim;

		AnimActions[pAnimFile->Name] = { frameStart, pAnimFile->Range.FrameLength };
		frameStart += pAnimFile->Range.FrameLength;
	}
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
	m_speedFactor(1.0f)
{
}

SkinnedMesh1::~SkinnedMesh1()
{
}

void SkinnedMesh1::init(
	FileMesh1::MeshData& model,
	SkinnedMesh1::AnimData& anim,
	FileMesh1::MeshTextures& modelTexture,
	Shader& shader,
	Material* material,
	FLOAT32 speedFactor)
{
	m_anim = anim.Anim;
	m_speedFactor = speedFactor;
	m_animActions = anim.AnimActions;
	
	throwIfFailed(TRUE == SetupBoneTransform(model.Data, m_anim), "ERROR: Fail to setup skinned mesh type 1");
	throwIfFailed(TRUE == RemapBoneIndices(model.Data, m_boneRemap, m_boneRemapCount), "ERROR: Fail to setup skinned mesh type 1");

	FileMesh1::init(model, modelTexture, shader, material);
}

void SkinnedMesh1::update(Timer& timer)
{
	UINT32 ticksPerFrame = (UINT32)(TICKS_PER_FRAME / m_speedFactor);

	for (auto i = m_instances.begin(); i != m_instances.end(); ++i)
	{
		SkinnedMesh1::Instance* instance = (SkinnedMesh1::Instance*)(*i);

		instance->TotalTicks += (UINT32)(timer.getElapsedTime() * FRM_ANIMATION_TICKS_PER_SEC);

		if (m_speedFactor > 0.0f)
		{
			UINT32 frameStart, frameLength;

			// Computing frameStart and frameLength
			if (!m_animActions.empty())
			{
				AnimAction action = getAction(instance->CurrentAction);
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

			// Computing LeftFrame, RightFrame and FrameWeight
			frameLength = (frameLength == 0) ? m_anim->NumFrames - frameStart : frameLength;

			UINT32 totalFrames = instance->TotalTicks / ticksPerFrame;
			
			instance->LeftFrame = frameStart + totalFrames % frameLength;
			if (instance->LeftFrame == frameStart + frameLength - 1)
			{
				instance->RightFrame = frameStart;
				if (!instance->LoopedAction)
				{
					instance->setAction(instance->NextAction);
				}
			}
			else
			{
				instance->RightFrame = instance->LeftFrame + 1;
			}

			//instance->RightFrame = (instance->LeftFrame != frameStart + frameLength - 1) ? instance->LeftFrame + 1 : frameStart;
			instance->FrameWeight = (FLOAT32)(instance->TotalTicks - totalFrames * ticksPerFrame) / ticksPerFrame;
		}
	}

	FileMesh1::update(timer);
}

void SkinnedMesh1::render(Camera& camera, Light* light)
{
	Mesh::render(camera, light);

	for (auto i = m_instances.begin(); i != m_instances.end(); ++i)
	{
		SkinnedMesh1::Instance* instance = (SkinnedMesh1::Instance*)(*i);

		// Set uniform for each instance
		m_shader->setUniform("u_world", instance->World);
		setWorldArray(instance);

		// Draw mesh
		for (INT32 meshIndex = 0; meshIndex < m_model->NumMeshes; ++meshIndex)
		{
			Adreno::Mesh* pMesh = m_model->Meshes + meshIndex;

			prepareRenderSubmesh(meshIndex);

			// Render each mesh surface
			for (UINT32 surfaceIndex = 0; surfaceIndex < pMesh->Surfaces.NumSurfaces; ++surfaceIndex)
			{
				Adreno::MeshSurface* pSurface = pMesh->Surfaces.Surfaces + surfaceIndex;

				// Set the material for the surface
				m_shader->setUniform("u_diffuseSampler", m_modelTexture[pSurface->MaterialId]->bind());

				// Draw the surface
				glDrawElements(GL_TRIANGLES, pSurface->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)(pSurface->StartIndex * sizeof(UINT32)));
			}
		}
	}

	FrmSetVertexBuffer(NULL);
	FrmSetIndexBuffer(NULL);
}

void SkinnedMesh1::setWorldArray(SkinnedMesh1::Instance* instance)
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
			pCurTransform = pTrack->Keyframes + instance->RightFrame;
			pLastTransform = pTrack->Keyframes + instance->LeftFrame;
		}
		else
		{
			pCurTransform = &pJoint->Transform;
			pLastTransform = &pJoint->Transform;
		}

		// Interpolate between the last and current transforms
		Adreno::Transform transform;
		transform.Position = FrmVector3Lerp(pLastTransform->Position, pCurTransform->Position, instance->FrameWeight);
		transform.Rotation = FrmVector4SLerp(pLastTransform->Rotation, pCurTransform->Rotation, instance->FrameWeight);

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