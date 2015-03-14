
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

	void renderTexture2D(Texture& texture, const MyVec2& pos, float rot = 0.0f, const MyVec2& scale = MyVec2(1.0f));
	void renderTexture2D(Texture& texture, const Rect2D& dest, float rot = 0.0f);

private:
	Camera2D m_camera2D;

	BasicMesh m_squad;

	int m_scrWidth, m_scrHeight;
};