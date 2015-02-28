
#pragma once

#include "MyCamera2D.h"
#include "MyBasicMesh.h"

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init(Shader& shader);
	void resize(int width, int height);
	void update();

	MyVec2 convertToWorld(const MyVec2& screenPos);

	void renderTexture2D(Texture& texture, const MyVec2& pos, float rot, const MyVec2& scale);

private:
	Camera2D m_camera2D;

	BasicMesh m_squad;

	int m_scrWidth, m_scrHeight;
};