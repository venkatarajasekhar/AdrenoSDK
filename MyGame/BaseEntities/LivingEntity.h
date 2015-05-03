
#pragma once

#include "BaseEntity.h"
#include "BloodBar.h"
#include <MyTimer.h>
#include <MyLight.h>

class LivingEntity : public BaseEntity
{
public:
	LivingEntity();
	virtual ~LivingEntity();

	virtual void init(
		int maxHealth, 
		int damage, 
		BloodBar& bloodBar, 
		const MyVec2& bloodBarScale, 
		const MyVec3& bloodBarOffset,
		std::vector<LivingEntity*>& lEnts,
		float atkRange);
	virtual void update(Timer& timer) = 0;
	virtual void render(SpriteBatch& spriteBatch, Camera& camera, Light& light);

	// Getter

	int getHealth()const;
	int getDamage()const;
	virtual MyVec3 getPos() = 0;

	bool inUse()const;

	// Setter

	void accMaxHealth(int delta);
	void accHealth(int delta);
	void accDamage(int delta);

protected:
	virtual void dead();

protected:
	int m_maxHealth;
	int m_health;
	int m_damage;

	BloodBar* m_bloodBar;
	MyVec2 m_bloodBarScale;
	MyVec3 m_bloodBarOffset;

	std::vector<LivingEntity*>* m_lEnts;
	LivingEntity* m_atkTarget;
	float m_atkRange;

	bool m_inUse;
};