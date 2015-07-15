
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
	bool m_isUsed = false;
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

class HeroItem_BootsOfSpeed : public HeroItem
{
public:
	HeroItem_BootsOfSpeed(
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
	~HeroItem_BootsOfSpeed(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_PhaseBoots : public HeroItem
{
public:
	HeroItem_PhaseBoots(
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
	~HeroItem_PhaseBoots(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_PowerTreads : public HeroItem
{
public:
	HeroItem_PowerTreads(
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
	~HeroItem_PowerTreads(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_ArcaneBoots : public HeroItem
{
public:
	HeroItem_ArcaneBoots(
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
	~HeroItem_ArcaneBoots(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_DemonEdge : public HeroItem
{
public:
	HeroItem_DemonEdge(
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
	~HeroItem_DemonEdge(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_SacredRelic : public HeroItem
{
public:
	HeroItem_SacredRelic(
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
	~HeroItem_SacredRelic(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_Quarterstaff : public HeroItem
{
public:
	HeroItem_Quarterstaff(
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
	~HeroItem_Quarterstaff(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_Broadsword : public HeroItem
{
public:
	HeroItem_Broadsword(
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
	~HeroItem_Broadsword(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_Claymore : public HeroItem
{
public:
	HeroItem_Claymore(
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
	~HeroItem_Claymore(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};

class HeroItem_DivineRapier : public HeroItem
{
public:
	HeroItem_DivineRapier(
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
	~HeroItem_DivineRapier(){}

	HeroItem* clone();
	void execute(Hero* hero, float elapsedTime);

private:

};