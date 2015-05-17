
#pragma once

#include "MyBasicMesh.h"

class Quad3D
{
public:
	Quad3D();
	virtual ~Quad3D();

	virtual void init(Texture* diffuseMap, Shader& shader, const MyVec3& pos, const MyVec3& rot, const MyVec2& size);
	virtual void update(Timer& timer);
	virtual void render(Camera& camera);

	// Setter

	void setPos(const MyVec3& pos);
	void setRot(const MyVec3& rot);
	void setSize(const MyVec2& size);

	void setDiffuseMap(Texture* diffuseMap);

protected:
	static BasicMesh m_mesh;
	static bool m_initedStatic;
};