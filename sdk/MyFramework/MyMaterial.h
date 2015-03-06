
#pragma once

#include "MyUtils.h"

struct Material
{
	Material()
		: Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
		Specular(1.0f, 1.0f, 1.0f, 1.0f),
		Ambient(0.2f, 0.2f, 0.2f),
		Shininess(8.0f)
	{}

	MyVec4 Diffuse;
	MyVec4 Specular;
	MyVec3 Ambient;

	float Shininess;
};