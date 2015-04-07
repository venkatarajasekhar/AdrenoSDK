
#pragma once

#include "MyBasicMesh.h"

class Billboard
{
public:
	Billboard();
	~Billboard();

	void init(Texture* texture, Shader& shader, const MyVec3& pos, const MyVec2& size, float rot);
	void update(Timer& timer);
	void render(Camera& camera);

	void setPos(const MyVec3& pos);

private:
	Texture* m_texture;
	MyVec3 m_pos;
	MyVec2 m_billboardSize;

private:
	static BasicMesh m_mesh;
	static bool m_initedStatic;
};