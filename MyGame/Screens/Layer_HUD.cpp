
#include "Layer_HUD.h"
#include "HeroSkill.h"

#pragma region Constants

//==========================================================================================================
//
// Constants
//
//==========================================================================================================

static const bool   SHOW_FPS            = true;
static const MyVec2 LIST_ITEM_MARGIN    = MyVec2(20, 20);
static const float LIST_SKILL_HMARGIN = 20;

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
	UIListItem_ActiveItem(UIList* list, HeroItem* heroItem, Font& font)
		: UIListItem(list),
		m_font(&font)
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
			&listViewport,
			(!m_heroItem->getIsUsing() ? MyColor(1) : MyColor(0.5f)));

		if (m_heroItem->getIsUsing())
		{
			m_font->setScale(MyVec2(1.5f));

			MyString text = toString((int)m_heroItem->getCoolDownTime());
			MyVec2 textSize(m_font->getTextWidth(text), m_font->getTextHeight());
			MyVec2 offset = 0.5f * (getSize() - textSize);

			spriteBatch.renderText2D(*m_font, text, getPos() + offset, 0.0f, &listViewport);

			m_font->setScale(MyVec2(1.0f));
		}
	}

	HeroItem* getHeroItem()
	{
		return m_heroItem;
	}

private:
	HeroItem* m_heroItem;
	Font* m_font;
};

#pragma endregion

#pragma region UIListItem_Skill class

//==========================================================================================================
//
// UIListItem_Skill class
//
//==========================================================================================================

class UIListItem_Skill : public UIListItem
{
public:
	UIListItem_Skill(UIList* list, HeroSkill* heroSkill, Font& font)
		: UIListItem(list),
		m_font(&font)
	{
		m_heroSkill = heroSkill;

		Texture* texture = m_heroSkill->Avatar;

		UIWidget::init("", MyVec2(), MyVec2(texture->getWidth(), texture->getHeight()));
	}

	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr)
	{
		Rect2D listViewport = m_list->getViewport();

		Texture* texture = m_heroSkill->Avatar;

		spriteBatch.renderTexture2D(
			texture,
			m_bounding,
			nullptr,
			0.0f,
			&listViewport,
			(m_heroSkill->isUsable() ? MyColor(1) : MyColor(0.5f)));

		if (!m_heroSkill->isUsable())
		{
			m_font->setScale(MyVec2(1.5f));

			MyString text = toString((int)m_heroSkill->getCoolDownTimeRemain());
			MyVec2 textSize(m_font->getTextWidth(text), m_font->getTextHeight());
			MyVec2 offset = 0.5f * (getSize() - textSize);

			spriteBatch.renderText2D(*m_font, text, getPos() + offset, 0.0f, &listViewport);

			m_font->setScale(MyVec2(1.0f));
		}
	}

	HeroSkill* getHeroSkill()
	{
		return m_heroSkill;
	}

private:
	HeroSkill* m_heroSkill;
	Font* m_font;
};

#pragma endregion

//==========================================================================================================
//
// Layer_HUD class
//
//==========================================================================================================

