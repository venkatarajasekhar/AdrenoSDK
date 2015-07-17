
#include "HeroSkill.h"
#include "Hero.h"

//==================================================================================================================
//
// HeroSkill class
//
//==================================================================================================================

HeroSkill::HeroSkill(
	const MyString& _name,
	int _damage,
	int _cost,
	float _coolDownTime,
	Texture* _avatar,
	Sphere* effect)
	: Name(_name),
	Damage(_damage),
	Cost(_cost),
	CoolDownTime(_coolDownTime),
	Avatar(_avatar),
	m_effect(effect),
	m_coolDownTimeRemain(-1.0f)
{
}

HeroSkill::~HeroSkill()
{
}

void HeroSkill::use(Hero* hero)
{
	if (isUsable() && (hero->getMana() >= Cost))
	{
		m_hero = hero;
		if (doUse(hero))
		{
			hero->accMana(-Cost);
			m_coolDownTimeRemain = CoolDownTime;
		}
	}
}

void HeroSkill::update(Timer& timer)
{
	m_coolDownTimeRemain -= timer.getElapsedTime();
	if ((!isUsable()) && (m_effect != nullptr))
	{
		doUpdate(timer);
	}
}

void HeroSkill::render(Camera& camera)
{
	if ((!isUsable()) && (m_effect != nullptr) && (CoolDownTime - m_coolDownTimeRemain <= 2.0f))
	{
		m_effect->setPos(m_hero->getPos());
		m_effect->render(camera);
	}
}

bool HeroSkill::isUsable()
{
	return (m_coolDownTimeRemain < 0);
}

bool HeroSkill::outOfMana()
{
	if (m_hero == nullptr)
	{
		return false;
	}
	else
	{
		return (m_hero->getMana() < Cost);
	}
}

float HeroSkill::getCoolDownTimeRemain()
{
	return m_coolDownTimeRemain;
}

//==================================================================================================================
//
// Skills for Dan Mei hero
//
//==================================================================================================================

// HeroSkill_BattleBorn
bool HeroSkill_BattleBorn::doUse(Hero* hero)
{
	auto target = hero->getTarget();

	if (target == nullptr)
	{
		return false;
	}

	if ((hero != target) &&
		(target->inUse()) &&
		(hero->getTeamType() != target->getTeamType()) &&
		(distance_optimized(hero->getPos(), target->getPos()) <= 10.0f))
	{
		target->accHealth(-300);
		hero->useSkill(1);
		return true;
	}

	return false;
}

void HeroSkill_BattleBorn::doUpdate(Timer& timer)
{
	MyVec3 rot = m_effect->getRot();
	rot.y += timer.getElapsedTime() * 50;
	rot.x += timer.getElapsedTime() * 30;
	m_effect->setRot(rot);
}

// HeroSkill_Bladefall
bool HeroSkill_Bladefall::doUse(Hero* hero)
{
	hero->useSkill(2);
	return true;
}

void HeroSkill_Bladefall::doUpdate(Timer& timer)
{
	MyVec3 rot = m_effect->getRot();
	rot.y += timer.getElapsedTime() * 50;
	m_effect->setRot(rot);
}

// HeroSkill_DecimationDay
bool HeroSkill_DecimationDay::doUse(Hero* hero)
{
	hero->useSkill(3);

	auto list = hero->getLivingEnts();

	for (auto i = list->begin(); i != list->end(); ++i)
	{
		if ((hero != (*i)) &&
			((*i)->inUse()) &&
			(hero->getTeamType() != (*i)->getTeamType()) &&
			(distance_optimized(hero->getPos(), (*i)->getPos()) <= 10.0f))
		{
			(*i)->accHealth(-200);
		}
	}

	return true;
}

void HeroSkill_DecimationDay::doUpdate(Timer& timer)
{
	MyVec3 rot = m_effect->getRot();
	rot.y = m_hero->getRot().y + 60;
	m_effect->setRot(rot);
}

// HeroSkill_JustDesserts
bool HeroSkill_JustDesserts::doUse(Hero* hero)
{
	return true;
}

void HeroSkill_JustDesserts::doUpdate(Timer& timer)
{

}