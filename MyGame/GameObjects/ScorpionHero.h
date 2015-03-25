#pragma once

#include <MySkinnedMesh1.h>
#include <MySpriteSheet.h>
#include <MyInput.h>
#include "Global.h"
#include "Projectile.h"

const int MaxHealthScorpion = 1000;

class ScorpionHero
{
private:
	SkinnedMesh1 m_player;
	SkinnedMesh1::Instance* m_instance;
	BloodBar* m_bloodBar;
	Projectile m_projectile;

	MyVec3 m_pointTouch;
	MyVec3 m_direction = MyVec3(0, 0, 0);
	int m_idEmemy = -1;
	float m_range = 2.0f;
	int m_health;
	int m_dam;
	float m_countTime = 0;
	bool m_isUsingSkill = false;
	float m_countTimeSkill = 0;
public:

	void init(
		Adreno::Model* model,
		Adreno::Animation* animation,
		FileMesh1::MeshTextures& textures,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		BloodBar* bloodBar,
		Billboard& billboard);

	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	void rotatePlayer(MyVec3 pointDestination);
	void setHealth(int health);
	int getHealth();
	void setDam(int dam);
	int getDam();
	int findTrooperToBeat();
	void projectile();
};