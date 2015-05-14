
#pragma once

#include <MyTexture.h>

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
		ITEM_TYPE _type);
	virtual ~HeroItem();

	virtual HeroItem* clone() = 0;

public:
	// Item info
	MyString Name;
	MyString Desc;
	int Price;
	MyString Benefit;
	Texture* Avatar;

protected:
	ITEM_TYPE m_type;
};

//==================================================================================================================
//
// Items
//
//==================================================================================================================

class HeroItem_ChainMail : public HeroItem
{
public:
	HeroItem_ChainMail(
		const MyString& _name,
		const MyString& _desc,
		int _price,
		const MyString& _benefit,
		Texture& _avatar,
		ITEM_TYPE _type)
		: HeroItem(_name, _desc, _price, _benefit, _avatar, _type)
	{}
	~HeroItem_ChainMail(){}

	HeroItem* clone(){ return nullptr; }

private:

};

class HeroItem_CloakOfTheResistant : public HeroItem
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

	HeroItem* clone(){ return nullptr; }

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

	HeroItem* clone(){ return nullptr; }

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

	HeroItem* clone(){ return nullptr; }

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

	HeroItem* clone(){ return nullptr; }

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

	HeroItem* clone(){ return nullptr; }

private:

};