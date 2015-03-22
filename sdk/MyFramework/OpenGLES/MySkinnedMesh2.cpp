
#include "MySkinnedMesh2.h"

#pragma region Helper

//===============================================================================================================
//
// Helper
//
//===============================================================================================================

SkinnedMesh2::Instance* SkinnedMesh2::buildSkinnedMeshInstance(const MyVec3& pos, const MyVec3& rot, const MyVec3& scale, const MyString& action)
{
	SkinnedMesh2::Instance* instance = new SkinnedMesh2::Instance;

	instance->Position = pos;
	instance->Rotation = rot;
	instance->Scale = scale;

	instance->CurrentAction = action;
	instance->TotalTicks = 0;

	return instance;
}

static VOID BindAnimationDataToMesh(FRM_ANIMATION_SET* pAnimationSet, CFrmMesh* pMesh)
{
	// Bind the animation to the mesh frames
	for (UINT32 i = 0; i < pAnimationSet->m_nNumAnimations; i++)
	{
		for (UINT32 j = 0; j < pMesh->m_nNumFrames; j++)
		{
			if (!strcmp(pMesh->m_pFrames[j].m_strName,
				pAnimationSet->m_pAnimations[i].m_strFrameName))
			{
				pAnimationSet->m_pAnimations[i].m_pMeshFrame = &pMesh->m_pFrames[j];
				break;
			}
		}
	}
}

static VOID SetAnimationTime(FRM_ANIMATION_SET* pAnimationSet, UINT32 nPeriodicAnimTime)
{
	nPeriodicAnimTime = nPeriodicAnimTime % pAnimationSet->m_nPeriodInTicks;

	for (UINT32 i = 0; i<pAnimationSet->m_nNumAnimations; i++)
	{
		FRM_ANIMATION*  pAnimation = &pAnimationSet->m_pAnimations[i];
		FRM_MESH_FRAME* pMeshFrame = pAnimation->m_pMeshFrame;
		if (NULL == pMeshFrame)
			continue;

		UINT32 key1 = 0;
		while (pAnimation->m_pKeyTimes[key1] < nPeriodicAnimTime)
			key1++;

		UINT32 key0 = key1 - 1;
		if (key1 == 0)
			key0 = pAnimationSet->m_nNumAnimations - 1;

		UINT32 nTime0 = pAnimation->m_pKeyTimes[key0];
		UINT32 nTime1 = pAnimation->m_pKeyTimes[key1];
		FLOAT32  fLerp = (FLOAT32)(nPeriodicAnimTime - nTime0) / (FLOAT32)(nTime1 - nTime0);

		FRM_ANIMATION_KEY* pKey0 = &pAnimation->m_pKeyValues[key0];
		FRM_ANIMATION_KEY* pKey1 = &pAnimation->m_pKeyValues[key1];

		// Lerp the values for pKey0 and pKey1
		FRMVECTOR3 v3Tmp = FrmVector3Lerp(pKey0->m_vScale, pKey1->m_vScale, fLerp);
		FRMMATRIX4X4 matScale = FrmMatrixScale(v3Tmp);
		FRMVECTOR4 v4Tmp = FrmVector4SLerp(pKey0->m_vRotate, pKey1->m_vRotate, fLerp);
		FRMMATRIX4X4 matRotate = FrmMatrixRotate(v4Tmp);
		v3Tmp = FrmVector3Lerp(pKey0->m_vTranslate, pKey1->m_vTranslate, fLerp);
		FRMMATRIX4X4 matTranslate = FrmMatrixTranslate(v3Tmp);

		// Build the matrix transform for the mesh frame
		pMeshFrame->m_matTransform = FrmMatrixMultiply(matScale, matRotate);
		pMeshFrame->m_matTransform = FrmMatrixMultiply(pMeshFrame->m_matTransform, matTranslate);
	}
}

#pragma endregion

//===============================================================================================================
//
// SkinnedMesh class
//
//===============================================================================================================

SkinnedMesh2::SkinnedMesh2()
	: m_mesh(nullptr),
	m_animationSet(nullptr)
{
}

SkinnedMesh2::~SkinnedMesh2()
{
}

