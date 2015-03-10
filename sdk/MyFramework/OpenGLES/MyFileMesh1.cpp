
#include "MyFileMesh1.h"

#pragma region Helpers

//===============================================================================================================
//
// Helpers
//
//===============================================================================================================

// Returns index for a given property name
INT32 GetPropertyIndexFromName(Adreno::Mesh* pMesh, char* propertyName)
{
	Adreno::NamedId propertyNameId(propertyName);

	for (INT32 propertyIndex = 0; propertyIndex < pMesh->Vertices.Format.NumProperties; ++propertyIndex)
	{
		if (pMesh->Vertices.Format.Properties[propertyIndex].Usage == propertyNameId)
		{
			return propertyIndex;
		}
	}

	return -1;
}

Texture** FileMesh1::initTextures(Adreno::Model* model, CFrmPackedResourceGLES& resource)
{
	Texture** modelTexture = nullptr;

	if (model->NumMaterials > 0)
	{
		modelTexture = new Texture*[model->NumMaterials];
		memset(modelTexture, 0, sizeof(Texture*) * model->NumMaterials);

		for (INT32 materialIndex = 0; materialIndex < model->NumMaterials; ++materialIndex)
		{
			Adreno::Material* pMaterial = model->Materials + materialIndex;

			modelTexture[materialIndex] = new Texture;
			modelTexture[materialIndex]->init(resource.GetTexture(pMaterial->Id.Name));
		}
	}

	return modelTexture;
}

void FileMesh1::destroyTextures(Adreno::Model* model, Texture** modelTexture)
{
	if (modelTexture)
	{
		for (INT32 materialIndex = 0; materialIndex < model->NumMaterials; ++materialIndex)
		{
			delete modelTexture[materialIndex];
		}

		delete[] modelTexture;
	}
}

#pragma endregion

//===============================================================================================================
//
// FileMesh1 class
//
//===============================================================================================================

FileMesh1::FileMesh1()
	: m_model(nullptr),
	m_modelTexture(nullptr),
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_vertexFormatMap(nullptr)
{
}

FileMesh1::~FileMesh1()
{
	// Delete vertex buffers
	if (m_vertexBuffer)
	{
		glDeleteBuffers(m_model->NumMeshes, &m_vertexBuffer[0]);
		delete[] m_vertexBuffer;
	}

	// Delete index buffers
	if (m_indexBuffer)
	{
		glDeleteBuffers(m_model->NumMeshes, &m_indexBuffer[0]);
		delete[] m_indexBuffer;
	}

	// Delete vertex format map
	SAFE_DELETE_ARRAY(m_vertexFormatMap);
}

void FileMesh1::init(
	Adreno::Model* model,
	Texture** modelTexture,
	Shader& shader,
	Material* material)
{
	m_model = model;
	m_modelTexture = modelTexture;

	// Create vertex and index buffers, and map vertex format
	m_vertexBuffer = new GLuint[m_model->NumMeshes];
	memset(m_vertexBuffer, 0, sizeof(GLuint) * m_model->NumMeshes);

	m_indexBuffer = new GLuint[m_model->NumMeshes];
	memset(m_indexBuffer, 0, sizeof(GLuint) * m_model->NumMeshes);

	m_vertexFormatMap = new VERTEX_FORMAT_MAP[m_model->NumMeshes];
	memset(m_vertexFormatMap, 0, sizeof(VERTEX_FORMAT_MAP) * m_model->NumMeshes);

	for (INT32 meshIndex = 0; meshIndex < m_model->NumMeshes; ++meshIndex)
	{
		Adreno::Mesh* pMesh = m_model->Meshes + meshIndex;

		FrmCreateVertexBuffer(pMesh->Vertices.NumVerts, pMesh->Vertices.Format.Stride, pMesh->Vertices.Buffer, &m_vertexBuffer[meshIndex]);
		FrmCreateIndexBuffer(pMesh->Indices.NumIndices, sizeof(UINT32), pMesh->Indices.Indices, &m_indexBuffer[meshIndex]);

		m_vertexFormatMap[meshIndex].position = GetPropertyIndexFromName(pMesh, "position");
		m_vertexFormatMap[meshIndex].normal = GetPropertyIndexFromName(pMesh, "normal");
		m_vertexFormatMap[meshIndex].tangent = GetPropertyIndexFromName(pMesh, "tangent");
		m_vertexFormatMap[meshIndex].binormal = GetPropertyIndexFromName(pMesh, "binormal");
		m_vertexFormatMap[meshIndex].boneIndex = GetPropertyIndexFromName(pMesh, "skinindex");
		m_vertexFormatMap[meshIndex].boneWeight = GetPropertyIndexFromName(pMesh, "skinweight");
		m_vertexFormatMap[meshIndex].texCoord = GetPropertyIndexFromName(pMesh, "texcoord");
	}

	enableLighting();

	Mesh::init(shader, material);
}

void FileMesh1::update(Timer& timer)
{
}

