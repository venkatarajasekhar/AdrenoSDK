
#pragma once

#include "MyUIWidget.h"

//===========================================================================================================
//
// UIListItem class
//
//===========================================================================================================

class UIList;

class UIListItem : public UIWidget
{
public:
	UIListItem(UIList* list)
		: m_list(list)
	{}
	virtual ~UIListItem(){}

	virtual void render(SpriteBatch& spriteBatch) = 0;

protected:
	UIList* m_list;
};

//===========================================================================================================
//
// UIList class
//
//===========================================================================================================

class UIList : public UIWidget
{
public:
	UIList();
	~UIList();

	void init(const MyString& id, const MyVec2& pos, Texture& background);
	void update(UserInput& userInput);
	void render(SpriteBatch& spriteBatch);

	void addItem(UIListItem* item);

	Rect2D getViewport();

private:
	std::vector<UIListItem*> m_listItems;
	Texture* m_background;

	float m_currPos;
	float m_maxCurrPos;
};