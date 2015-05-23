
#include "Layer_HUD.h"

#pragma region Constants

//==========================================================================================================
//
// Constants
//
//==========================================================================================================

static const bool   SHOW_FPS            = true;
static const MyVec2 BTN_FIGHTING_MARGIN = MyVec2(30, 20);
static const MyVec2 LIST_ITEM_MARGIN    = MyVec2(100, 20);

#pragma endregion

#pragma region UIListItem_ActiveItem class

//==========================================================================================================
//
// UIListItem_ActiveItem class
//
//==========================================================================================================

class UIListItem_ActiveItem : public UIListItem
{
public:
	UIListItem_ActiveItem(UIList* list, HeroItem* heroItem)
		: UIListItem(list)
	{
		m_heroItem = heroItem;

		Texture* texture = m_heroItem->Avatar;

		UIWidget::init("", MyVec2(), MyVec2(texture->getWidth(), texture->getHeight()));
	}

	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr)
	{
		Rect2D listViewport = m_list->getViewport();

		Texture* texture = m_heroItem->Avatar;

		spriteBatch.renderTexture2D(
			texture,
			m_bounding,
			nullptr,
			0.0f,
			&listViewport);
	}

	HeroItem* getHeroItem()
	{
		return m_heroItem;
	}

private:
	HeroItem* m_heroItem;
};

#pragma endregion

//==========================================================================================================
//
// Layer_HUD class
//
//==========================================================================================================

Layer_HUD::Layer_HUD()
	: m_fps(0.0f),
	m_player(nullptr)
{
}

Layer_HUD::~Layer_HUD()
{
}

// Core functions

void Layer_HUD::init(Layer_HUD::InitBundle& bundle)
{
	m_player = bundle.Player;

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

	// List widgets
	m_list[LIST_ITEM].init("hud_list_item", MyVec2(), 400, 60, UIList::HORIZONTAL);
	m_list[LIST_ITEM].addPressListItemListener(this);

	// Other HUD-components
	m_miniMap.init(
		m_textures[TEXTURE_MINIMAP_BACKGROUND],
		m_textures[TEXTURE_MINIMAP_PLAYER],
		m_textures[TEXTURE_MINIMAP_BTN_CLOSE],
		bundle.MapCenter,
		bundle.MapSize);
	m_miniMap.addPressListener(this);

	m_playerInfo.init(m_fonts[FONT_CONSOLAS_12]);
}

void Layer_HUD::resize(int width, int height)
{
	// Button widgets
	{
		MyVec2 pos = MyVec2(width, height) - m_btns[BTN_FIGHTING].getSize() - BTN_FIGHTING_MARGIN;
		m_btns[BTN_FIGHTING].setPos(pos);
	}

	// List widgets
	{
		MyVec2 pos = MyVec2(LIST_ITEM_MARGIN.x, height - LIST_ITEM_MARGIN.y - m_list[LIST_ITEM].getSize().y);
		m_list[LIST_ITEM].setPos(pos);
	}

	// Other HUD-components
	m_miniMap.resize(width, height);
}

void Layer_HUD::update(Timer& timer, UserInput& userInput)
{
	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(userInput);
	}

	// List widgets
	for (size_t i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].update(userInput);
	}
	
	// Other HUD-components
	m_miniMap.update(userInput);
	m_playerInfo.update(timer, userInput);

	m_fps = timer.getFPS();
}

void Layer_HUD::render(SpriteBatch& spriteBatch, Layer_HUD::RenderBundle& bundle)
{
	int sWidth, sHeight;
	getWindowDimension(sWidth, sHeight);

	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(spriteBatch);
	}

	// List widgets
	for (size_t i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].render(spriteBatch);
	}

	// Other HUD-components
	m_miniMap.render(spriteBatch, m_player->getPos());
	m_playerInfo.render(spriteBatch, *m_player);

	if (SHOW_FPS)
	{
		MyVec2 pos(10, sHeight - 10 - m_fonts[FONT_CONSOLAS_12].getTextHeight());
		spriteBatch.renderText2D(m_fonts[FONT_CONSOLAS_12], toString(m_fps), pos);
	}
}

// Event handling

void Layer_HUD::OnPress(const IOnPressListener::Data& data)
{
	IOnPressListener::Data hudData("hud", data.x, data.y);
	throwPressEvent(hudData);
}

void Layer_HUD::OnBuyItemItem(const IOnBuyItemListener::Data& data)
{
	if (data.BoughtItem->getType()==data.BoughtItem->ACTIVE)
		m_list[LIST_ITEM].addItem(new UIListItem_ActiveItem(&m_list[LIST_ITEM], data.BoughtItem));
}

void Layer_HUD::OnPressListItem(const IOnPressListItemListener::Data& data)
{
	if (data.Id == "hud_list_item")
	{
		UIListItem_ActiveItem* listItem = (UIListItem_ActiveItem*)data.ListItem;
		HeroItem* heroItem = listItem->getHeroItem();

		// Use 'heroItem' for hero 'm_player' here ...

		//smartLog("Pressed item: " + heroItem->Name);
		heroItem->useItem();

		IOnPressListener::Data hudData("hud", 0, 0);
		throwPressEvent(hudData);
	}
}