Layer_HUD::Layer_HUD()
	: m_player(nullptr)
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
	}

	// Assets fonts
	m_fonts[FONT_CONSOLAS_12].init(resolveAssetsPath("Fonts/Consolas12.pak"));

	// List widgets
	m_list[LIST_ITEM].init("hud_list_item", MyVec2(), 400, 60, UIList::HORIZONTAL);
	m_list[LIST_ITEM].addPressListItemListener(this);

	{
		m_list[LIST_SKILL].init("hud_list_skill", MyVec2(0), 63, 276);
		m_list[LIST_SKILL].addPressListItemListener(this);

		auto& skillBag = m_player->getSkillBag();
		for (auto i = skillBag.begin(); i != skillBag.end(); ++i)
		{
			m_list[LIST_SKILL].addItem(new UIListItem_Skill(&m_list[LIST_SKILL], *i, m_fonts[FONT_CONSOLAS_12]));
		}
	}

	// Labels widgets
	m_labels[LABEL_FPS].init("", MyVec2(), m_fonts[FONT_CONSOLAS_12], "");
	if (!SHOW_FPS)
	{
		m_labels[LABEL_FPS].setStatus(UIWidget::HIDDEN);
	}
	
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
	// List widgets
	{
		MyVec2 pos = MyVec2(LIST_ITEM_MARGIN.x, height - LIST_ITEM_MARGIN.y - m_list[LIST_ITEM].getSize().y);
		m_list[LIST_ITEM].setPos(pos);
	}

	{
		float yOffset = 0.5f * (height - m_miniMap.getPos().y - m_miniMap.getSmallSize().y - m_list[LIST_SKILL].getSize().y);
		MyVec2 pos = MyVec2(
			width - LIST_SKILL_HMARGIN - m_list[LIST_SKILL].getSize().x, 
			m_miniMap.getPos().y + m_miniMap.getSmallSize().y + yOffset);
		m_list[LIST_SKILL].setPos(pos);
	}

	// Label widgets
	{
		MyVec2 pos = m_list[LIST_ITEM].getPos();
		pos.y -= 5 + m_labels[LABEL_FPS].getSize().y;
		m_labels[LABEL_FPS].setPos(pos);
	}

	// Other HUD-components
	m_miniMap.resize(width, height);
}

void Layer_HUD::update(Timer& timer, UserInput& userInput)
{
	// List widgets
	for (size_t i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].update(userInput);
	}

	// Label widgets
	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].update(userInput);
	}
	
	m_labels[LABEL_FPS].setText(toString(timer.getFPS()));

	// Other HUD-components
	m_miniMap.update(userInput);
	m_playerInfo.update(timer, userInput);
}

void Layer_HUD::render(SpriteBatch& spriteBatch, Layer_HUD::RenderBundle& bundle)
{
	// List widgets
	for (size_t i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].render(spriteBatch);
	}

	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].render(spriteBatch);
	}

	// Other HUD-components
	m_miniMap.render(spriteBatch, m_player->getPos());
	m_playerInfo.render(spriteBatch, *m_player);
}

// Event handling

void Layer_HUD::OnPress(const IOnPressListener::Data& data)
{
	IOnPressListener::Data hudData("hud", data.x, data.y);
	throwPressEvent(hudData);
}

void Layer_HUD::OnBuyItemItem(const IOnBuyItemListener::Data& data)
{
	if (m_player->getGold() >= data.BoughtItem->getPrice())
	{
		if (data.BoughtItem->getType() == data.BoughtItem->ACTIVE)
			m_list[LIST_ITEM].addItem(new UIListItem_ActiveItem(&m_list[LIST_ITEM], data.BoughtItem, m_fonts[FONT_CONSOLAS_12]));
		m_player->accGold(-data.BoughtItem->getPrice());
	}
}

void Layer_HUD::OnPressListItem(const IOnPressListItemListener::Data& data)
{
	if (data.Id == "hud_list_item")
	{
		UIListItem_ActiveItem* listItem = (UIListItem_ActiveItem*)data.ListItem;
		HeroItem* heroItem = listItem->getHeroItem();

		// Use 'heroItem' for hero 'm_player' here ...

		//smartLog("Pressed item: " + heroItem->Name);
		if ((heroItem->getNTime() != 0) && (!heroItem->getIsUsing()))
		{
			heroItem->accNTime(1);
			heroItem->useItem();
		}
	}
	else if (data.Id == "hud_list_skill")
	{
		UIListItem_Skill* listItem = (UIListItem_Skill*)data.ListItem;
		HeroSkill* heroSkill = listItem->getHeroSkill();

		heroSkill->use(m_player);
	}

	IOnPressListener::Data hudData("hud", 0, 0);
	throwPressEvent(hudData);
}