
#include "MySpriteSheet.h"

//======================================================================================================
//
// SpriteSheet Helpers
//
//======================================================================================================

Rect2D SpriteSheet::computingSrc(int spriteIndex)
{
	int nw = m_numSprites.x, nh = m_numSprites.y;

	int x = (m_orientation == HORIZONTAL ? spriteIndex % nw : spriteIndex / nh);
	int y = (m_orientation == HORIZONTAL ? spriteIndex / nw : spriteIndex % nh);

	return Rect2D(m_offsetSprites + m_sizeSprite * MyIVec2(x, y), m_sizeSprite);
}

//======================================================================================================
//
// SpriteSheet class
//
//======================================================================================================

SpriteSheet::SpriteSheet()
	: m_elapsedTime(0),
	m_currSprite(0),
	m_spf(1.0f)
{
}

SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::init(
	CFrmTexture* frmTexture,
	int fps,
	const MyIVec2& numSprites,
	const MyIVec2& sizeSprite,
	const MyIVec2& offsetSprites,
	Orientation orientation)
{
	m_numSprites = numSprites;
	m_sizeSprite = sizeSprite;
	m_offsetSprites = offsetSprites;
	m_orientation = orientation;

	if (fps > 0)
	{
		m_spf = 1.0f / (float)fps;
	}

	Texture::init(frmTexture);

	setSrcTex(computingSrc(m_currSprite));
}

void SpriteSheet::update(Timer& timer)
{
	m_elapsedTime += timer.getElapsedTime();
	if (m_elapsedTime >= m_spf)
	{
		int nw = m_numSprites.x, nh = m_numSprites.y;

		// Computing index of current sprite
		m_currSprite = (m_currSprite + 1 >= nw * nh ? 0 : m_currSprite + 1);

		// Computing source texture according to index of current sprite
		setSrcTex(computingSrc(m_currSprite));

		// Reset elapsed time
		m_elapsedTime -= m_spf;
	}
}

int SpriteSheet::getWidth()
{
	return m_sizeSprite.x;
}

int SpriteSheet::getHeight()
{
	return m_sizeSprite.y;
}