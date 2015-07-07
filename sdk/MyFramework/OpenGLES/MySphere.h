
#pragma once

#include "MyBasicMesh.h"

class Sphere : public BasicMesh
{
public:
	Sphere();
	~Sphere();

	void init(
		Shader& shader,
		Texture* diffuseMap,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		int sliceCount,
		int stackCount,
		MyVec3 distortMod = MyVec3(0));

	void update(Timer& timer);
	void render(Camera& camera, Light* light = nullptr);

private:
	MyVec3 m_distortMod;
};