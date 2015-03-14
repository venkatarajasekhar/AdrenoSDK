
#include "MyMiniMap.h"

MiniMap::MiniMap()
	: m_background(nullptr),
	m_player(nullptr),
	m_widthWnd(0),
	m_heightWnd(0),
	m_status(SMALL)
{
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
}

void MiniMap::update(UserInput& userInput)
{

}

void MiniMap::render(SpriteBatch& spriteBatch, const MyVec3& playerPos)
{
	MyVec2 miniMapSize(m_background->getWidth(), m_background->getHeight());

	MyVec2 miniMapPos(m_widthWnd - PADDING_MINI_MAP - miniMapSize.x, PADDING_MINI_MAP);
	MyVec2 closeBtnPos(
		m_widthWnd - PADDING_MINI_MAP - PADDING_CLOSE_BUTTON - m_closeButton.getSize().x, 
		PADDING_MINI_MAP + PADDING_CLOSE_BUTTON);

	spriteBatch.renderTexture2D(*m_background, miniMapPos, 0, MyVec2(1));

	m_closeButton.setPos(closeBtnPos);
	m_closeButton.render(spriteBatch);
}