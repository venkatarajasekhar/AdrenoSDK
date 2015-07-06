
#pragma once

#include "MyBasicMesh.h"

class Cylinder : public BasicMesh
{
public:
	Cylinder();
	~Cylinder();

	void init(
		Shader& shader,
		Texture* diffuseMap,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		int sliceCount,
		int stackCount);

	void update(Timer& timer);
	void render(Camera& camera, Light* light = nullptr);

private:

};