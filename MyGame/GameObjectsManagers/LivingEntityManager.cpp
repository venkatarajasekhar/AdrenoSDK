#include "Global.h"
#include "LivingEntityManager.h"

LivingEntityManager::LivingEntityManager()
{
}

LivingEntityManager::~LivingEntityManager()
{
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		delete i->second;
}

/*void LivingEntityManager::init(
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
}*/

void LivingEntityManager::init(
	SkinnedMesh1* scorpion)
{
	m_troopers[TROOPER_SCORPION] = scorpion;
}

void LivingEntityManager::insertLivingEntityToList(LivingEntity* entity, int typeEntity)
{
	auto i = m_listLivingEntitys.find(entity->getIdEntity());
	if (i == m_listLivingEntitys.end())
	{
		m_listLivingEntitys[entity->getIdEntity()] = entity;
		if (typeEntity != -1) m_troopers[typeEntity]->addInstance(entity->getInstance());
	}
}

void LivingEntityManager::removeLivingEntityFromList(LivingEntity* entity)
{
	m_listLivingEntitys.erase(entity->getIdEntity());
	SAFE_DELETE(entity);
}

void LivingEntityManager::update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height)
{
	auto i = m_listLivingEntitys.end();
	if (i != m_listLivingEntitys.begin())
	{
		i--;
		while (true)
		{
			bool flag = false;
			std::map<int, LivingEntity*>::iterator j;

			if (i == m_listLivingEntitys.begin())
			{
				flag = true;
			}
			else
			{
				j = i;
				j--;
			}

			if (i->second->isDead())
			{
				if (i->second->getEntityType() == ENTITY_TYPE_PAWN) m_troopers[((Trooper*)(i->second))->getTrooperType()]->removeInstance(i->second->getInstance());
				removeLivingEntityFromList(i->second);
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

	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		i->second->update(userInput, timer, camera, width, height);

	for (int i=0; i<N_TROOPER_TYPE; i++) m_troopers[i]->update(timer);
}

void LivingEntityManager::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	for (int i = 0; i<N_TROOPER_TYPE; i++) m_troopers[i]->render(camera, &light);
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		i->second->render(camera, light, spriteBatch);
}

int LivingEntityManager::getNEntity()
{
	return m_listLivingEntitys.size();
}

LivingEntity* LivingEntityManager::getLivingEntityById(int id)
{
	auto result = m_listLivingEntitys.find(id);
	if (result != m_listLivingEntitys.end())
		return result->second;
	return NULL;
}

int LivingEntityManager::getIdLivingEntityInRange(int idEntity, float range)
{
	float minDistance = range;
	int result = -1;
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		if ((!i->second->isDead()) && (i->second->getIdEntity() != idEntity) && (m_listLivingEntitys[idEntity]->getTeamType() != i->second->getTeamType()))
			if (distance(m_listLivingEntitys[idEntity]->getInstance()->Position, i->second->getInstance()->Position) < minDistance)
			{
				result = i->second->getIdEntity();
				minDistance = distance(m_listLivingEntitys[idEntity]->getInstance()->Position, i->second->getInstance()->Position);
			}
	return result;
}

MyVec3 LivingEntityManager::checkLivingEntityCanMove(int idEntity)
{
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		if ((idEntity != i->second->getIdEntity()) && (twoPointIsContact(m_listLivingEntitys[idEntity]->getInstance()->Position, i->second->getInstance()->Position, 1.0f)))
		{
			return m_listLivingEntitys[idEntity]->getInstance()->Position - i->second->getInstance()->Position;
		}

	return MyVec3(0);
}

void LivingEntityManager::beatLivingEntitys(MyVec3 positionBeat, int dam)
{
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		if ((!i->second->isDead()) && (distance(positionBeat, i->second->getInstance()->Position) < 2.0f))
		{
			smartLog(toString(i->second->getIdEntity()) + " " + toString(dam));
			i->second->setHealth(i->second->getHealth() - dam);
		}
}