
#include "MyUIList.h"

//=========================================================================================================================
//
// Constants
//
//=========================================================================================================================

static const float LIST_ITEM_MARGIN = 10.0f;
static const float LIST_PADDING = 5.0f;

//=========================================================================================================================
//
// UIListItem class
//
//=========================================================================================================================

UIListItem::UIListItem(UIList* list)
	: m_list(list),
	m_selected(false)
{}

UIListItem::~UIListItem()
{}

void UIListItem::select()
{ 
	m_selected = true; 
}

void UIListItem::deselect()
{ 
	m_selected = false; 
}

void UIListItem::throwPressEvent(IOnPressListener::Data& data)
{
	data.Tag = this;
	OnPressListenee::throwPressEvent(data);
}

//=========================================================================================================================
//
// Event class
//
//=========================================================================================================================

OnPressListItemListenee::OnPressListItemListenee()
{}

OnPressListItemListenee::~OnPressListItemListenee()
{}

void OnPressListItemListenee::addPressListItemListener(IOnPressListItemListener* listener)
{
	if (listener == nullptr) return;

	bool existed(false);
	for (auto i = m_pressListItemListeners.begin(); i != m_pressListItemListeners.end(); ++i)
	{
		if ((*i) == listener)
		{
			existed = true;
			break;
		}
	}
	if (!existed)
	{
		m_pressListItemListeners.push_back(listener);
	}
}

void OnPressListItemListenee::throwPressListItemEvent(IOnPressListItemListener::Data& data)
{
	for (auto i = m_pressListItemListeners.begin(); i != m_pressListItemListeners.end(); ++i)
	{
		if ((*i) != nullptr)
		{
			(*i)->OnPressListItem(data);
		}
	}
}

//=========================================================================================================================
//
// UIList class
//
//=========================================================================================================================

UIList::UIList()
	: m_background(nullptr),
	m_currPos(0.0f),
	m_maxCurrPos(0.0f)
{
}

UIList::~UIList()
{
	for (auto i = m_listItems.begin(); i != m_listItems.end(); ++i)
	{
		delete (*i);
	}
}

void UIList::init(const MyString& id, const MyVec2& pos, Texture& background)
{
	m_background = &background;
	UIWidget::init(id, pos, MyVec2(m_background->getWidth(), m_background->getHeight()));
}

void UIList::update(UserInput& userInput)
{
	// Update list
	{
		MyVec2 delta, pos;
		if (userInput.pointer_Dragging(delta) && userInput.pointer_Holding(pos) && isInside(pos, m_bounding))
		{
			m_currPos -= delta.y;
			m_currPos = clamp(m_currPos, 0.0f, m_maxCurrPos - LIST_ITEM_MARGIN - getSize().y);
		}
	}

	// Update items
	for (auto i = m_listItems.begin(); i != m_listItems.end(); ++i)
	{
		UIListItem* listItem = (*i);
		listItem->update(userInput);
	}

	UIWidget::update(userInput);
}

void UIList::render(SpriteBatch& spriteBatch, const Rect2D* viewport)
{
	float height(0.0f);
	MyVec2 listItemPos = getPos();
	bool flag(false);
	int count = 0;

	// Render background
	spriteBatch.renderTexture2D(m_background, m_bounding);

	// Render list items
	for (auto i = m_listItems.begin(); i != m_listItems.end(); ++i)
	{
		UIListItem* listItem = (*i);

		height += listItem->getSize().y;

		// Begin rendering items
		if (height >= m_currPos)
		{
			if (!flag)
			{
				listItemPos.y += height - m_currPos - listItem->getSize().y;

				flag = true;
			}

			// Render item
			listItem->setPos(listItemPos + MyVec2(0.5f * (getSize().x - listItem->getSize().x), LIST_ITEM_MARGIN * (count++)));
			listItem->render(spriteBatch);

			listItemPos += MyVec2(0, listItem->getSize().y);

			// End rendering items
			if (height >= m_currPos + getSize().y)
			{
				break;
			}
		}

		height += LIST_ITEM_MARGIN;
	}
}

void UIList::addItem(UIListItem* item)
{
	item->addPressListener(this);
	m_listItems.push_back(item);
	m_maxCurrPos += item->getSize().y + LIST_ITEM_MARGIN;
}

Rect2D UIList::getViewport()
{
	Rect2D viewport;
	viewport.Pos = getPos() + MyVec2(0.0f, LIST_PADDING);
	viewport.Size = getSize() - MyVec2(0.0f, 2.0f * LIST_PADDING);

	return viewport;
}

void UIList::OnPress(const IOnPressListener::Data& data)
{
	if (data.Tag != nullptr)
	{
		UIListItem* item = (UIListItem*)data.Tag;

		for (auto i = m_listItems.begin(); i != m_listItems.end(); ++i)
		{
			(*i)->deselect();
		}

		item->select();

		// Throw press list item event
		IOnPressListItemListener::Data data(m_id, item);
		throwPressListItemEvent(data);
	}
}