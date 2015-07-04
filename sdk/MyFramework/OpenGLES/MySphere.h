
#pragma once

#include "MyBasicMesh.h"

class Sphere : public BasicMesh
{
public:
	Sphere();
	~Sphere();

	void init(
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& scale,
		int sliceCount,
		int stackCount);

	void update(Timer& timer);

private:

};