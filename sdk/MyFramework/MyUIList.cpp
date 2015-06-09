
#include "MyUIList.h"

#pragma region Constants

//=========================================================================================================================
//
// Constants
//
//=========================================================================================================================

static const float DEFAULT_LIST_ITEM_DIST = 10.0f;
static const float DEFAULT_LIST_PADDING = 5.0f;

#pragma endregion

#pragma region UIListItem class

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

#pragma endregion

#pragma region Event class

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

#pragma endregion

//=========================================================================================================================
//
// UIList class
//
//=========================================================================================================================

UIList::UIList()
	: m_background(nullptr),
	m_currPos(0.0f),
	m_maxCurrPos(0.0f),
	m_orientation(VERTICAL)
{
	setListPadding(DEFAULT_LIST_PADDING);
	setListItemDist(DEFAULT_LIST_ITEM_DIST);
}

UIList::~UIList()
{
	for (auto i = m_listItems.begin(); i != m_listItems.end(); ++i)
	{
		delete (*i);
	}
}

void UIList::init(const MyString& id, const MyVec2& pos, Texture& background, UIList::ORIENTATION orientation)
{
	m_background = &background;

	init(id, pos, m_background->getWidth(), m_background->getHeight(), orientation);
}

void UIList::init(const MyString& id, const MyVec2& pos, int width, int height, UIList::ORIENTATION orientation)
{
	m_orientation = orientation;

	UIWidget::init(id, pos, MyVec2(width, height));
}

void UIList::update(UserInput& userInput)
{
	// Update list
	{
		if (
			((m_orientation == VERTICAL) && (m_maxCurrPos - m_listItemDist - getSize().y > 0)) ||
			((m_orientation == HORIZONTAL) && (m_maxCurrPos - m_listItemDist - getSize().x > 0))
			)
		{
			MyVec2 delta, pos;
			if (userInput.pointer_Dragging(delta) && userInput.pointer_Holding(pos) && isInside(pos, m_bounding))
			{
				if (m_orientation == VERTICAL)
				{
					m_currPos -= delta.y;
					m_currPos = clamp(m_currPos, 0.0f, m_maxCurrPos - m_listItemDist - getSize().y);
				}
				else
				{
					m_currPos -= delta.x;
					m_currPos = clamp(m_currPos, 0.0f, m_maxCurrPos - m_listItemDist - getSize().x);
				}
			}
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
	float length(0.0f);
	MyVec2 listItemPos = getPos();
	bool flag(false);
	int count = 0;

	// Render background
	if (m_background != nullptr)
	{
		spriteBatch.renderTexture2D(m_background, m_bounding);
	}
	
	// Render list items
	for (auto i = m_listItems.begin(); i != m_listItems.end(); ++i)
	{
		UIListItem* listItem = (*i);

		if (m_orientation == VERTICAL)
		{
			length += listItem->getSize().y;
		}
		else
		{
			length += listItem->getSize().x;
		}
		
		// Begin rendering items
		if (length >= m_currPos)
		{
			if (!flag)
			{
				if (m_orientation == VERTICAL)
				{
					listItemPos.y += length - m_currPos - listItem->getSize().y;
				}
				else
				{
					listItemPos.x += length - m_currPos - listItem->getSize().x;
				}
				
				flag = true;
			}

			// Render item
			if (m_orientation == VERTICAL)
			{
				listItem->setPos(listItemPos + MyVec2(0.5f * (getSize().x - listItem->getSize().x), m_listItemDist * (count++)));
				listItem->render(spriteBatch);

				listItemPos += MyVec2(0, listItem->getSize().y);
			}
			else
			{
				listItem->setPos(listItemPos + MyVec2(m_listItemDist * (count++), 0.5f * (getSize().y - listItem->getSize().y)));
				listItem->render(spriteBatch);

				listItemPos += MyVec2(listItem->getSize().x, 0);
			}
			
			// End rendering items
			if (
				((m_orientation == VERTICAL) && (length >= m_currPos + getSize().y)) ||
				((m_orientation == HORIZONTAL) && (length >= m_currPos + getSize().x))
				)
			{
				break;
			}
		}

		length += m_listItemDist;
	}
}

void UIList::addItem(UIListItem* item)
{
	item->addPressListener(this);
	m_listItems.push_back(item);
	if (m_orientation == VERTICAL)
	{
		m_maxCurrPos += item->getSize().y + m_listItemDist;
	}
	else
	{
		m_maxCurrPos += item->getSize().x + m_listItemDist;
	}
}

Rect2D UIList::getViewport()
{
	Rect2D viewport;
	if (m_orientation == VERTICAL)
	{
		viewport.Pos = getPos() + MyVec2(0.0f, m_listPadding);
		viewport.Size = getSize() - MyVec2(0.0f, 2.0f * m_listPadding);
	}
	else
	{
		viewport.Pos = getPos() + MyVec2(m_listPadding, 0.0f);
		viewport.Size = getSize() - MyVec2(2.0f * m_listPadding, 0.0f);
	}
	
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

void UIList::setListPadding(float padding)
{
	m_listPadding = padding;
}

void UIList::setListItemDist(float dist)
{
	m_listItemDist = dist;
}