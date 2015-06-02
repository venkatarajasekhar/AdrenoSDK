
#pragma once

#include <MyBillboard.h>

//==================================================================================================================
//
// HeroSkill class
//
//==================================================================================================================

class Hero;

class HeroSkill
{
public:
	HeroSkill(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar,
		Billboard* effect = nullptr);
	virtual ~HeroSkill();

	virtual void use(Hero* hero);
	virtual void update(Timer& timer);
	virtual void render(Camera& camera);

	virtual bool isUsable();
	virtual float getCoolDownTimeRemain();

public:
	MyString Name;
	int      Damage;
	int      Cost;
	float    CoolDownTime;
	Texture* Avatar;

private:
	Billboard* m_effect;
	float m_coolDownTimeRemain;
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
		Texture* _avatar,
		Billboard* effect)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar, effect)
	{}
	~HeroSkill_BattleBorn(){}

private:

};

class HeroSkill_Bladefall : public HeroSkill
{
public:
	HeroSkill_Bladefall(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar)
	{}
	~HeroSkill_Bladefall(){}

private:

};

class HeroSkill_DecimationDay : public HeroSkill
{
public:
	HeroSkill_DecimationDay(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar)
	{}
	~HeroSkill_DecimationDay(){}

private:

};

class HeroSkill_JustDesserts : public HeroSkill
{
public:
	HeroSkill_JustDesserts(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar)
	{}
	~HeroSkill_JustDesserts(){}

private:

};