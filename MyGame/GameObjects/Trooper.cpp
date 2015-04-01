#include "Trooper.h"

Trooper::Trooper()
{

}

void Trooper::init(
	int trooperType,
	TEAM_TYPE teamType,
	int health,
	int damage,
	float range,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar* bloodBarRed,
	BloodBar* bloodBarGreen)
{
	if (teamType == MY_TEAM) m_bloodBar = bloodBarGreen;
	else m_bloodBar = bloodBarRed;
	
	LivingEntity::init(health, damage, range, 2.0f, ENTITY_TYPE_PAWN, teamType);
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "Run");
	
	MyVec3 pointEnd = pos;
	if (teamType == MY_TEAM) pointEnd.x = 20;
	else pointEnd.x = -20;
	
	m_ai.init(m_idEntity, teamType, m_instance->Position, MyVec3(0, 1, 0), m_instance->Rotation.y, m_instance->Scale, pointEnd);
}

void Trooper::update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height)
{
	m_ai.m_pos = m_instance->Position;
	m_ai.update(timer);
	copyAllProperties();
}

void Trooper::copyAllProperties()
{
	//if (g_livingEntityManager.checkLivingEntityCanMove(m_ai.m_pos, m_teamType))
		//m_instance->Position = m_ai.m_pos;
	//else m_ai.m_pos = m_instance->Position;
	m_instance->Position = m_ai.m_pos;
	m_instance->Rotation = MyVec3(0, m_ai.m_angle, 0);
	m_instance->Scale = m_ai.m_scale;

}

void Trooper::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	MyVec3 pos = m_instance->Position + MyVec3(-0.8, 2.3, 0);
	m_bloodBar->render(spriteBatch, camera, pos, getHealthRatio());
}

SkinnedMesh1::Instance* Trooper::getInstance()
{
	return m_instance;
}


int Trooper::getTrooperType()
{
	return m_trooperType;
}