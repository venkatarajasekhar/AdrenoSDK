
#include "MySpriteSheet.h"

SpriteSheet::SpriteSheet()
	: m_texture(nullptr),
	m_elapsedTime(0),
	m_currSprite(0),
	m_spf(1.0f)
{
}

SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::init(
	Texture& texture,
	int fps,
	const MyIVec2& numSprites,
	const MyIVec2& sizeSprite,
	const MyIVec2& offsetSprites,
	Orientation orientation)
{
	m_texture = &texture;
	m_numSprites = numSprites;
	m_sizeSprite = sizeSprite;
	m_offsetSprites = offsetSprites;
	m_orientation = orientation;

	if (fps > 0)
	{
		m_spf = 1.0f / (float)fps;
	}
}

void SpriteSheet::update(Timer& timer)
{
	m_elapsedTime += timer.getElapsedTime();
	if (m_elapsedTime >= m_spf)
	{
		int nw = m_numSprites.x, nh = m_numSprites.y;

		m_currSprite = (m_currSprite + 1 >= nw * nh ? 0 : m_currSprite + 1);

		m_elapsedTime -= m_spf;
	}
}

void SpriteSheet::render2D(SpriteBatch& spriteBatch, const MyVec2& pos, float rot, const MyVec2& scale)
{
	Rect2D dest(pos, MyVec2(scale.x * m_sizeSprite.x, scale.y * m_sizeSprite.y));
	render2D(spriteBatch, dest, rot);
}

void SpriteSheet::render2D(SpriteBatch& spriteBatch, const Rect2D& dest, float rot)
{
	int nw = m_numSprites.x, nh = m_numSprites.y;

	int x = (m_orientation == HORIZONTAL ? m_currSprite % nw : m_currSprite / nh);
	int y = (m_orientation == HORIZONTAL ? m_currSprite / nw : m_currSprite % nh);

	Rect2D src(m_offsetSprites + m_sizeSprite * MyIVec2(x, y), m_sizeSprite);
	spriteBatch.renderTexture2D(*m_texture, dest, &src, rot);
}