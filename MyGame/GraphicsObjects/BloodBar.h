
#pragma once

#include <MySpriteBatch.h>

class BloodBar
{
public:
	BloodBar();
	~BloodBar();

	void init(Texture& foreground, Texture& background);
	void render(SpriteBatch& spriteBatch, Camera& camera, const MyVec3& posW, float health);

	void setScale(const MyVec2& scale);

private:
	Texture* m_foreground;
	Texture* m_background;

	MyVec2 m_scale;
};