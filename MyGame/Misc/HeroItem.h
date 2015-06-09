
#pragma once

#include <MyTexture.h>
#include <MyTimer.h>

class Hero;

//==================================================================================================================
//
// HeroItem class
//
//==================================================================================================================

class HeroItem
{
public:
	enum ITEM_TYPE
	{
		ACTIVE,
		PASSIVE,
	};

public:
	HeroItem(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime);
	virtual ~HeroItem();

	virtual HeroItem* clone() = 0;

	void update(Timer& timer, Hero* hero);
	void useItem();
	virtual void execute(Hero* hero, float elapsedTime);
	void sell();

	int getPrice();
	ITEM_TYPE getType();
	bool getIsUsing();
	int getNTime();
	void accNTime(int n);
	int getCoolDownTime();

public:
	// Item info
	MyString Name;
	MyString Desc;
	int Price;
	MyString Benefit;
	Texture* Avatar;

protected:
	ITEM_TYPE m_type;
	bool m_isUsing;
	float m_timeUse;
	float m_timeWait;
	float m_countTimeUsed;
	int m_nTime;
};

//==================================================================================================================
//
// Items
//
//==================================================================================================================

class HeroItem_HealingPotion : public HeroItem
{
public:
	HeroItem_HealingPotion(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait, _nTime)
	{}
	~HeroItem_HealingPotion(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:
	int m_count = 1;
};

class HeroItem_AcolyteStaff : public HeroItem
{
public:
	HeroItem_AcolyteStaff(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait, _nTime)
	{}
	~HeroItem_AcolyteStaff(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_LifeWard : public HeroItem
{
public:
	HeroItem_LifeWard(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait, _nTime)
	{}
	~HeroItem_LifeWard(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_BloodPouch : public HeroItem
{
public:
	HeroItem_BloodPouch(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait, _nTime)
	{}
	~HeroItem_BloodPouch(){}

	HeroItem* clone();

private:

};

class HeroItem_StaffOfTheSathlenar : public HeroItem
{
public:
	HeroItem_StaffOfTheSathlenar(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait, _nTime)
	{}
	~HeroItem_StaffOfTheSathlenar(){}

	HeroItem* clone();

private:

};

class HeroItem_LightCalvaryHat : public HeroItem
{
public:
	HeroItem_LightCalvaryHat(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type,
		float _timeUse,
		float _timeWait,
		int _nTime)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait, _nTime)
	{}
	~HeroItem_LightCalvaryHat(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};