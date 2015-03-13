#include "DmanManager.h"

DmanManager::DmanManager()
{
}

DmanManager::~DmanManager()
{
	for (auto i = m_listDmans.begin(); i != m_listDmans.end(); i++)
		delete i->second;
}

void DmanManager::init(
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

	m_dman.init(mesh, animationSet, resource, shader, &material, NULL);
}

void DmanManager::insertDmanToList(Dman* dman)
{
	auto i = m_listDmans.find(dman->getId());
	if (i == m_listDmans.end())
	{
		m_listDmans[dman->getId()] = dman;
		m_dman.addInstance(dman->getDman());
	}
}

void DmanManager::removeDmanFromList(Dman* dman)
{
	m_listDmans.erase(dman->getId());
	SAFE_DELETE(dman);
}

void DmanManager::update(Timer& timer)
{
	for (auto i = m_listDmans.begin(); i != m_listDmans.end(); i++)
		if (i->second->getIsDeleted() == true)
			removeDmanFromList(i->second);

	for (auto i = m_listDmans.begin(); i != m_listDmans.end(); i++)	
		i->second->update(timer);

	m_dman.update(timer);
}

void DmanManager::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	m_dman.render(camera, &light);
	for (auto i = m_listDmans.begin(); i != m_listDmans.end(); i++)
		i->second->render(camera, spriteBatch);
}

int DmanManager::getNDman()
{
	return m_listDmans.size();
}
