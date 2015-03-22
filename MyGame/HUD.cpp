
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
		resource.LoadFromFile(resolveAssetsPath("Textures/hud.pak").c_str());

		m_textures[TEXTURE_MINIMAP_BACKGROUND].init(resource.GetTexture("minimap_background"));
		m_textures[TEXTURE_MINIMAP_BTN_CLOSE].init(resource.GetTexture("minimap_btn_close"));
		m_textures[TEXTURE_MINIMAP_PLAYER].init(resource.GetTexture("minimap_player"));

		m_textures[TEXTURE_BTN_FIGHTING].init(resource.GetTexture("btn_fighting"));
	}

	// HUD
	m_miniMap.init(
		m_textures[TEXTURE_MINIMAP_BACKGROUND],
		m_textures[TEXTURE_MINIMAP_PLAYER],
		m_textures[TEXTURE_MINIMAP_BTN_CLOSE],
		MyVec3(0),
		MyVec2(100));

	m_btn_fighting.init("btn_hud_fighting", MyVec2(), m_textures[TEXTURE_BTN_FIGHTING]);
	m_btn_fighting.addListener(this);
}

void HUD::resize(int width, int height)
{
	m_miniMap.resize(width, height);

	{
		MyVec2 offset(30, 20);
		MyVec2 pos = MyVec2(width, height) - m_btn_fighting.getSize() - offset;
		m_btn_fighting.setPos(pos);
	}
}

void HUD::update(Timer& timer, UserInput& userInput, bool& isClicked)
{
	m_miniMap.update(userInput, isClicked);
	m_btn_fighting.update(userInput);

	//isClicked |= m_btn_fighting.isPressing();
}

void HUD::render(SpriteBatch& spriteBatch)
{
	m_miniMap.render(spriteBatch, PositionPlayer);
	m_btn_fighting.render(spriteBatch);
}

void HUD::OnPress(const IOnPressListener::Data& data)
{

}