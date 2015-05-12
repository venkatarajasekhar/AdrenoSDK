
#pragma once

#include "MyUIWidget.h"

#pragma region UIListItem class

//===========================================================================================================
//
// UIListItem class
//
//===========================================================================================================

class UIList;

class UIListItem : public UIWidget
{
public:
	UIListItem(UIList* list);
	virtual ~UIListItem();

	virtual void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr) = 0;

	void select();
	void deselect();

protected:
	virtual void throwPressEvent(IOnPressListener::Data& data);

protected:
	UIList* m_list;
	bool m_selected;
};

#pragma endregion

#pragma region Event class

//===========================================================================================================
//
// Event class
//
//===========================================================================================================

class IOnPressListItemListener
{
public:
	struct Data
	{
		// ID of listenee
		MyString Id;

		// Selected list item
		UIListItem* ListItem;

		Data(const MyString& _id, UIListItem* _listItem)
			: Id(_id),
			ListItem(_listItem)
		{}
	};

public:
	virtual void OnPressListItem(const Data& data) = 0;
};

class OnPressListItemListenee
{
public:
	OnPressListItemListenee();
	virtual ~OnPressListItemListenee();

	virtual void addPressListItemListener(IOnPressListItemListener* listener);

protected:
	virtual void throwPressListItemEvent(IOnPressListItemListener::Data& data);

protected:
	std::vector<IOnPressListItemListener*> m_pressListItemListeners;
};

#pragma endregion

//===========================================================================================================
//
// UIList class
//
//===========================================================================================================

class UIList : public UIWidget, public IOnPressListener, public OnPressListItemListenee
{
public:
	enum ORIENTATION
	{
		VERTICAL,
		HORIZONTAL,
	};

public:
	UIList();
	~UIList();

	void init(const MyString& id, const MyVec2& pos, Texture& background, UIList::ORIENTATION orientation = UIList::VERTICAL);
	void update(UserInput& userInput);
	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr);

	void addItem(UIListItem* item);
	Rect2D getViewport();

	void OnPress(const IOnPressListener::Data& data);

private:
	std::vector<UIListItem*> m_listItems;
	Texture* m_background;

	float m_currPos;
	float m_maxCurrPos;

	ORIENTATION m_orientation;
};