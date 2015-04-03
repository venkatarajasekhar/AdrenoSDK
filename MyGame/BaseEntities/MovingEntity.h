
#pragma once

#include <MyTimer.h>

class MovingEntity
{
public:
	MovingEntity();
	virtual ~MovingEntity();

	// rotYOffset: When rotY = 0, angle between entity's heading and Oz
	//             -180 <= rotYOffset <= 180
	//             counter-clockwise (toward Oz) is positive direction
	virtual void init(const MyVec3& pos, const MyVec3& target, const MyVec3& rot, float rotYOffset, float speed, float turnSpeed);
	virtual void update(Timer& timer);

	// Getter

	MyVec3 getPos();
	MyVec3 getRot();
	bool isMoving();

	// Setter

	void setPos(const MyVec3& pos);
	void setRot(const MyVec3& rot);
	void setTarget(const MyVec3& target);
	void setSpeed(float speed);

	void accelerate(float dSpeed);

protected:
	MyVec3 m_position;
	MyVec3 m_target;

	MyVec3 m_rotation;
	float m_rotYOffset;
	float m_currOrientation;
	
	float m_speed;
	float m_turnSpeed;

	bool m_isMoving;
};