
#pragma once

#include "MyMesh.h"
#include "Model/FrmModelIO.h"
#include "FrmPackedResourceGLES.h"
#include "MyTexture.h"

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
	UINT32 position;
	UINT32 normal;
	UINT32 tangent;
	UINT32 binormal;
	UINT32 boneIndex;
	UINT32 boneWeight;
	UINT32 texCoord;
};

//===============================================================================================================
//
// Wrapper for file *.model
//
//===============================================================================================================

class FileMesh1 : public Mesh
{
public:
	FileMesh1();
	virtual ~FileMesh1();

	/*
	virtual void init(
		Adreno::Model* model,
		Texture** modelTexture,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		Material* material = nullptr);
	/**/
	virtual void init(
		Adreno::Model* model,
		Texture** modelTexture,
		Shader& shader,
		Material* material = nullptr);

	virtual void update(Timer& timer);
	virtual void render(Camera& camera, Light* light = nullptr);

public:
	static Texture** initTextures(Adreno::Model* model, CFrmPackedResourceGLES& resource);
	static void destroyTextures(Adreno::Model* model, Texture** modelTexture);

protected:
	virtual void foreachSubmesh(int index){}

protected:
	Adreno::Model* m_model;
	Texture**      m_modelTexture;

	GLuint* m_vertexBuffer;
	GLuint* m_indexBuffer;

	VERTEX_FORMAT_MAP* m_vertexFormatMap;
};