
#pragma once

#include <MyTimer.h>

class MovingEntity
{
public:
	MovingEntity();
	virtual ~MovingEntity();

	virtual void update(Timer& timer);

	// Getter

	MyVec3 getPos();

	// Setter

	void setPos(const MyVec3& pos);
	void setVelocity(const MyVec3& velocity);

protected:
	MyVec3 m_position;
	MyVec3 m_velocity;
};