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
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		if (i->second->getIdEntity() == id)
		{
			return i->second;
		}
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

bool LivingEntityManager::checkLivingEntityCanMove(MyVec3 positionTrooper, int type)
{
	int count = 0;
	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
		if (twoPointIsContact(positionTrooper, i->second->getInstance()->Position, 0.6f))
		{
			count++;
			if (count == 2)
				return false;
		}

	return true;
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

//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
/*MyVec3 LivingEntityManager::obstacleAvoidance(int idEntity)
{
	//the detection box length is proportional to the agent's velocity
	float m_dDBoxLength = m_listLivingEntitys[idEntity]->getRadius() + (0.05f / 0.1f) * m_listLivingEntitys[idEntity]->getRadius();

	//this will keep track of the closest intersecting obstacle (CIB)
	LivingEntity* ClosestIntersectingObstacle = NULL;

	//this will be used to track the distance to the CIB
	float DistToClosestIP = 1000000000.0f;

	//this will record the transformed local coordinates of the CIB
	MyVec3 LocalPosOfClosestObstacle;

	for (auto i = m_listLivingEntitys.begin(); i != m_listLivingEntitys.end(); i++)
	{
		//if the obstacle has been tagged within range proceed
		if (distance(m_listLivingEntitys[idEntity]->getInstance()->Position, i->second->getInstance()->Position) <= m_dDBoxLength)
		{
			//calculate this obstacle's position in local space

			MyVec3 LocalPos = i->second->getInstance()->Position;

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPos.z >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				double ExpandedRadius = i->second->getRadius() + m_listLivingEntitys[idEntity]->getRadius();

				if (fabs(LocalPos.x) < ExpandedRadius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					double cZ = LocalPos.z;
					double cX = LocalPos.x;

					//we only need to calculate the sqrt part of the above equation once
					double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cX*cX);

					double ip = cZ - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cZ + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < DistToClosestIP)
					{
						DistToClosestIP = ip;

						ClosestIntersectingObstacle = i->second;

						LocalPosOfClosestObstacle = LocalPos;
					}
				}
			}
		}
	}

	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	MyVec3 SteeringForce;

	if (ClosestIntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		double multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.z) /
			m_dDBoxLength;

		//calculate the lateral force
		SteeringForce.x = (ClosestIntersectingObstacle->getRadius() - LocalPosOfClosestObstacle.x) * multiplier;

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const double BrakingWeight = 0.2;

		SteeringForce.z = (ClosestIntersectingObstacle->getRadius() - LocalPosOfClosestObstacle.z) * BrakingWeight;
	}

	//finally, convert the steering vector from local to world space
	return VectorToWorldSpace(SteeringForce, m_pVehicle->Heading(), m_pVehicle->Side());
}*/