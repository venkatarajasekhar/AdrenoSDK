
#pragma once

#include "MyTexture.h"
#include "MyTimer.h"

class SpriteSheet : public Texture
{
public:
	enum Orientation
	{
		HORIZONTAL,
		VERTICAL,
	};

public:
	SpriteSheet();
	~SpriteSheet();

	void init(
		CFrmTexture* frmTexture,
		int fps,
		const MyIVec2& numSprites, 
		const MyIVec2& sizeSprite,
		const MyIVec2& offsetSprites = MyIVec2(0, 0),
		Orientation orientation = HORIZONTAL);

	void update(Timer& timer);

	int getWidth();
	int getHeight();

private:
	Rect2D computingSrc(int spriteIndex);

private:
	MyIVec2 m_numSprites;
	MyIVec2 m_sizeSprite;
	MyIVec2 m_offsetSprites;
	Orientation m_orientation;

	float m_elapsedTime;
	int m_currSprite;

	float m_spf;
};