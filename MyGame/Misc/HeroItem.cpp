
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

//==================================================================================================================
//
// HeroItem class
//
//==================================================================================================================

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

//==================================================================================================================
//
// HeroItem_ChainMail class
//
//==================================================================================================================

HeroItem* HeroItem_ChainMail::clone()
{
	return new HeroItem_ChainMail(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type);
}

//==================================================================================================================
//
// HeroItem_CloakOfTheResistant class
//
//==================================================================================================================

HeroItem* HeroItem_CloakOfTheResistant::clone()
{
	return new HeroItem_CloakOfTheResistant(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type);
}

//==================================================================================================================
//
// HeroItem_LifeWard class
//
//==================================================================================================================

HeroItem* HeroItem_LifeWard::clone()
{
	return new HeroItem_LifeWard(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type);
}

//==================================================================================================================
//
// HeroItem_BloodPouch class
//
//==================================================================================================================

HeroItem* HeroItem_BloodPouch::clone()
{
	return new HeroItem_BloodPouch(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type);
}

//==================================================================================================================
//
// HeroItem_StaffOfTheSathlenar class
//
//==================================================================================================================

HeroItem* HeroItem_StaffOfTheSathlenar::clone()
{
	return new HeroItem_StaffOfTheSathlenar(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type);
}

//==================================================================================================================
//
// HeroItem_LightCalvaryHat class
//
//==================================================================================================================

HeroItem* HeroItem_LightCalvaryHat::clone()
{
	return new HeroItem_LightCalvaryHat(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type);
}