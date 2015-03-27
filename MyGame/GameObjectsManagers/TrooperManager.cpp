#include "TrooperManager.h"

TrooperManager::TrooperManager()
{
}

TrooperManager::~TrooperManager()
{
	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		delete i->second;
}

void TrooperManager::init(
	int type,
	CFrmMesh& mesh,
	FRM_ANIMATION_SET* animationSet,
	CFrmPackedResourceGLES& resource,
	Shader& shader)
{
	m_type = type;

	Material material;

	material.Ambient = MyVec3(0.1f, 0.1f, 0.1f);
	material.Diffuse = MyVec4(1.0f, 0.0f, 0.0f, 1.0f);
	material.Specular = MyVec4(0.4f, 0.4f, 0.4f, 1.0f);
	material.Shininess = 16.0f;

	m_trooper.init(mesh, animationSet, resource, shader, &material, NULL);
}

void TrooperManager::insertTrooperToList(Trooper* trooper)
{
	auto i = m_listTroopers.find(trooper->getId());
	if (i == m_listTroopers.end())
	{
		m_listTroopers[trooper->getId()] = trooper;
		m_trooper.addInstance(trooper->getTrooper());
	}
}

void TrooperManager::removeTrooperFromList(Trooper* trooper)
{
	m_listTroopers.erase(trooper->getId());
	SAFE_DELETE(trooper);
}

void TrooperManager::update(Timer& timer)
{
	auto i = m_listTroopers.end();
	if (i != m_listTroopers.begin())
	{
		i--;
		while (true)
		{
			bool flag = false;
			std::map<int, Trooper*>::iterator j;

			if (i == m_listTroopers.begin())
			{
				flag = true;
			}
			else
			{
				j = i;
				j--;
			}

			if (i->second->getIsDeleted() == true)
			{

				m_trooper.removeInstance(i->second->getTrooper());
				removeTrooperFromList(i->second);
			}

			if (flag)
			{
				break;
			}
			else
			{
				i = j;
			}
		}
	}

	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		i->second->update(timer);

	m_trooper.update(timer);
}

void TrooperManager::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	m_trooper.render(camera, &light);
	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		i->second->render(camera, spriteBatch);
}

int TrooperManager::getNTrooper()
{
	return m_listTroopers.size();
}

Trooper* TrooperManager::getTrooperById(int id)
{
	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		if (i->second->getId() == id)
		{
			return i->second;
		}
}

int TrooperManager::getIdTrooperToBeat(MyVec3 positionPlayer)
{
	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		if ((distance(positionPlayer, i->second->getTrooper()->Position) < 2.0f) && (!i->second->getIsDeleted()))
		{
			smartLog(toString(i->second->getId()));
			return i->second->getId();
		}
	return -1;
}

bool TrooperManager::checkTrooperCanMove(MyVec3 positionTrooper, int type)
{
	if (type != m_type)
	{
		for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
			if (twoPointIsContact(positionTrooper, i->second->getTrooper()->Position, 1))
				return false;
	}
	else
	{
		int count = 0;
		for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
			if (twoPointIsContact(positionTrooper, i->second->getTrooper()->Position, 1))
			{
				count++;
				if (count == 2)
					return false;
			}
	}

	return true;
}

void TrooperManager::beatTroopers(MyVec3 positionBeat, int dam)
{
	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		if ((!i->second->getIsDeleted()) && (distance(positionBeat, i->second->getTrooper()->Position) < 2.0f))
		{
			smartLog(toString(i->second->getId()) + " " + toString(dam));
			i->second->setHealth(i->second->getHealth() - dam);
		}
}