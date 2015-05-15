
#pragma once

#include <MyBasicMesh.h>
#include <MyEventListener.h>
#include <MyInput.h>

//========================================================================================================
//
// FlatTerrainProperties struct
//
//========================================================================================================

struct FlatTerrainProperties
{
	// TileRepeatFactor.x: Tile factor for diffuse texture 1
	// TileRepeatFactor.y: Tile factor for diffuse texture 2
	// TileRepeatFactor.z: Tile factor for blending map
	MyVec3 TileRepeatFactor;

	// Threshold to begin blending:
	//	- if 0 <= blending_value < Threshold.x: No blending at all, take diffuse texture 1
	//	- if Threshold.y < blending_value <= 1: No blending at all, take diffuse texture 2
	//	- otherwise, blend using linear interpolation
	MyVec2 Threshold;
};

//========================================================================================================
//
// Flat terrain is a flat-square quad having properties:
//	- Perpendicular to Oy-axis at y=0
//	- Aligned with Ox, Oz, axis
//
//========================================================================================================
class FlatTerrain : public OnPressListenee
{
public:
	FlatTerrain();
	~FlatTerrain();

	void init(
		Shader& shader,
		Texture& diffuseMap1,
		Texture& diffuseMap2,
		Texture& blendMap,
		const MyVec3& center,
		const MyVec2& size,
		FlatTerrainProperties& properties);
	void update(Timer& timer, bool isPressed, MyVec3& pressedPoint);
	void render(Camera& camera);

private:
	bool isValidPos(const MyVec3& pos);

private:
	BasicMesh m_mesh;

	Texture* m_diffuseMap1; 
	Texture* m_diffuseMap2; 
	Texture* m_blendMap;

	FlatTerrainProperties m_properties;

	MyVec3 m_center;
	MyVec2 m_size;
};