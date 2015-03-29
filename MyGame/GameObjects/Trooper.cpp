#include "Trooper.h"

Trooper::Trooper()
{

}

void Trooper::init(
	int trooperType,
	TEAM_TYPE teamType,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar* bloodBar)
{
	m_bloodBar = bloodBar;
	
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "");
	m_ai.init(trooperType, teamType, m_instance->Position, MyVec3(0, 1, 0), m_instance->Rotation.y, m_instance->Scale);
}

void Trooper::update(Timer& timer)
{
	m_ai.m_pos = m_instance->Position;
	m_ai.update(timer);
	copyAllProperties();
}

void Trooper::copyAllProperties()
{
	if (g_livingEntityManager.checkLivingEntityCanMove(m_ai.m_pos, m_teamType))
		m_instance->Position = m_ai.m_pos;
	else m_ai.m_pos = m_instance->Position;
	m_instance->Rotation = MyVec3(0, m_ai.m_angle, 0);
	m_instance->Scale = m_ai.m_scale;

}

void Trooper::render(Camera& camera, SpriteBatch& spriteBatch)
{
	MyVec3 pos = m_instance->Position + MyVec3(-0.8, 2.3, 0);
	m_bloodBar->render(spriteBatch, camera, pos, m_health/(float)m_maxHealth);
}

SkinnedMesh1::Instance* Trooper::getInstance()
{
	return m_instance;
}

int Trooper::getTrooperType()
{
	return m_trooperType;
}