#include "EnemyAI.h"

extern MyVec3 PositionPlayer;

void EnemyAI::init(
	int type,
	MyVec3 pos,
	MyVec3 vectorRotation,
	float angle,
	MyVec3 scale
	)
{
	m_type = type;
	m_pos = pos;
	m_vectorRotation = vectorRotation;
	m_angle = angle;
	m_scale = scale;
}



void EnemyAI::update(Timer& timer)
{
	if (m_enemyState == Die)
	{
		/*countTime += (float)gameTime.ElapsedGameTime.TotalSeconds;
		if (countTime >= 1.5)
		{
		if (rand.Next(3) == 0)
		{
		int type = rand.Next(4) + 1;
		Product product = CreateProduct(type);
		_level.ProductGroup.AddProduct(product);
		}
		_level.EnemyGroup.KillEnemy(this);
		_level.EnemyGroup.AddEnemy();
		}*/
	}
	else
	{
		float EnemyChaseThreshold = EnemyChaseDistance;
		float EnemyCaughtThreshold = EnemyCaughtDistance;
		//m_enemyOrientation = m_angle;

		if (m_enemyState == Wander)
		{
			EnemyChaseThreshold -= EnemyHysteresis / 2;
		}
		else if (m_enemyState == Chasing)
		{
			EnemyChaseThreshold += EnemyHysteresis / 2;
			EnemyCaughtThreshold -= EnemyHysteresis / 2;
		}
		else if (m_enemyState == Caught)
		{
			EnemyCaughtThreshold += EnemyHysteresis / 2;
		}

		float distanceFromCat = distance(m_pos, PositionPlayer);
		if (distanceFromCat > EnemyChaseThreshold)
		{
			//if (m_enemyState != Wander) SetAnim(this, 0);
			m_enemyState = Wander;
		}
		else if (distanceFromCat > EnemyCaughtThreshold)
		{
			//if (m_enemyState != Chasing) SetAnim(this, 0);
			m_enemyState = Chasing;
		}
		else
		{
			/*countTime += (float)gameTime.ElapsedGameTime.TotalSeconds;
			if (countTime >= 1.5)
			{
			SoundAttack();
			countTime = -2.5f;
			_level.Player.AttackPlayer(EnemyDam);
			}*/

			//if (m_enemyState != Caught) SetAnim(this, 0);
			m_enemyState = Caught;
		}

		float currentEnemySpeed;
		if (m_enemyState == Chasing)
		{
			m_enemyOrientation = TurnToFace(m_pos, PositionPlayer, m_enemyOrientation, EnemyTurnSpeed);
			currentEnemySpeed = MaxEnemySpeed;
		}
		else if (m_enemyState == Wander)
		{
			//Wanders(m_pos, m_enemyWanderDirection, m_enemyOrientation, EnemyTurnSpeed);
			Wanders(m_pos, m_enemyOrientation, EnemyTurnSpeed);
			currentEnemySpeed = .5f * MaxEnemySpeed;
		}
		else
		{
			currentEnemySpeed = 0.0f;
		}

		MyVec3 heading = MyVec3(
			(float)dCos(m_enemyOrientation), 0, (float)dSin(m_enemyOrientation));

		m_pos = MoveEnemy(m_pos, heading * currentEnemySpeed, timer);
		//m_pos.x = clamp(m_pos.x, -20.0f, 20.0f);
		//m_pos.z = clamp(m_pos.z, -20.0f, 20.0f);

		float rY = WrapAngle(-m_enemyOrientation);
		m_angle = rY + 100;
	}

}

MyVec3 EnemyAI::MoveEnemy(MyVec3 currPos, MyVec3 moveAmt, Timer& timer)
{
	MyVec3 foreMove = currPos + moveAmt * timer.getElapsedTime() * 40.0f;

	// kiem tra dich den co phai mat dat k?
	/*if (_level.Terrain.DetermineTerrainType(foreMove.x, foreMove.z) == Terrain.TerrainType.Ground)
	{
	m_pos = foreMove;
	if ((_level.Terrain.CheckCollision(foreMove.X, foreMove.Z, this)) || (MonsterCheckCollision()))
	return currPos;
	else
	return foreMove;
	}
	else
	return currPos;*/

	// tam thoi la thich di dau cung dc
	return foreMove;
}

void EnemyAI::Wanders(MyVec3 position, float& orientation, float turnSpeed)
{
	float e = 0.1f;

	if (fabs(m_pos.x + m_pos.z) > e)
	{
		MyVec3 positionNext = MyVec3(m_pos.x, 0, -m_pos.x);
		orientation = TurnToFace(m_pos, positionNext, orientation, .15f * turnSpeed);
	}
	else
	{
		MyVec3 positionNext;
		if (m_type == 0) positionNext = MyVec3(20, 0, -20);
		else positionNext = MyVec3(-20, 0, 20);
		orientation = TurnToFace(m_pos, positionNext, orientation, .15f * turnSpeed);
	}
}

void EnemyAI::Wanders(MyVec3 position, MyVec3& wanderDirection, float& orientation, float turnSpeed)
{
	wanderDirection.x += lerp(-.15f, .15f, random(0.0f, 1.0f));
	wanderDirection.z += lerp(-.15f, .15f, random(0.0f, 1.0f));

	if (wanderDirection != MyVec3(0, 0, 0))
	{
		wanderDirection = normalize(wanderDirection);
	}

	orientation = TurnToFace(position, position + wanderDirection, orientation,
		.15f * turnSpeed);

	MyVec3 screenCenter = MyVec3(20, 0, -20);

	float distanceFromScreenCenter = distance(screenCenter, position);
	float MaxDistanceFromScreenCenter = min(screenCenter.z, screenCenter.x);

	float normalizedDistance = distanceFromScreenCenter / MaxDistanceFromScreenCenter;

	float turnToCenterSpeed = .3f * normalizedDistance * normalizedDistance * turnSpeed;

	orientation = TurnToFace(position, screenCenter, orientation, turnToCenterSpeed);
}

float EnemyAI::TurnToFace(MyVec3 position, MyVec3 faceThis, float currentAngle, float turnSpeed)
{
	float x = faceThis.x - position.x;
	float z = faceThis.z - position.z;

	float desiredAngle = dATan2(z, x);

	float difference = WrapAngle(desiredAngle - currentAngle);

	difference = clamp(difference, -turnSpeed, turnSpeed);

	return WrapAngle(currentAngle + difference);
}

float EnemyAI::WrapAngle(float radians)
{
	while (radians < -180)
	{
		radians += 360;
	}
	while (radians > 180)
	{
		radians -= 360;
	}
	return radians;
}

void EnemyAI::rotateEnemy(const MyVec3& positionNext)
{
	MyVec3 position = m_pos;
	MyVec3 dir = position - positionNext;
	MyVec3 baseVec = MyVec3(0, 0, -1);

	dir = normalize(dir);
	float cos = dot(dir, baseVec);

	if (cos < -1)
		cos = -1;
	if (cos > 1)
		cos = 1;

	float angle = dACos(cos);
	if (positionNext.x <= position.x)
	{
		angle = -angle;
	}
	m_angle = angle - 20;
}