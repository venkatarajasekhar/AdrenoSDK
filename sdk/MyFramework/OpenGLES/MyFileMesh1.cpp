
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

#pragma endregion

#pragma region FileMesh1 Helpers

//===============================================================================================================
//
// FileMesh1 Helpers
//
//===============================================================================================================

void FileMesh1::prepareRenderSubmesh(int submesh)
{
	Adreno::Mesh* pMesh = m_model->Meshes + submesh;

	// Set vertex buffer
	FrmSetVertexBuffer(m_vertexBuffer[submesh]);

	// Set vertex attributes
	Adreno::VertexProperty* pPositionProperty = (m_vertexFormatMap[submesh].position >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].position : nullptr);
	Adreno::VertexProperty* pNormalProperty = (m_vertexFormatMap[submesh].normal >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].normal : nullptr);
	Adreno::VertexProperty* pTangentProperty = (m_vertexFormatMap[submesh].tangent >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].tangent : nullptr);
	Adreno::VertexProperty* pBinormalProperty = (m_vertexFormatMap[submesh].binormal >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].binormal : nullptr);
	Adreno::VertexProperty* pBoneIndexProperty = (m_vertexFormatMap[submesh].boneIndex >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].boneIndex : nullptr);
	Adreno::VertexProperty* pBoneWeightProperty = (m_vertexFormatMap[submesh].boneWeight >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].boneWeight : nullptr);
	Adreno::VertexProperty* pTexCoordProperty = (m_vertexFormatMap[submesh].texCoord >= 0 ? pMesh->Vertices.Format.Properties + m_vertexFormatMap[submesh].texCoord : nullptr);

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

	// Set index buffer
	FrmSetIndexBuffer(m_indexBuffer[submesh]);
}

#pragma endregion

#pragma region Wrapper for mesh data

//===============================================================================================================
//
// FileMesh1::MeshTextures struct
//
//===============================================================================================================

FileMesh1::MeshData::MeshData()
	: Data(nullptr)
{}

FileMesh1::MeshData::~MeshData()
{
	Adreno::FrmDestroyLoadedModel(Data);
}

void FileMesh1::MeshData::init(const MyString& filename)
{
	Data = Adreno::FrmLoadModelFromFile(filename.c_str());
}

//===============================================================================================================
//
// FileMesh1::MeshTextures struct
//
//===============================================================================================================

FileMesh1::MeshTextures::MeshTextures()
	: NumTextures(0)
{
	Textures = nullptr;
}

FileMesh1::MeshTextures::~MeshTextures()
{
	if (Textures)
	{
		for (INT32 i = 0; i < NumTextures; ++i)
		{
			delete Textures[i];
		}

		delete[] Textures;
	}
}

void FileMesh1::MeshTextures::init(FileMesh1::MeshData& meshData, CFrmPackedResourceGLES& resource)
{
	auto model = meshData.Data;

	if (model->NumMaterials > 0)
	{
		NumTextures = model->NumMaterials;
		Textures = new Texture*[model->NumMaterials];
		memset(Textures, 0, sizeof(Texture*) * model->NumMaterials);

		for (INT32 materialIndex = 0; materialIndex < model->NumMaterials; ++materialIndex)
		{
			Adreno::Material* pMaterial = model->Materials + materialIndex;

			Textures[materialIndex] = new Texture;
			Textures[materialIndex]->init(resource.GetTexture(pMaterial->Id.Name));
		}
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
	FileMesh1::MeshData& model,
	FileMesh1::MeshTextures& modelTexture,
	Shader& shader,
	Material* material)
{
	m_model = model.Data;
	m_modelTexture = modelTexture.Textures;

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

	// Render each of the meshes
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
			for (auto i = m_instances.begin(); i != m_instances.end(); ++i)
			{
				if ((*i)->Visible)
				{
					m_shader->setUniform("u_world", (*i)->World);
					glDrawElements(GL_TRIANGLES, pSurface->NumTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)(pSurface->StartIndex * sizeof(UINT32)));
				}
			}
		}
	}

	FrmSetVertexBuffer(NULL);
	FrmSetIndexBuffer(NULL);
}