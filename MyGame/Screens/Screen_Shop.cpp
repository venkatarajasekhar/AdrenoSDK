
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Shop.h"
#include <MyScreenManager.h>

//==============================================================================================================
//
// Some class declarations
//
//==============================================================================================================

class UIListItem_Dump : public UIListItem
{
public:
	UIListItem_Dump(UIList* list, Texture& texture)
		: UIListItem(list)
	{
		m_texture = &texture;
		UIWidget::init("", MyVec2(), MyVec2(m_texture->getWidth(), m_texture->getHeight()));
	}

	void render(SpriteBatch& spriteBatch)
	{
		spriteBatch.renderTexture2D(m_texture, m_bounding, nullptr, 0.0f, &m_list->getViewport());
	}

private:
	Texture* m_texture;
};

//==============================================================================================================
//
// ShopScreen class
//
//==============================================================================================================

ShopScreen::ShopScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_width(0),
	m_height(0)
{
}

ShopScreen::~ShopScreen()
{
}

void ShopScreen::init()
{
	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/shop.pak").c_str());

		m_textures[TEXTURE_SHOP_BACKGROUND].init(resource.GetTexture("shop_background"));
		m_textures[TEXTURE_SHOP_BTN_CLOSE].init(resource.GetTexture("shop_btn_close"));
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/dump_list.pak").c_str());

		m_textures[TEXTURE_DUMP_LIST_BACKGROUND].init(resource.GetTexture("dump_list_background"));
		m_textures[TEXTURE_DUMP_LIST_ITEM_1].init(resource.GetTexture("dump_list_item_1"));
		m_textures[TEXTURE_DUMP_LIST_ITEM_2].init(resource.GetTexture("dump_list_item_2"));
		m_textures[TEXTURE_DUMP_LIST_ITEM_3].init(resource.GetTexture("dump_list_item_3"));
		m_textures[TEXTURE_DUMP_LIST_ITEM_4].init(resource.GetTexture("dump_list_item_4"));
		m_textures[TEXTURE_DUMP_LIST_ITEM_5].init(resource.GetTexture("dump_list_item_5"));
		m_textures[TEXTURE_DUMP_LIST_ITEM_6].init(resource.GetTexture("dump_list_item_6"));
	}

	// Button widgets
	m_btns[BTN_CLOSE].init("shop_btn_close", MyVec2(), m_textures[TEXTURE_SHOP_BTN_CLOSE]);
	m_btns[BTN_CLOSE].addPressListener(this);

	// List widgets
	m_dumpList.init("dump_list", MyVec2(0, 0), m_textures[TEXTURE_DUMP_LIST_BACKGROUND]);
	m_dumpList.addItem(new UIListItem_Dump(&m_dumpList, m_textures[TEXTURE_DUMP_LIST_ITEM_1]));
	m_dumpList.addItem(new UIListItem_Dump(&m_dumpList, m_textures[TEXTURE_DUMP_LIST_ITEM_2]));
	m_dumpList.addItem(new UIListItem_Dump(&m_dumpList, m_textures[TEXTURE_DUMP_LIST_ITEM_3]));
	m_dumpList.addItem(new UIListItem_Dump(&m_dumpList, m_textures[TEXTURE_DUMP_LIST_ITEM_4]));
	m_dumpList.addItem(new UIListItem_Dump(&m_dumpList, m_textures[TEXTURE_DUMP_LIST_ITEM_5]));
	m_dumpList.addItem(new UIListItem_Dump(&m_dumpList, m_textures[TEXTURE_DUMP_LIST_ITEM_6]));
}

void ShopScreen::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void ShopScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}

	// List widgets
	m_dumpList.update(*globalUtilObjs->userInput);
}

void ShopScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	{
		MyVec2 pos = 0.5f * MyVec2(
			m_width - m_textures[TEXTURE_SHOP_BACKGROUND].getWidth(),
			m_height - m_textures[TEXTURE_SHOP_BACKGROUND].getHeight());

		globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_SHOP_BACKGROUND], pos);

		m_dumpList.setPos(pos + MyVec2(20));

		pos.x += m_textures[TEXTURE_SHOP_BACKGROUND].getWidth() - m_btns[BTN_CLOSE].getSize().x;
		m_btns[BTN_CLOSE].setPos(pos);
	}

	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}

	// List widgets
	m_dumpList.render(*globalUtilObjs->spriteBatch);
}

void ShopScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "shop_btn_close")
	{
		m_screenManager->activeScreen("PlayScreen");
	}
}