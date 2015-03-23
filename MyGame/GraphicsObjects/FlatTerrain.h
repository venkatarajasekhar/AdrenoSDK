
#pragma once

#include <MyBasicMesh.h>

struct FlatTerrainProperties
{
	MyVec3 TileRepeatFactor;
	MyVec2 Threshold;
};

class FlatTerrain
{
public:
	FlatTerrain();
	~FlatTerrain();

	void init(
		Shader& shader,
		Texture& diffuseMap1,
		Texture& diffuseMap2,
		Texture& blendMap,
		const MyVec2& size,
		FlatTerrainProperties& properties);

	void render(Camera& camera);

private:
	BasicMesh m_mesh;

	Texture *m_diffuseMap1, *m_diffuseMap2, *m_blendMap;

	FlatTerrainProperties m_properties;
};