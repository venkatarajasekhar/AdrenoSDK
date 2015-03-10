#pragma once

#include <MyUtils.h>
#include <MyTimer.h>

class EnemyAI
{
public:
	int m_type;
	MyVec3 m_pos;
	MyVec3 m_scale;
	MyVec3 m_vectorRotation;
	float m_angle;

	// enumeration
	enum EnemyAiState
	{
		Chasing,
		Caught,
		Wander,
		Die
	};

	// constant
	const float MaxEnemySpeed = 0.05f;
	const float EnemyTurnSpeed = 5.0f;
	const float EnemyChaseDistance = 8.0f;
	const float EnemyCaughtDistance = 1.5f;
	const float EnemyHysteresis = 0.5f;

	// field

	EnemyAiState m_enemyState = Wander;
	float m_enemyOrientation;
	MyVec3 m_enemyWanderDirection;
	int m_enemyHP;
	int m_enemyDam;

public:
	void init(
		int type,
		MyVec3 pos,
		MyVec3 vectorRotation,
		float angle,
		MyVec3 scale);
	void update(Timer& timer);
	float TurnToFace(MyVec3 position, MyVec3 faceThis, float currentAngle, float turnSpeed);
	void Wanders(MyVec3 position, float& orientation, float turnSpeed);
	void Wanders(MyVec3 position, MyVec3& wanderDirection, float& orientation, float turnSpeed);
	MyVec3 MoveEnemy(MyVec3 currPos, MyVec3 moveAmt, Timer& timer);
	float WrapAngle(float radians);
	void rotateEnemy(const MyVec3& positionNext);
};