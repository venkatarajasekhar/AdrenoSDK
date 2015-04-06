
#pragma once

#include <MyBillboard.h>
#include "BaseEntity.h"
#include "MovingEntity.h"
#include <MyTexture.h>

class Projectile : public BaseEntity
{
public:
	Projectile();
	~Projectile();

	void init(Billboard& billboard, 
		int idEnemy, 
		int idHero,
		float range,
		int damage,
		TEAM_TYPE teamType);
	void update(Timer& timer);
	void render(Camera& camera);

	// Setter
	void setPos(const MyVec3& pos);
	MyVec3 getPos();
	int getIdEnemy();
	int getIdHero();
	float getRange();
	void setActive(bool active);
	bool getActive();
	void setTarget(const MyVec3& target);

private:
	bool m_active;
	Billboard* m_billboard;
	MovingEntity m_movingEntity;
	int m_idEnemy;
	int m_idHero;
	float m_range;
	int m_damage;
};