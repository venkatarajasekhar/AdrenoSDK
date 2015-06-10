#pragma once

#include "LivingEntity.h"
#include <MyUILabel.h>

//==============================================================================================================
//
// Projectile class
//
//==============================================================================================================


class Notify : public BaseEntity
{
public:
	Notify();
	~Notify();

	void init(Font& font);
	void update(Timer& timer);
	void render(Camera& camera, SpriteBatch& spriteBatch);
	void respawn(const MyString& content,
		const MyVec3& position,
		const MyVec2& vTranslate,
		float nTime);
	bool inUse()const;

private:
	UILabel m_label;

	MyVec3 m_position;
	MyVec2 m_vTranslate;
	MyString m_content;
	bool m_inUse;
	float m_nTime;
	float m_countTime;
};

//==============================================================================================================
//
// NotifyPool class
//
//==============================================================================================================

class NotifyPool
{
private:
	static const int MAX_NUM_NOTIFIES = 10;

public:
	NotifyPool();
	~NotifyPool();

	void init();
	void update(Timer& timer);
	void render(Camera& camera, SpriteBatch& spriteBatch);

	void spawnNotify(const MyString& content,
		const MyVec3& position,
		const MyVec2& vTranslate,
		float nTime);

private:
	Notify* getFreeSlot();

private:
	Notify m_notifies[MAX_NUM_NOTIFIES];
	Font m_font;
};