
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

private:
	BasicMesh m_mesh;
	Texture* m_texture;

	MyVec2 m_billboardSize;
};