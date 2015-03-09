
#pragma once

#include "MyUtils.h"

struct Light
{
	Light()
		: PosOrDir(0, -1, -1, 0)
	{}

	// w == 0.0f: Directional light
	// w == 1.0f: Point light
	MyVec4 PosOrDir;
};