void SkinnedMesh2::init(
	CFrmMesh& mesh,
	FRM_ANIMATION_SET* animationSet,
	CFrmPackedResourceGLES& resource,
	Shader& shader,
	Material* material,
	std::map<MyString, AnimAction>* animActions)
{
	m_mesh = &mesh;
	m_animationSet = animationSet;

	if (animActions != nullptr)
	{
		for (auto i = animActions->begin(); i != animActions->end(); ++i)
		{
			m_animActions.insert(std::pair<MyString, AnimAction>(i->first, i->second));
		}
	}

	m_mesh->MakeDrawable(&resource);
	BindAnimationDataToMesh(m_animationSet, m_mesh);

	enableLighting();

	Mesh::init(shader, material);
}

void SkinnedMesh2::update(Timer& timer)
{
	for (auto i = m_instances.begin(); i != m_instances.end(); ++i)
	{
		SkinnedMesh2::Instance* instance = (SkinnedMesh2::Instance*)(*i);

		instance->TotalTicks += (UINT32)(timer.getElapsedTime() * FRM_ANIMATION_TICKS_PER_SEC);
	}

	Mesh::update(timer);
}

void SkinnedMesh2::render(Camera& camera, Light* light)
{
	Mesh::render(camera, light);

	FRM_MESH* pMesh = m_mesh->m_pFrames[0].m_pMesh;

	// Set the buffers
	FrmSetVertexBuffer(pMesh->m_hVertexBuffer);
	FrmSetVertexLayout(pMesh->m_VertexLayout, pMesh->m_nVertexSize);
	FrmSetIndexBuffer(pMesh->m_hIndexBuffer);

	for (auto i = m_instances.begin(); i != m_instances.end(); ++i)
	{
		SkinnedMesh2::Instance* instance = (SkinnedMesh2::Instance*)(*i);

		// Set transform for bones
		if (!m_animActions.empty())
		{
			UINT32 start = (UINT32)(getAction(instance->CurrentAction).TimeStart * FRM_ANIMATION_TICKS_PER_SEC);
			UINT32 end = (UINT32)(getAction(instance->CurrentAction).TimeEnd * FRM_ANIMATION_TICKS_PER_SEC);

			SetAnimationTime(m_animationSet, instance->TotalTicks % (end - start + 1) + start);
		}
		else
		{
			SetAnimationTime(m_animationSet, instance->TotalTicks);
		}

		// Draw all subsets

		m_shader->setUniform("u_world", instance->World);

		for (UINT32 nSubset = 0; nSubset < pMesh->m_nNumSubsets; nSubset++)
		{
			FRM_MESH_SUBSET* pSubset = &pMesh->m_pSubsets[nSubset];

			m_shader->setUniform("u_diffuseSampler", 0);

			// Setup the material of the mesh subset
			for (UINT32 iTexture = 0; iTexture < pSubset->m_nNumTextures; iTexture++)
			{
				FrmSetTexture(
					iTexture,
					pSubset->m_pTextures[iTexture].m_pTexture ? pSubset->m_pTextures[iTexture].m_pTexture->m_hTextureHandle : NULL);
			}

			// Build a palette of 4x3 world matrices
			FRMMATRIX4X3 matWorldMatrixArray[MAX_BONES];
			for (UINT32 iBone = 0; iBone < pMesh->m_nNumBoneMatrices; ++iBone)
			{
				UINT32 nBoneId = pSubset->m_pBoneIds[iBone];
				if (nBoneId < pMesh->m_nNumBones)
				{
					FRMMATRIX4X4 matBoneMatrix = pMesh->m_pBones[nBoneId].m_pFrame->m_matTransform;

					// Isolate the 4x3 portion of the transposed bone matrix
					matBoneMatrix = FrmMatrixTranspose(matBoneMatrix);
					memcpy(&matWorldMatrixArray[iBone], &matBoneMatrix, sizeof(FRMMATRIX4X3));
				}
			}

			// Set the bone matrix palette to the vertex shader
			m_shader->setUniform(
				"u_worldArray",
				(FLOAT32*)&matWorldMatrixArray[0],
				pMesh->m_nNumBoneMatrices * 3,
				4);

			// Draw the mesh
			FrmDrawIndexedVertices(
				pSubset->m_nPrimType,
				pSubset->m_nNumIndices,
				pMesh->m_nIndexSize,
				pSubset->m_nIndexOffset);
		}

	}

	FrmSetVertexBuffer(NULL);
}

SkinnedMesh2::AnimAction SkinnedMesh2::getAction(const MyString& name)const
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