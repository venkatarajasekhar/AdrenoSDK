
#pragma once

#include "MyCamera2D.h"
#include "MyBasicMesh.h"
#include "MyFont.h"

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init(Shader& shader, Camera2D& camera);
	void resize(int width, int height);
	void update();

	MyVec2 convertToWorld(const MyVec2& screenPos);

	void renderTexture2D(Texture* texture, const MyVec2& pos, float rot = 0.0f, const MyVec2& scale = MyVec2(1.0f), MyColor color = MyColor(1, 1, 1));
	void renderTexture2D(Texture* texture, const Rect2D& dest, const Rect2D* src = nullptr, float rot = 0.0f, MyColor color = MyColor(1, 1, 1));

	void renderText2D(Font& font, const MyString& text, const MyVec2& pos, float rot = 0.0f, MyColor color = MyColor(1, 1, 1));

private:
	Camera2D* m_camera2D;

	BasicMesh m_squad;

	int m_scrWidth, m_scrHeight;
};