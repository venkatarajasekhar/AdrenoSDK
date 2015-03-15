
#pragma once

#include "MySpriteBatch.h"

class SpriteSheet
{
public:
	enum Orientation
	{
		HORIZONTAL,
		VERTICAL,
	};

public:
	SpriteSheet();
	virtual ~SpriteSheet();

	virtual void init(
		Texture& texture, 
		int fps,
		const MyIVec2& numSprites, 
		const MyIVec2& sizeSprite,
		const MyIVec2& offsetSprites = MyIVec2(0, 0),
		Orientation orientation = HORIZONTAL);

	virtual void update(Timer& timer);

	virtual void render2D(SpriteBatch& spriteBatch, const MyVec2& pos, float rot = 0.0f, const MyVec2& scale = MyVec2(1.0f));
	virtual void render2D(SpriteBatch& spriteBatch, const Rect2D& dest, float rot = 0.0f);

protected:
	Texture* m_texture;

	MyIVec2 m_numSprites;
	MyIVec2 m_sizeSprite;
	MyIVec2 m_offsetSprites;
	Orientation m_orientation;

	float m_elapsedTime;
	int m_currSprite;

	float m_spf;
};