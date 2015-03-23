
#pragma once

#include <MyBillboard.h>
#include "MovingEntity.h"

class Projectile
{
public:
	Projectile();
	~Projectile();

	void init(Billboard& billboard);
	void update(Timer& timer);
	void render(Camera& camera);

	// Setter

	void setPos(const MyVec3& pos);
	void setVelocity(const MyVec3& velocity);

private:
	Billboard* m_billboard;
	MovingEntity m_movingEntity;
};