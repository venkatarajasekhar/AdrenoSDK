
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

	virtual void init(int maxHealth, int damage, ENTITY_TYPE entityType, TEAM_TYPE teamType);

	int getHealth()const;
	void setHealth(int health);
	float getHealthRatio()const;
	int getDamage()const;
	bool isDead()const;
	int getIdEntity();
	virtual SkinnedMesh1::Instance* getInstance();
	virtual void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	virtual void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);

protected:
	int m_maxHealth;
	int m_health;
	int m_damage;
	int m_idEntity;
	static int m_numIDs;
};