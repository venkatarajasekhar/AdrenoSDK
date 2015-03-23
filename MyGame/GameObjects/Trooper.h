#pragma once

#include <MySkinnedMesh2.h>
#include <MyTexture.h>
#include "EnemyAI.h"
#include "BloodBar.h"

const int MaxHealth = 100;

class Trooper
{
private:
	SkinnedMesh2::Instance* m_instance;
	EnemyAI m_ai;
	BloodBar* m_bloodBar;
	int m_id;
	int m_type;
	int m_team;
	int m_health;
	int m_dam;
	static int m_numIDs;
	bool m_isDeleted;
public:
	Trooper();
	void init(
		int type,
		int team,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		BloodBar* bloodBar);

	void update(Timer& timer);
	void render(Camera& camera, SpriteBatch& spriteBatch);
	void copyAllProperties();

	SkinnedMesh2::Instance* getTrooper();
	bool getIsDeleted();
	int getId();
	void setHealth(int helth);
	int getHealth();
	void setDam(int dam);
	int getDam();
};