
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
	Sphere* effect,
	Audio* audio,
	NotifyPool* notifyPool)
	: Name(_name),
	Damage(_damage),
	Cost(_cost),
	CoolDownTime(_coolDownTime),
	Avatar(_avatar),
	m_effect(effect),
	m_coolDownTimeRemain(-1.0f)
{
	m_audio = audio;
	m_notifyPool = notifyPool;
	m_notify.init();
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
			if (m_audio != nullptr) m_audio->play();
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
	m_notify.update(timer);
}

void HeroSkill::render(Camera& camera, SpriteBatch& spriteBatch)
{
	if ((!isUsable()) && (m_effect != nullptr) && (CoolDownTime - m_coolDownTimeRemain <= 2.0f))
	{
		m_effect->setPos(m_hero->getPos());
		m_effect->render(camera);
	}
	m_notify.render(spriteBatch);
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
		m_notify.respawn("Select a target !", 1.5f);
		return false;
	}

	if ((hero != target) &&
		(target->inUse()) &&
		(hero->getTeamType() != target->getTeamType()) &&
		(distance_optimized(hero->getPos(), target->getPos()) <= 10.0f))
	{
		if (target->getHealth() - Damage <= 0)
		{
			MyVec3 pos = target->getPos();
			MyVec3 offset = MyVec3(-1, 3, 0);

			int gold = target->getGoldLost();
			int exp = target->getExpLost();
			hero->accGold(gold);
			hero->accExp(exp);
			m_notifyPool->spawnNotify("+" + toString(gold), pos + offset, MyVec2(0, -0.5f), 1.5f);
		}

		target->accHealth(-Damage);
		hero->useSkill(1);
		return true;
	}

	m_notify.respawn("Out of range !", 1.5f);
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
			auto target = (*i);
			if (target->getHealth() - Damage <= 0)
			{
				MyVec3 pos = target->getPos();
				MyVec3 offset = MyVec3(-1, 3, 0);

				int gold = target->getGoldLost();
				int exp = target->getExpLost();
				hero->accGold(gold);
				hero->accExp(exp);
				m_notifyPool->spawnNotify("+" + toString(gold), pos + offset, MyVec2(0, -0.5f), 1.5f);
			}

			(*i)->accHealth(-Damage);
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