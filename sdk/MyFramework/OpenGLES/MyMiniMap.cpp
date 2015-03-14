
#include "MyMiniMap.h"

MiniMap::MiniMap()
	: m_background(nullptr),
	m_player(nullptr),
	m_widthWnd(0),
	m_heightWnd(0)
{
	setStatus(SMALL);
}

MiniMap::~MiniMap()
{
}

void MiniMap::init(
	Texture& background,
	Texture& player,
	Texture& closeButton,
	const MyVec3& mapCenter,
	const MyVec2& mapSize)
{
	m_background = &background;
	m_player = &player;
	m_closeButton.init(MyVec2(), closeButton);
	m_mapCenter = mapCenter;
	m_mapSize = mapSize;
}

void MiniMap::resize(int width, int height)
{
	m_widthWnd = width;
	m_heightWnd = height;
	updateRect();
}

void MiniMap::update(UserInput& userInput)
{
	if (m_status == SMALL)
	{
		MyVec2 pos;
		if (userInput.pointer_Releasing(pos))
		{
			if (isInside(pos, m_miniMapRect))
			{
				setStatus(LARGE);
			}
		}
	}
	else
	{
		m_closeButton.update(userInput);
		if (m_closeButton.isPressing())
		{
			setStatus(SMALL);
		}
	}
}

void MiniMap::render(SpriteBatch& spriteBatch, const MyVec3& playerPos)
{
	// Render background
	spriteBatch.renderTexture2D(*m_background, m_miniMapRect);

	// Render player
	{
		MyVec3 upleftMap = 0.5f * MyVec3(m_mapSize.x, 0, m_mapSize.y) - m_mapCenter;
		MyVec3 pos = playerPos + upleftMap;

		if (isInside(MyVec2(pos.x, pos.z), Rect2D(MyVec2(0, 0), m_mapSize)))
		{
			MyVec2 relativePos(
				pos.x / m_mapSize.x * m_miniMapRect.Size.x,
				pos.z / m_mapSize.y * m_miniMapRect.Size.y);
			spriteBatch.renderTexture2D(*m_player, m_miniMapRect.Pos + relativePos - 0.5f * MyVec2(m_player->getWidth(), m_player->getHeight()));
		}
	}
	
	// Render closing button
	MyVec2 closeBtnPos(
		m_widthWnd - PADDING_MINI_MAP - PADDING_CLOSE_BUTTON - m_closeButton.getSize().x, 
		PADDING_MINI_MAP + PADDING_CLOSE_BUTTON);

	m_closeButton.setPos(closeBtnPos);
	m_closeButton.render(spriteBatch);
}

void MiniMap::setStatus(Status status)
{
	m_status = status;

	if (m_status == SMALL)
	{
		m_closeButton.setStatus(UIWidget::HIDDEN);
	}
	else
	{
		m_closeButton.setStatus(UIWidget::ACTIVE);
	}

	updateRect();
}

void MiniMap::updateRect()
{
	float scale = (m_status == SMALL ? 0.5f : 1.0f);

	if (m_background != nullptr)
	{
		m_miniMapRect.Size = scale * MyVec2(m_background->getWidth(), m_background->getHeight());
		m_miniMapRect.Pos = MyVec2(m_widthWnd - PADDING_MINI_MAP - m_miniMapRect.Size.x, PADDING_MINI_MAP);
	}
}