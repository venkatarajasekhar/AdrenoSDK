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
	{
		m_countTimeUsed += timer.getElapsedTime();
		execute(hero, timer.getElapsedTime());
	}
	else
	{
		if (m_isUsing)
		{
			m_countTimeUsed += timer.getElapsedTime();

			if (m_countTimeUsed <= m_timeUse)
			{
				execute(hero, timer.getElapsedTime());
			}

			if (m_countTimeUsed > m_timeUse + m_timeWait)
			{
				m_countTimeUsed = 0;
				m_isUsing = false;
			}
		}
	}
}

void HeroItem::execute(Hero* hero, float elapsedTime)
{

}


void HeroItem::useItem()
{
	m_isUsing = true;
	m_countTimeUsed = 0;
}

void HeroItem::sell()
{

}

int HeroItem::getPrice()
{
	return Price;
}

HeroItem::ITEM_TYPE HeroItem::getType()
{
	return m_type;
}

bool HeroItem::getIsUsing()
{
	return m_isUsing;
}

int HeroItem::getNTime()
{
	return m_nTime;
}

void HeroItem::accNTime(int n)
{
	m_nTime -= n;
}

int HeroItem::getCoolDownTime()
{
	return m_timeUse + m_timeWait - m_countTimeUsed;
}

//==================================================================================================================
//
// HeroItem_HealingPotion class
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

void HeroItem_HealingPotion::execute(Hero* hero, float elapsedTime)
{
	if (m_countTimeUsed >= m_count)
	{
		hero->getAudio(hero->AUDIO_HEALTH_SKILL)->play();
		hero->accHealth(20);
		m_count++;
	}
}


//==================================================================================================================
//
// HeroItem_CloakOfTheResistant class
//
//==================================================================================================================

HeroItem* HeroItem_AcolyteStaff::clone()
{
	return new HeroItem_AcolyteStaff(
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


void HeroItem_AcolyteStaff::execute(Hero* hero, float elapsedTime)
{
	float rot = hero->getRot().y;

	float dx = 5 * dSin(rot) * elapsedTime / m_timeUse;
	float dz = 5 * dCos(rot) * elapsedTime / m_timeUse;
	hero->accPos(MyVec3(dx, 0, dz));
	//hero->setTarget(hero->getPos());
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

void HeroItem_LifeWard::execute(Hero* hero, float elapsedTime)
{
	if (m_countTimeUsed >= 1.0f)
	{
		hero->accHealth(4);
		m_countTimeUsed--;
	}
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

void HeroItem_LightCalvaryHat::execute(Hero* hero, float elapsedTime)
{
	hero->setHealthPerAttack(3*hero->getDamage()/20);
}

//==================================================================================================================
//
// HeroItem_BootsOfSpeed class
//
//==================================================================================================================

HeroItem* HeroItem_BootsOfSpeed::clone()
{
	return new HeroItem_BootsOfSpeed(
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

void HeroItem_BootsOfSpeed::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed) hero->accSpeed(1.0f);
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_PhaseBoots class
//
//==================================================================================================================

HeroItem* HeroItem_PhaseBoots::clone()
{
	return new HeroItem_PhaseBoots(
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

void HeroItem_PhaseBoots::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accSpeed(1.0f);
		hero->accDamage(24);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_PowerTreads class
//
//==================================================================================================================

HeroItem* HeroItem_PowerTreads::clone()
{
	return new HeroItem_PowerTreads(
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

void HeroItem_PowerTreads::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accSpeed(1.0f);
		hero->accMaxHealth(200);
		hero->accHealth(200);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_ArcaneBoots class
//
//==================================================================================================================

HeroItem* HeroItem_ArcaneBoots::clone()
{
	return new HeroItem_ArcaneBoots(
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

void HeroItem_ArcaneBoots::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accSpeed(1.0f);
		hero->accMaxMana(200);
		hero->accMana(200);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_DemonEdge class
//
//==================================================================================================================

HeroItem* HeroItem_DemonEdge::clone()
{
	return new HeroItem_DemonEdge(
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

void HeroItem_DemonEdge::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accDamage(46);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_SacredRelic class
//
//==================================================================================================================

HeroItem* HeroItem_SacredRelic::clone()
{
	return new HeroItem_SacredRelic(
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

void HeroItem_SacredRelic::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accDamage(60);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_Quarterstaff class
//
//==================================================================================================================

HeroItem* HeroItem_Quarterstaff::clone()
{
	return new HeroItem_Quarterstaff(
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

void HeroItem_Quarterstaff::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accDamage(10);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_Broadsword class
//
//==================================================================================================================

HeroItem* HeroItem_Broadsword::clone()
{
	return new HeroItem_Broadsword(
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

void HeroItem_Broadsword::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accDamage(18);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_Claymore class
//
//==================================================================================================================

HeroItem* HeroItem_Claymore::clone()
{
	return new HeroItem_Claymore(
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

void HeroItem_Claymore::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accDamage(21);
	}
	m_isUsed = true;
}

//==================================================================================================================
//
// HeroItem_DivineRapier class
//
//==================================================================================================================

HeroItem* HeroItem_DivineRapier::clone()
{
	return new HeroItem_DivineRapier(
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

void HeroItem_DivineRapier::execute(Hero* hero, float elapsedTime)
{
	if (!m_isUsed)
	{
		hero->accDamage(300);
	}
	m_isUsed = true;
}