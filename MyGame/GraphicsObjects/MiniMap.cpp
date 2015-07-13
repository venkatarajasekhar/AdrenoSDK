
#include "MiniMap.h"

#pragma region Constants

//=====================================================================================================
//
// Constants
//
//=====================================================================================================

static const int   MINI_MAP_MARGIN     = 8;
static const int   CLOSE_BUTTON_MARGIN = 8;
static const float SIZE_WHEN_LARGE     = 1.0f;
static const float SIZE_WHEN_SMALL     = 0.35f;

#pragma endregion

#pragma region MiniMap helpers

//=====================================================================================================
//
// MiniMap helpers
//
//=====================================================================================================

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

	updateBounding();
}

void MiniMap::updateBounding()
{
	float scale = (m_status == SMALL ? SIZE_WHEN_SMALL : SIZE_WHEN_LARGE);

	if (m_background != nullptr)
	{
		m_bounding.Size = scale * MyVec2(m_background->getWidth(), m_background->getHeight());
		m_bounding.Pos = MyVec2(m_widthWnd - MINI_MAP_MARGIN - m_bounding.Size.x, MINI_MAP_MARGIN);
	}
}

#pragma endregion

//=====================================================================================================
//
// MiniMap class
//
//=====================================================================================================

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
	m_closeButton.init("hud_minimap_btn_close", MyVec2(), closeButton);
	m_mapCenter = mapCenter;
	m_mapSize = mapSize;

	m_closeButton.addPressListener(this);
}

void MiniMap::resize(int width, int height)
{
	m_widthWnd = width;
	m_heightWnd = height;
	updateBounding();
}

void MiniMap::update(UserInput& userInput)
{
	MyVec2 pos;
	if (userInput.pointer_Releasing(pos) && isInside(pos, m_bounding))
	{
		IOnPressListener::Data data("hud_minimap", (int)pos.x, (int)pos.y);
		throwPressEvent(data);

		if (m_status == SMALL) setStatus(LARGE);
		else m_closeButton.update(userInput);
	}
}

void MiniMap::render(SpriteBatch& spriteBatch, const MyVec3& playerPos)
{
	// Render background
	spriteBatch.renderTexture2D(m_background, m_bounding);

	// Render player
	{
		MyVec3 upleftMap = 0.5f * MyVec3(m_mapSize.x, 0, m_mapSize.y) - m_mapCenter;
		MyVec3 pos = playerPos + upleftMap;

		if (isInside(MyVec2(pos.x, pos.z), Rect2D(MyVec2(0, 0), m_mapSize)))
		{
			MyVec2 relativePos(
				pos.x / m_mapSize.x * m_bounding.Size.x,
				pos.z / m_mapSize.y * m_bounding.Size.y);
			spriteBatch.renderTexture2D(m_player, m_bounding.Pos + relativePos - 0.5f * MyVec2(m_player->getWidth(), m_player->getHeight()));
		}
	}
	
	// Render closing button
	MyVec2 closeBtnPos(
		m_widthWnd - MINI_MAP_MARGIN - CLOSE_BUTTON_MARGIN - m_closeButton.getSize().x,
		MINI_MAP_MARGIN + CLOSE_BUTTON_MARGIN);

	m_closeButton.setPos(closeBtnPos);
	m_closeButton.render(spriteBatch);
}

MyVec2 MiniMap::getSmallSize()
{
	return SIZE_WHEN_SMALL * MyVec2(m_background->getWidth(), m_background->getHeight());
}

MyVec2 MiniMap::getPos()
{
	return m_bounding.Pos;
}

void MiniMap::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "hud_minimap_btn_close")
	{
		setStatus(SMALL);
	}
}