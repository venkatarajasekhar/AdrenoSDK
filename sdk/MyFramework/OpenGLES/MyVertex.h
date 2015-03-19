#pragma once

#include "MyShader.h"

struct PosColorVertex
{
	PosColorVertex(){}
	PosColorVertex(float x, float y, float z, float r, float g, float b)
		: pos(x, y, z),
		color(r, g, b)
	{}
	PosColorVertex(MyVec3 _pos, MyColor _color)
		: pos(_pos),
		color(_color)
	{}

	MyVec3  pos;
	MyColor color;

	static const int NumShaderAttribsDesc = 2;
	static const SHADER_ATTRIBUTE_DESC ShaderAttribsDesc[NumShaderAttribsDesc];
};

struct PosTexVertex
{
	PosTexVertex(){}
	PosTexVertex(float x, float y, float z, float u, float v)
		: pos(x, y, z),
		tex(u, v)
	{}
	PosTexVertex(MyVec3 _pos, MyVec2 _tex)
		: pos(_pos),
		tex(_tex)
	{}

	MyVec3 pos;
	MyVec2 tex;

	static const int NumShaderAttribsDesc = 2;
	static const SHADER_ATTRIBUTE_DESC ShaderAttribsDesc[NumShaderAttribsDesc];
};

struct PosNorTexVertex
{
	PosNorTexVertex(){}
	PosNorTexVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		: pos(x, y, z),
		nor(nx, ny, nz),
		tex(u, v)
	{}
	PosNorTexVertex(MyVec3 _pos, MyVec3 _nor, MyVec2 _tex)
		: pos(_pos),
		nor(_nor),
		tex(_tex)
	{}

	MyVec3 pos;
	MyVec3 nor;
	MyVec2 tex;

	static const int NumShaderAttribsDesc = 3;
	static const SHADER_ATTRIBUTE_DESC ShaderAttribsDesc[NumShaderAttribsDesc];
};

struct Pos2TexVertex
{
	Pos2TexVertex(){}
	Pos2TexVertex(float x, float y, float u, float v)
		: pos(x, y),
		tex(u, v)
	{}
	Pos2TexVertex(MyVec2 _pos, MyVec2 _tex)
		: pos(_pos),
		tex(_tex)
	{}

	MyVec2 pos;
	MyVec2 tex;

	static const int NumShaderAttribsDesc = 2;
	static const SHADER_ATTRIBUTE_DESC ShaderAttribsDesc[NumShaderAttribsDesc];
};

struct SkinnedVertex
{
	static const int NumShaderAttribsDesc = 5;
	static const SHADER_ATTRIBUTE_DESC ShaderAttribsDesc[NumShaderAttribsDesc];
};

struct BillboardVertex
{
	BillboardVertex()
	{}

	BillboardVertex(float u, float v)
		: tex(u, v)
	{}

	BillboardVertex(MyVec2 _tex)
		: tex(_tex)
	{}

	MyVec2 tex;

	static const int NumShaderAttribsDesc = 1;
	static const SHADER_ATTRIBUTE_DESC ShaderAttribsDesc[NumShaderAttribsDesc];
};