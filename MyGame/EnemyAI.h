#pragma once

#include <MyUtils.h>

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
	const float MaxEnemySpeed = 0.06f;
	const float EnemyTurnSpeed = 25.0f;
	const float EnemyChaseDistance = 8.0f;
	const float EnemyCaughtDistance = 2.0f;
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
	void update(CFrmTimer& timer);
	float TurnToFace(MyVec3 position, MyVec3 faceThis, float currentAngle, float turnSpeed);
	void Wanders(MyVec3 position, float& orientation, float turnSpeed);
	void Wanders(MyVec3 position, MyVec3& wanderDirection, float& orientation, float turnSpeed);
	MyVec3 MoveEnemy(MyVec3 currPos, MyVec3 moveAmt);
	float WrapAngle(float radians);
	void rotateEnemy(const MyVec3& positionNext);
};