
#include "MyUIList.h"

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
			m_currPos = clamp(m_currPos, 0.0f, m_maxCurrPos - getSize().y);
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

void UIList::render(SpriteBatch& spriteBatch)
{
	float height(0.0f);
	MyVec2 listItemPos = getPos();
	bool flag(false);

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
			listItem->setPos(listItemPos);
			listItem->render(spriteBatch);

			listItemPos += MyVec2(0, listItem->getSize().y);

			// End rendering items
			if (height >= m_currPos + getSize().y)
			{
				break;
			}
		}
	}
}

void UIList::addItem(UIListItem* item)
{
	m_listItems.push_back(item);
	m_maxCurrPos += item->getSize().y;
}