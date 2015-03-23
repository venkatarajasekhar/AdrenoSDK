
#include "MovingEntity.h"

MovingEntity::MovingEntity()
{
}

MovingEntity::~MovingEntity()
{
}

void MovingEntity::update(Timer& timer)
{
	m_position += m_velocity * timer.getElapsedTime();
}

// Getter

MyVec3 MovingEntity::getPos()
{
	return m_position;
}

// Setter

void MovingEntity::setVelocity(const MyVec3& velocity)
{
	m_velocity = velocity;
}