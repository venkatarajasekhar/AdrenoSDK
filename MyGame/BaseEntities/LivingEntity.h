
#pragma once

#include "BaseEntity.h"
#include "BloodBar.h"
#include <MyTimer.h>
#include <MyLight.h>

/*
#include <MyInput.h>
#include <MySpriteBatch.h>
#include "BaseEntity.h"
#include "MySkinnedMesh1.h"

class LivingEntity : public BaseEntity
{
public:
	LivingEntity();
	virtual ~LivingEntity();

	virtual void init(int maxHealth, int damage, float range, float radius, ENTITY_TYPE entityType, TEAM_TYPE teamType);

	int getHealth()const;
	void setHealth(int health);
	float getHealthRatio()const;
	int getDamage()const;
	float getRange()const;
	bool isDead()const;
	int getIdEntity()const;
	float getRadius()const;
	virtual Mesh::Instance* getInstance() = 0;
	virtual void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height) = 0;
	virtual void render(Camera& camera, Light& light, SpriteBatch& spriteBatch) = 0;

protected:
	int m_maxHealth;
	int m_health;
	int m_damage;
	float m_range;
	int m_idEntity;
	float m_radius;
	static int m_numIDs;
};
/**/

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