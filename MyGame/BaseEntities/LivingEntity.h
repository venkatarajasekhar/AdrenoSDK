
#pragma once

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
	virtual SkinnedMesh1::Instance* getInstance() = 0;
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