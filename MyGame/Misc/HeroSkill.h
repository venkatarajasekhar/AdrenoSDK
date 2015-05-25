
#pragma once

#include <MyTexture.h>

//==================================================================================================================
//
// HeroSkill class
//
//==================================================================================================================

class HeroSkill
{
public:
	HeroSkill(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar);
	virtual ~HeroSkill();

public:
	MyString Name;
	int Damage;
	int Cost;
	float CoolDownTime;
	Texture* Avatar;
};

//==================================================================================================================
//
// Skills
//
//==================================================================================================================

class HeroSkill_BattleBorn : public HeroSkill
{
public:
	HeroSkill_BattleBorn(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar)
	{}
	~HeroSkill_BattleBorn(){}

private:

};