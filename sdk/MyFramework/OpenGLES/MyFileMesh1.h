
#pragma once

#include "MyMesh.h"
#include "Model/FrmModelIO.h"
#include "FrmPackedResourceGLES.h"
#include "MyTexture.h"

#pragma region struct & enum

//===============================================================================================================
//
// struct & enum
//
//===============================================================================================================

enum FRM_VERTEX_USAGE_CUSTOM
{
	FRM_VERTEX_CUSTOM_BONEINDEX1 = FRM_VERTEX_BONEINDICES,
	FRM_VERTEX_CUSTOM_BONEINDEX2 = FRM_VERTEX_COLOR0,
	FRM_VERTEX_CUSTOM_BONEINDEX3 = FRM_VERTEX_COLOR1,
};

struct VERTEX_FORMAT_MAP
{
	INT32 position;
	INT32 normal;
	INT32 tangent;
	INT32 binormal;
	INT32 boneIndex;
	INT32 boneWeight;
	INT32 texCoord;
};

#pragma endregion

//===============================================================================================================
//
// Wrapper for file *.model
//
//===============================================================================================================

class FileMesh1 : public Mesh
{

#pragma region Wrapper for mesh data

public:
	struct MeshData
	{
		MeshData();
		~MeshData();

		void init(const MyString& filename);

		Adreno::Model* Data;
	};

	struct MeshTextures
	{
		MeshTextures();
		~MeshTextures();

		void init(FileMesh1::MeshData& meshData, CFrmPackedResourceGLES& resource);

		Texture** Textures;
		int NumTextures;
	};

#pragma endregion

public:
	FileMesh1();
	virtual ~FileMesh1();

	virtual void init(
		FileMesh1::MeshData& model,
		FileMesh1::MeshTextures& modelTexture,
		Shader& shader,
		Material* material = nullptr);

	virtual void update(Timer& timer);
	virtual void render(Camera& camera, Light* light = nullptr);

protected:
	void prepareRenderSubmesh(int submesh);

protected:
	Adreno::Model* m_model;
	Texture**      m_modelTexture;

	GLuint* m_vertexBuffer;
	GLuint* m_indexBuffer;

	VERTEX_FORMAT_MAP* m_vertexFormatMap;
};