#pragma once

#include <MySkinnedMesh1.h>
#include <MyTexture.h>
#include "EnemyAI.h"
#include "BloodBar.h"
#include "LivingEntity.h"
#include "Global.h"

/*enum TROOPER_TYPE
{
	TROOPER_SCORPION,
	N_TROOPER_TYPE,
};*/

class Trooper : public LivingEntity
{
private:
	SkinnedMesh1::Instance* m_instance;
	EnemyAI m_ai;
	BloodBar* m_bloodBar;
	int m_trooperType;
public:
	Trooper();
	void init(
		int trooperType,
		TEAM_TYPE teamType,
		int health,
		int damage,
		float range,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		BloodBar* bloodBarRed,
		BloodBar* bloodBarGreen);

	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	void copyAllProperties();

	SkinnedMesh1::Instance* getInstance();
	int getTrooperType();
};