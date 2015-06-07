
#pragma once

#include "LivingEntity.h"
#include "MovingEntity.h"
#include <MyBillboard.h>

//==============================================================================================================
//
// Projectile class
//
//==============================================================================================================


class Projectile : public BaseEntity
{
public:
	Projectile();
	~Projectile();

	void init();
	void update(Timer& timer);
	void render(Camera& camera);

	void respawn(Billboard& billboard, LivingEntity* attacker, LivingEntity* atkTarget, MyVec3 attackerOffset);

	// Getter

	bool inUse()const;

	// Setter

	void setPos(const MyVec3& pos);
	void setTarget(const MyVec3& target);

private:
	// Mesh/Appearance elements
	Billboard* m_billboard;

	MyVec3 m_position;
	MyVec3 m_target;

	bool m_inUse;

	LivingEntity* m_attacker;
	LivingEntity* m_atkTarget;
};

//==============================================================================================================
//
// ProjectilePool class
//
//==============================================================================================================

class ProjectilePool
{
private:
	static const int MAX_NUM_PROJECTILES = 10;

public:
	ProjectilePool();
	~ProjectilePool();

	void init();
	void update(Timer& timer);
	void render(Camera& camera);

	void spawnProjectile(Billboard& billboard, LivingEntity* attacker, LivingEntity* atkTarget, MyVec3 attackerOffset);

private:
	Projectile* getFreeSlot();

private:
	Projectile m_projectiles[MAX_NUM_PROJECTILES];
};