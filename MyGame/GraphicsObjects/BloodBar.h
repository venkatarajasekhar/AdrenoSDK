
#pragma once

#include <MySpriteBatch.h>

class BloodBar
{
public:
	BloodBar();
	~BloodBar();

	void init(Texture& foreground, Texture& background);
	void render(SpriteBatch& spriteBatch, Camera& camera, const MyVec3& posW, float health);

private:
	Texture* m_foreground;
	Texture* m_background;
};