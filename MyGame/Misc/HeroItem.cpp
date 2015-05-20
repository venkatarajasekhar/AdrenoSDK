#include "Hero.h"
#include "HeroItem.h"

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
	ITEM_TYPE _type,
	float _timeUse,
	float _timeWait,
	int _nTime)
	: Name(_name),
	Desc(_desc),
	Price(_price),
	Benefit(_benefit),
	Avatar(&_avatar),
	m_type(_type),
	m_timeUse(_timeUse),
	m_timeWait(_timeWait),
	m_nTime(_nTime)
{
	m_countTimeUsed = 0;
	m_isUsing = false;
}

HeroItem::~HeroItem()
{
}

void HeroItem::update(Timer& timer, Hero* hero)
{
	if (m_type == PASSIVE)
		execute(hero);
	else
	{
		if (m_isUsing)
		{
			m_countTimeUsed += timer.getElapsedTime();

			if (m_countTimeUsed <= m_timeUse)
			{
				execute(hero);
			}

			if (m_countTimeUsed > m_timeUse + m_timeWait)
			{
				m_countTimeUsed = 0;
				m_isUsing = false;
			}
		}
	}
}

void HeroItem::execute(Hero* hero)
{

}

void HeroItem::useItem()
{
	m_isUsing = true;
	//m_countTimeUsed = 0;
}

void HeroItem::sell()
{

}

int HeroItem::getPrice()
{
	return Price;
}

//==================================================================================================================
//
// HeroItem_ChainMail class
//
//==================================================================================================================

HeroItem* HeroItem_HealingPotion::clone()
{
	return new HeroItem_HealingPotion(
		this->Name,
		this->Desc,
		this->Price,
		this->Benefit,
		*(this->Avatar),
		this->m_type,
		this->m_timeUse,
		this->m_timeWait,
		this->m_nTime);
}

void HeroItem_HealingPotion::execute(Hero* hero)
{
	static float exp = 1.0f;
	if (m_countTimeUsed >= exp)
	{
		hero->accHealth(20);
		exp++;
	}
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
		this->m_type,
		this->m_timeUse,
		this->m_timeWait,
		this->m_nTime);
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
		this->m_type,
		this->m_timeUse,
		this->m_timeWait,
		this->m_nTime);
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
		this->m_type,
		this->m_timeUse,
		this->m_timeWait,
		this->m_nTime);
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
		this->m_type,
		this->m_timeUse,
		this->m_timeWait,
		this->m_nTime);
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
		this->m_type,
		this->m_timeUse,
		this->m_timeWait,
		this->m_nTime);
}
