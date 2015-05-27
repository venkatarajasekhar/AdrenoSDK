
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
	Billboard* effect)
	: Name(_name),
	Damage(_damage),
	Cost(_cost),
	CoolDownTime(_coolDownTime),
	Avatar(_avatar),
	m_effect(effect)
{
}

HeroSkill::~HeroSkill()
{
}

void HeroSkill::use(Hero* hero)
{
	if (m_effect != nullptr)
	{
		m_effect->setPos(hero->getPos() + MyVec3(0, 5, -1));
	}
}

void HeroSkill::render(Camera& camera)
{
	if (m_effect != nullptr)
	{
		//m_effect->render(camera);
	}
}