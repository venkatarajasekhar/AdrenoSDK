
#include "MovingEntity.h"

#pragma region Constants

//========================================================================================================================
//
// Constants
//
//========================================================================================================================

static const float DISTANCE_THRESHOLD = 0.1f;

#pragma endregion

#pragma region Helper functions

//========================================================================================================================
//
// Helper functions
//
//========================================================================================================================

static float turnToFace(float elapsedTime, MyVec3 position, MyVec3 faceThis, float currentAngle, float turnSpeed)
{
	float x = faceThis.x - position.x;
	float z = faceThis.z - position.z;

	float desiredAngle = dATan2(x, z);
	
	float difference = wrapAngle(desiredAngle - currentAngle);
	difference = clamp(difference, -turnSpeed * elapsedTime, turnSpeed * elapsedTime);

	return wrapAngle(currentAngle + difference);
}

#pragma endregion

//========================================================================================================================
//
// MovingEntity class
//
//========================================================================================================================

MovingEntity::MovingEntity()
	: m_rotYOffset(0),
	m_currOrientation(0),
	m_speed(0),
	m_turnSpeed(0),
	m_isMoving(false),
	m_pathPivot(0),
	m_followingPath(true)
{
}

MovingEntity::~MovingEntity()
{
}

void MovingEntity::init(
	const MyVec3& pos, 
	const MyVec3& target, 
	const MyVec3& rot, 
	float rotYOffset, 
	float speed, 
	float turnSpeed)
{
	setPos(pos);
	setTarget(target);
	setRot(rot);
	setSpeed(speed);

	m_rotYOffset = rotYOffset;
	m_turnSpeed = turnSpeed;
}

void MovingEntity::init(
	const std::vector<MyVec3>& path,
	float rotYOffset,
	float speed,
	float turnSpeed)
{
	setSpeed(speed);
	setPath(path);

	m_rotYOffset = rotYOffset;
	m_turnSpeed = turnSpeed;
}

void MovingEntity::update(Timer& timer)
{
	if (distance_optimized(m_position, m_target) > DISTANCE_THRESHOLD)
	{
		m_currOrientation = turnToFace(timer.getElapsedTime(), m_position, m_target, m_currOrientation, m_turnSpeed);

		MyVec3 heading = m_speed * MyVec3(dSin_optimized(m_currOrientation), 0, dCos_optimized(m_currOrientation));

		m_position += heading * timer.getElapsedTime();
		m_rotation.y = m_currOrientation - m_rotYOffset;

		m_isMoving = true;
	}
	else
	{
		if (!m_path.empty() && m_followingPath)
		{
			m_pathPivot++;
			if (m_pathPivot < m_path.size())
			{
				setTarget(m_path[m_pathPivot]);
			}
			else
			{
				m_isMoving = false;
			}
		}
		else
		{
			m_isMoving = false;
		}
	}
}

void MovingEntity::turnTo(const MyVec3& target)
{
	m_currOrientation = turnToFace(1, m_position, target, m_currOrientation, 180);
	m_rotation.y = m_currOrientation - m_rotYOffset;
}

// Getter

MyVec3 MovingEntity::getPos()
{
	return m_position;
}

MyVec3 MovingEntity::getRot()
{
	return m_rotation;
}

float MovingEntity::getRotYOffset()
{
	return m_rotYOffset;
}


bool MovingEntity::isMoving()
{
	return m_isMoving;
}

// Setter

void MovingEntity::setPos(const MyVec3& pos)
{
	m_position = pos;
}

void MovingEntity::setRot(const MyVec3& rot)
{
	m_rotation = rot;
}

void MovingEntity::setTarget(const MyVec3& target)
{
	m_target = target;
}

void MovingEntity::setSpeed(float speed)
{
	m_speed = (speed < 0 ? 0 : speed);
}

void MovingEntity::setPath(const std::vector<MyVec3>& path)
{
	m_path = path;
	m_pathPivot = 1;
	m_followingPath = true;

	if (!m_path.empty())
	{
		setPos(m_path[0]);
		MyVec3 target = ((m_path.size() >= 2) ? m_path[1] : m_path[0]);
		setTarget(target);
	}
}

void MovingEntity::accelerate(float dSpeed)
{
	setSpeed(m_speed + dSpeed);
}

void MovingEntity::disFollowPath()
{
	m_followingPath = false;
}

void MovingEntity::reFollowPath()
{
	if (0 <= m_pathPivot && m_pathPivot < m_path.size())
	{
		setTarget(m_path[m_pathPivot]);
	}
	
	m_followingPath = true;
}