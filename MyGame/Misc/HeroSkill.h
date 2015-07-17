
#pragma once

#include <MyTexture.h>
#include <MySphere.h>
#include "Notify.h"
#include <MyAudio.h>

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
		Sphere* effect = nullptr,
		Audio* audio = nullptr,
		NotifyPool* notifyPool = nullptr);
	virtual ~HeroSkill();

	virtual void use(Hero* hero);
	virtual void update(Timer& timer);
	virtual void render(Camera& camera, SpriteBatch& spriteBatch);

	virtual bool isUsable();
	virtual bool outOfMana();
	virtual float getCoolDownTimeRemain();

protected:
	virtual bool doUse(Hero* hero) = 0;
	virtual void doUpdate(Timer& timer) = 0;

public:
	MyString Name;
	int      Damage;
	int      Cost;
	float    CoolDownTime;
	Texture* Avatar;

	Notify2D m_notify;
	Audio* m_audio;
	NotifyPool* m_notifyPool;

protected:
	Sphere* m_effect;
	float m_coolDownTimeRemain;
	Hero* m_hero;
};

//==================================================================================================================
//
// Skills for Dan Mei hero
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
		Sphere* effect,
		Audio* audio,
		NotifyPool* notifyPool)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar, effect, audio, notifyPool)
	{}
	~HeroSkill_BattleBorn(){}

private:
	bool doUse(Hero* hero);
	void doUpdate(Timer& timer);
};

class HeroSkill_Bladefall : public HeroSkill
{
public:
	HeroSkill_Bladefall(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar,
		Sphere* effect,
		Audio* audio,
		NotifyPool* notifyPool)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar, effect, audio, notifyPool)
	{}
	~HeroSkill_Bladefall(){}

private:
	bool doUse(Hero* hero);
	void doUpdate(Timer& timer);
};

class HeroSkill_DecimationDay : public HeroSkill
{
public:
	HeroSkill_DecimationDay(
		const MyString& _name,
		int _damage,
		int _cost,
		float _coolDownTime,
		Texture* _avatar,
		Sphere* effect,
		Audio* audio,
		NotifyPool* notifyPool)
		: HeroSkill(_name, _damage, _cost, _coolDownTime, _avatar, effect, audio, notifyPool)
	{}
	~HeroSkill_DecimationDay(){}

private:
	bool doUse(Hero* hero);
	void doUpdate(Timer& timer);
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
	bool doUse(Hero* hero);
	void doUpdate(Timer& timer);
};