void FileMesh1::render(Camera& camera, Light* light)
{
	Mesh::render(camera, light);

	// Set animation here instead of for each sub-mesh

	// Render each of the meshes
	for (INT32 meshIndex = 0; meshIndex < m_model->NumMeshes; ++meshIndex)
	{
		Adreno::Mesh* pMesh = m_model->Meshes + meshIndex;

		// Set vertex buffer
		FrmSetVertexBuffer(m_vertexBuffer[meshIndex]);

		// Set vertex attributes
		Adreno::VertexProperty* pPositionProperty = (m_vertexFormatMap[meshIndex].position >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].position : nullptr);
		Adreno::VertexProperty* pNormalProperty = (m_vertexFormatMap[meshIndex].normal >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].normal : nullptr);
		Adreno::VertexProperty* pTangentProperty = (m_vertexFormatMap[meshIndex].tangent >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].tangent : nullptr);
		Adreno::VertexProperty* pBinormalProperty = (m_vertexFormatMap[meshIndex].binormal >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].binormal : nullptr);
		Adreno::VertexProperty* pBoneIndexProperty = (m_vertexFormatMap[meshIndex].boneIndex >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].boneIndex : nullptr);
		Adreno::VertexProperty* pBoneWeightProperty = (m_vertexFormatMap[meshIndex].boneWeight >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].boneWeight : nullptr);
		Adreno::VertexProperty* pTexCoordProperty = (m_vertexFormatMap[meshIndex].texCoord >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[meshIndex].texCoord : nullptr);

		if (pPositionProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_POSITION, 3, GL_FLOAT, pPositionProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pPositionProperty->Offset);
			glEnableVertexAttribArray(FRM_VERTEX_POSITION);
		}

		if (pNormalProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_NORMAL, 3, GL_FLOAT, pNormalProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pNormalProperty->Offset);
			glEnableVertexAttribArray(FRM_VERTEX_NORMAL);
		}

		if (pTangentProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_TANGENT, 3, GL_FLOAT, pTangentProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pTangentProperty->Offset);
			glEnableVertexAttribArray(FRM_VERTEX_TANGENT);
		}

		if (pBinormalProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_BINORMAL, 3, GL_FLOAT, pBinormalProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pBinormalProperty->Offset);
			glEnableVertexAttribArray(FRM_VERTEX_BINORMAL);
		}

		if (pBoneIndexProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_CUSTOM_BONEINDEX1, 1, GL_UNSIGNED_BYTE, pBoneIndexProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)(pBoneIndexProperty->Offset + 0));
			glEnableVertexAttribArray(FRM_VERTEX_CUSTOM_BONEINDEX1);
		}

#if defined( USE_TWO_BONES ) || defined( USE_THREE_BONES )
		if (pBoneIndexProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_CUSTOM_BONEINDEX2, 1, GL_UNSIGNED_BYTE, pBoneIndexProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)(pBoneIndexProperty->Offset + 4));
			glEnableVertexAttribArray(FRM_VERTEX_CUSTOM_BONEINDEX2);
		}
#endif

#if defined( USE_THREE_BONES )
		if (pBoneIndexProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_CUSTOM_BONEINDEX3, 1, GL_UNSIGNED_BYTE, pBoneIndexProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)(pBoneIndexProperty->Offset + 8));
			glEnableVertexAttribArray(FRM_VERTEX_CUSTOM_BONEINDEX3);
		}
#endif

		if (pBoneWeightProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_BONEWEIGHTS, 3, GL_FLOAT, pBoneWeightProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pBoneWeightProperty->Offset);
			glEnableVertexAttribArray(FRM_VERTEX_BONEWEIGHTS);
		}

		if (pTexCoordProperty != nullptr)
		{
			glVertexAttribPointer(FRM_VERTEX_TEXCOORD0, 2, GL_FLOAT, pTexCoordProperty->IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pTexCoordProperty->Offset);
			glEnableVertexAttribArray(FRM_VERTEX_TEXCOORD0);
		}
		
		/*
		glVertexAttribPointer(FRM_VERTEX_TEXCOORD0, 2, GL_FLOAT, pMesh->Vertices.Format.Properties[6].IsNormalized(), pMesh->Vertices.Format.Stride, (GLvoid*)pMesh->Vertices.Format.Properties[6].Offset);
		glEnableVertexAttribArray(FRM_VERTEX_TEXCOORD0);
		/**/

		// Set index buffer
		FrmSetIndexBuffer(m_indexBuffer[meshIndex]);

		//foreachSubmesh(meshIndex);

		// Render each mesh surface
		for (UINT32 surfaceIndex = 0; surfaceIndex < pMesh->Surfaces.NumSurfaces; ++surfaceIndex)
		{
			Adreno::MeshSurface* pSurface = pMesh->Surfaces.Surfaces + surfaceIndex;

			// Set the material for the surface
			m_shader->setUniform("u_diffuseSampler", m_modelTexture[pSurface->MaterialId]->bind());

			// Draw the surface
			int id(0);
			for (auto i = m_instances.begin(); i != m_instances.end(); ++i)
			{
				m_shader->setUniform("u_world", (*i)->World);
				foreachInstance(id++);

				glDrawElements(GL_TRIANGLES, pSurface->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)(pSurface->StartIndex * sizeof(UINT32)));
			}
		}
	}

	FrmSetVertexBuffer(NULL);
	FrmSetIndexBuffer(NULL);
}