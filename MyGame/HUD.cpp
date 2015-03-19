
#include "HUD.h"

extern MyVec3 PositionPlayer;

HUD::HUD()
{
}

HUD::~HUD()
{
}

void HUD::init()
{
	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/minimap.pak").c_str());

		m_textures[TEXTURE_MINIMAP_BACKGROUND].init(resource.GetTexture("minimap"));
		m_textures[TEXTURE_MINIMAP_CLOSE_BTN].init(resource.GetTexture("close_button"));
		m_textures[TEXTURE_MINIMAP_PLAYER].init(resource.GetTexture("player"));
	}

	m_miniMap.init(
		m_textures[TEXTURE_MINIMAP_BACKGROUND],
		m_textures[TEXTURE_MINIMAP_PLAYER],
		m_textures[TEXTURE_MINIMAP_CLOSE_BTN],
		MyVec3(0),
		MyVec2(100));
}

void HUD::resize(int width, int height)
{
	m_miniMap.resize(width, height);
}

void HUD::update(Timer& timer, UserInput& userInput, bool& isClicked)
{
	m_miniMap.update(userInput, isClicked);
}

void HUD::render(SpriteBatch& spriteBatch)
{
	m_miniMap.render(spriteBatch, PositionPlayer);
}