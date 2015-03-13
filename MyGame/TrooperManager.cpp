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
	CFrmMesh& mesh,
	FRM_ANIMATION_SET* animationSet,
	CFrmPackedResourceGLES& resource,
	Shader& shader)
{
	Material material;

	material.Ambient = MyVec3(0.1f, 0.1f, 0.1f);
	material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
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
	for (auto i = m_listTroopers.begin(); i != m_listTroopers.end(); i++)
		if (i->second->getIsDeleted() == true)
			removeTrooperFromList(i->second);

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
