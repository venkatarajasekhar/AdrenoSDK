
#pragma once

#include "MyBasicMesh.h"

class Box : public BasicMesh
{
public:
	Box();
	~Box();

	void init(
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale);

	void update(Timer& timer);

private:

};