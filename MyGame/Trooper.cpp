#include "Global.h"
#include "Trooper.h"

int Trooper::m_numIDs = 0;

Trooper::Trooper()
{
	m_id = m_numIDs++;
	m_isDeleted = false;
	m_health = MaxHealth;
	m_dam = 50;
}

void Trooper::init(
	int type,
	int team,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar* bloodBar)
{
	m_type = type;
	m_team = team;
	m_bloodBar = bloodBar;
	//m_bloodBar.init(tex2DShader, tex2D);
	m_instance = SkinnedMesh2::buildSkinnedMeshInstance(pos, rot, scale, "");
	//m_player.addInstance(m_instance);
	//m_dman.init(mesh, animationSet, resource, shader, pos, rot, scale);
	m_ai.init(m_type, m_team, m_instance->Position, MyVec3(0, 1, 0), m_instance->Rotation.y, m_instance->Scale);
}

void Trooper::update(Timer& timer)
{
	m_ai.m_pos = m_instance->Position;
	m_ai.update(timer);
	copyAllProperties();

	/*float e = 0.1f;
	if (m_type == 0)
	{
		if ((fabs(m_instance->Position.x - 20) < e) && (fabs(m_instance->Position.z + 20) < e))
		{
			m_isDeleted = true;
		}
	}
	else
	{
		if ((fabs(m_instance->Position.x + 20) < e) && (fabs(m_instance->Position.z - 20) < e))
		{
			m_isDeleted = true;
		}
	}*/

	if (m_health <= 0) m_isDeleted = true;
}

void Trooper::copyAllProperties()
{
	if (g_dmanManager.checkTrooperCanMove(m_ai.m_pos, m_type))
		m_instance->Position = m_ai.m_pos;
	else m_ai.m_pos = m_instance->Position;
	m_instance->Rotation = MyVec3(0, m_ai.m_angle, 0);
	m_instance->Scale = m_ai.m_scale;

}

void Trooper::render(Camera& camera, SpriteBatch& spriteBatch)
{
	MyVec3 pos = m_instance->Position + MyVec3(-0.8, 2.3, 0);
	m_bloodBar->render(spriteBatch, camera, pos, m_health/(float)MaxHealth);
}

SkinnedMesh2::Instance* Trooper::getTrooper()
{
	return m_instance;
}

int Trooper::getId()
{
	return m_id;
}

bool Trooper::getIsDeleted()
{
	return m_isDeleted;
}

void Trooper::setHealth(int health)
{
	m_health = health;
}

int Trooper::getHealth()
{
	return m_health;
}

void Trooper::setDam(int dam)
{
	m_dam = dam;
}

int Trooper::getDam()
{
	return m_dam;
}