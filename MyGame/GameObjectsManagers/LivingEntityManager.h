#pragma once

#include <MyUtils.h>
#include <MyCamera.h>
#include "Trooper.h"

/*enum TROOPER_TYPE
{
	SCORPION,
	N_TROOPER_TYPE,
};*/

class LivingEntityManager
{
private:
	SkinnedMesh1* m_troopers[1];
	std::map<int, LivingEntity*> m_listLivingEntitys;
public:
	LivingEntityManager();
	~LivingEntityManager();
	/*void init(
		int type,
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader);*/
	void init(
		SkinnedMesh1* scorpion);
	void insertLivingEntityToList(LivingEntity* entity, int typeEntity);
	void removeLivingEntityFromList(LivingEntity* entity);
	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	int getNEntity();
	LivingEntity* getLivingEntityById(int id);
	int getIdLivingEntityInRange(int idEntity, float range);
	MyVec3 checkLivingEntityCanMove(int idEntity);
	void beatLivingEntitys(MyVec3 positionBeat, int dam);
	//MyVec3 obstacleAvoidance(int idEntity);
};