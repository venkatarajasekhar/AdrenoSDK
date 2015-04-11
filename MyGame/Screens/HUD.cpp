
#include "HUD.h"
#include "Hero_Controlled.h"
#include "Global.h"

extern Hero_Controlled* g_heroPlayer;

//==========================================================================================================
//
// Constants
//
//==========================================================================================================

static const MyVec2 BTN_FIGHTING_MARGIN = MyVec2(30, 20);

//==========================================================================================================
//
// HUD class
//
//==========================================================================================================

HUD::HUD()
{
}

HUD::~HUD()
{
}

#pragma region Core functions

void HUD::init(const MyVec3& mapCenter, const MyVec2& mapSize, IOnPressListener* listener)
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

	// Elements of HUD
	m_miniMap.init(
		m_textures[TEXTURE_MINIMAP_BACKGROUND],
		m_textures[TEXTURE_MINIMAP_PLAYER],
		m_textures[TEXTURE_MINIMAP_BTN_CLOSE],
		mapCenter,
		mapSize);
	m_miniMap.addPressListener(this);

	m_btn_fighting.init("btn_hud_fighting", MyVec2(), m_textures[TEXTURE_BTN_FIGHTING]);
	m_btn_fighting.addPressListener(this);
	m_btn_fighting.addPressListener(listener);
}

void HUD::resize(int width, int height)
{
	m_miniMap.resize(width, height);

	{
		MyVec2 pos = MyVec2(width, height) - m_btn_fighting.getSize() - BTN_FIGHTING_MARGIN;
		m_btn_fighting.setPos(pos);
	}
}

void HUD::update(Timer& timer, UserInput& userInput)
{
	m_miniMap.update(userInput);
	m_btn_fighting.update(userInput);
}

void HUD::render(SpriteBatch& spriteBatch)
{
	/*if (g_heroPlayer != nullptr)
	{
		m_miniMap.render(spriteBatch, g_heroPlayer->getPos());
	}
	else
	{
		m_miniMap.render(spriteBatch, MyVec3());
	}*/
	
	m_miniMap.render(spriteBatch, g_livingEntityManager.getLivingEntityById(0)->getInstance()->Position);

	m_btn_fighting.render(spriteBatch);
}

#pragma endregion

#pragma region Event handling

void HUD::OnPress(const IOnPressListener::Data& data)
{
	IOnPressListener::Data hudData("hud", data.x, data.y);
	throwPressEvent(hudData);
}

#pragma endregion