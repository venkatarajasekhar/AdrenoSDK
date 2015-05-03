
#include "Layer_HUD.h"

//==========================================================================================================
//
// Constants
//
//==========================================================================================================

static const bool   SHOW_FPS            = true;
static const MyVec2 BTN_FIGHTING_MARGIN = MyVec2(30, 20);

//==========================================================================================================
//
// Layer_HUD class
//
//==========================================================================================================

Layer_HUD::Layer_HUD()
	: m_fps(0.0f)
{
}

Layer_HUD::~Layer_HUD()
{
}

// Core functions

void Layer_HUD::init(Layer_HUD::InitBundle& bundle)
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

	// Assets fonts
	m_fonts[FONT_CONSOLAS_12].init(resolveAssetsPath("Fonts/Consolas12.pak"));

	// Button widgets
	m_btns[BTN_FIGHTING].init("hud_btn_fighting", MyVec2(), m_textures[TEXTURE_BTN_FIGHTING]);
	m_btns[BTN_FIGHTING].addPressListener(this);
	m_btns[BTN_FIGHTING].addPressListener(bundle.OpenShopListener);

	// Other HUD-components
	m_miniMap.init(
		m_textures[TEXTURE_MINIMAP_BACKGROUND],
		m_textures[TEXTURE_MINIMAP_PLAYER],
		m_textures[TEXTURE_MINIMAP_BTN_CLOSE],
		bundle.MapCenter,
		bundle.MapSize);
	m_miniMap.addPressListener(this);
}

void Layer_HUD::resize(int width, int height)
{
	// Button widgets
	{
		MyVec2 pos = MyVec2(width, height) - m_btns[BTN_FIGHTING].getSize() - BTN_FIGHTING_MARGIN;
		m_btns[BTN_FIGHTING].setPos(pos);
	}

	// Other HUD-components
	m_miniMap.resize(width, height);
}

void Layer_HUD::update(Timer& timer, UserInput& userInput)
{
	// Button widgets
	m_btns[BTN_FIGHTING].update(userInput);

	// Other HUD-components
	m_miniMap.update(userInput);
	m_fps = timer.getFPS();
}

void Layer_HUD::render(SpriteBatch& spriteBatch, Layer_HUD::RenderBundle& bundle)
{
	// Button widgets
	m_btns[BTN_FIGHTING].render(spriteBatch);

	// Other HUD-components
	m_miniMap.render(spriteBatch, bundle.PlayerPos);

	if (SHOW_FPS)
	{
		spriteBatch.renderText2D(
			m_fonts[FONT_CONSOLAS_12],
			toString(m_fps),
			MyVec2(10, 10));
	}
}

// Event handling

void Layer_HUD::OnPress(const IOnPressListener::Data& data)
{
	IOnPressListener::Data hudData("hud", data.x, data.y);
	throwPressEvent(hudData);
}