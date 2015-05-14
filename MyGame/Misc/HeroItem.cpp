
#include "HeroItem.h"

/*
Item::Item()
{

}

Item::~Item()
{

}


void Item::init()
{
	m_countTimeUsed = 0;
	m_isUsing = false;
	m_isDropping = false;
}

void Item::update(Timer& timer)
{
	if (!m_isDropping)
	{
		if (m_type == PASSIVE)
			use();
		else
		{
			if (m_isUsing) m_countTimeUsed += timer.getElapsedTime();
			if ((m_isUsing) && (m_countTimeUsed <= m_timeUse))
			{
				use();
			}
			if (m_countTimeUsed < m_timeUse + m_timeWait)
			{
				m_countTimeUsed = 0;
				m_isUsing = false;
			}
		}
	}
}

void Item::use()
{

}

void Item::drop()
{
	m_isDropping = true;
}

void Item::pick()
{
	m_isDropping = false;
}

void Item::sell()
{

}

int Item::getPrice()
{
	return m_price;
}
/**/

HeroItem::HeroItem(const MyString& _name,
	const MyString& _desc,
	int _price,
	const MyString& _benefit,
	Texture& _avatar,
	ITEM_TYPE _type)
	: Name(_name),
	Desc(_desc),
	Price(_price),
	Benefit(_benefit),
	Avatar(&_avatar),
	m_type(_type)
{
}

HeroItem::~HeroItem()
{
}