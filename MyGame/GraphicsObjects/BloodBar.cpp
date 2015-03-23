
#include "BloodBar.h"

BloodBar::BloodBar()
	: m_foreground(nullptr),
	m_background(nullptr)
{
}

BloodBar::~BloodBar()
{
}

void BloodBar::init(Texture& foreground, Texture& background)
{
	m_foreground = &foreground;
	m_background = &background;
}

void BloodBar::render(SpriteBatch& spriteBatch, Camera& camera, const MyVec3& posW, float health)
{
	int w, h;
	getWindowDimension(w, h);

	// Screen position
	MyVec2 posS = project(posW, w, h, camera.getView(), camera.getProj());

	spriteBatch.renderTexture2D(m_background, posS, 0, MyVec2(1));
	spriteBatch.renderTexture2D(m_foreground, posS, 0, MyVec2(health, 1));
}