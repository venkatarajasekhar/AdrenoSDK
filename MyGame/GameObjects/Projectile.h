
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
	MyVec3 getPos();
	void setActive(bool active);
	void setVelocity(const MyVec3& velocity);

private:
	bool m_active = false;
	Billboard* m_billboard;
	MovingEntity m_movingEntity;
};