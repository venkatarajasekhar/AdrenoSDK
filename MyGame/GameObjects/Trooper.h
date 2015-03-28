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
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		BloodBar* bloodBar);

	void update(Timer& timer);
	void render(Camera& camera, SpriteBatch& spriteBatch);
	void copyAllProperties();

	SkinnedMesh1::Instance* getInstance();
	int getTrooperType();
};