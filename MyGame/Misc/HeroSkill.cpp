
#include "HeroSkill.h"

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
	Texture* _avatar)
	: Name(_name),
	Damage(_damage),
	Cost(_cost),
	CoolDownTime(_coolDownTime),
	Avatar(_avatar)
{
}

HeroSkill::~HeroSkill()
{
}