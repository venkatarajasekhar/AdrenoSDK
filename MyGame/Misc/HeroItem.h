
#pragma once

#include <MyTexture.h>
#include <MyTimer.h>

/*
enum ITEM_TYPE
{
	ACTIVE,
	PASSIVE,
};

class Item
{
public:
	Item();
	~Item();
	void init();
	void update(Timer& timer);
	void use();
	void sell();
	void drop();
	void pick();
	Item clone();

	int getPrice();
private:
	//Texture m_icon;
	//UILabel m_name;
	int m_price;
	ITEM_TYPE m_type;
	bool m_isUsing;
	bool m_isDropping;
	float m_timeUse;
	float m_timeWait;
	float m_countTimeUsed;
};
/**/

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
		float _timeWait);
	virtual ~HeroItem();

	virtual HeroItem* clone() = 0;

	void update(Timer& timer, Hero* hero);
	void useItem();
	virtual void execute(Hero* hero);
	void sell();

	int getPrice();

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
		float _timeWait)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type, _timeUse, _timeWait)
	{}
	~HeroItem_HealingPotion(){}

	HeroItem* clone();
	void execute(Hero* hero);

private:

};

/*class HeroItem_CloakOfTheResistant : public HeroItem
{
public:
	HeroItem_CloakOfTheResistant(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type)
	{}
	~HeroItem_CloakOfTheResistant(){}

	HeroItem* clone();

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
		ITEM_TYPE _type)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type)
	{}
	~HeroItem_LifeWard(){}

	HeroItem* clone();

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
		ITEM_TYPE _type)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type)
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
		ITEM_TYPE _type)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type)
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
		ITEM_TYPE _type)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type)
	{}
	~HeroItem_LightCalvaryHat(){}

	HeroItem* clone();

private:

};*/