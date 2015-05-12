#pragma once

#include <MyTexture.h>
#include <MyUILabel.h>

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