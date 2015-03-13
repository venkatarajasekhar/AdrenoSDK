#include "Dman.h"

extern MyVec3 PositionDman;
int Dman::m_numIDs = 0;

Dman::Dman()
{
	m_id = m_numIDs++;
	m_isDeleted = false;
	m_health = MaxHealth;
	m_dam = 50;
}

void Dman::init(
	int type,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar* bloodBar)
{
	m_type = type;
	m_bloodBar = bloodBar;
	//m_bloodBar.init(tex2DShader, tex2D);
	m_instance = SkinnedMesh2::buildSkinnedMeshInstance(pos, rot, scale, "");
	//m_player.addInstance(m_instance);
	//m_dman.init(mesh, animationSet, resource, shader, pos, rot, scale);
	m_ai.init(m_type, m_instance->Position, MyVec3(0, 1, 0), m_instance->Rotation.y, m_instance->Scale);
}

void Dman::update(Timer& timer)
{
	m_ai.m_pos = m_instance->Position;
	m_ai.update(timer);
	copyAllProperties();
	PositionDman = m_instance->Position;
	
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
	}

	if (m_health <= 0) m_isDeleted = true;*/
}

void Dman::copyAllProperties()
{
	m_instance->Position = m_ai.m_pos;
	m_instance->Rotation = MyVec3(0, m_ai.m_angle, 0);
	m_instance->Scale = m_ai.m_scale;

}

void Dman::render(Camera& camera, SpriteBatch& spriteBatch)
{
	MyVec3 pos = m_instance->Position + MyVec3(-0.8, 2.3, 0);
	m_bloodBar->render(spriteBatch, camera, pos, m_health/(float)MaxHealth);
}

SkinnedMesh2::Instance* Dman::getDman()
{
	return m_instance;
}

int Dman::getId()
{
	return m_id;
}

bool Dman::getIsDeleted()
{
	return m_isDeleted;
}

void Dman::setHealth(int health)
{
	m_health = health;
}

int Dman::getHealth()
{
	return m_health;
}

void Dman::setDam(int dam)
{
	m_dam = dam;
}

int Dman::getDam()
{
	return m_dam;
}