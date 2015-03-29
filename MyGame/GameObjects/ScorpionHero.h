#pragma once

#include <MySkinnedMesh1.h>
#include <MySpriteSheet.h>
#include <MyInput.h>
#include "Global.h"
#include "Projectile.h"

class ScorpionHero : public LivingEntity
{
private:
	SkinnedMesh1 m_player;
	SkinnedMesh1::Instance* m_instance;
	BloodBar* m_bloodBar;
	Projectile m_projectile;

	MyVec3 m_pointTouch;
	MyVec3 m_direction = MyVec3(0, 0, 0);
	int m_idEmemy = -1;
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
		BloodBar* bloodBarRed,
		BloodBar* bloodBarGreen,
		Billboard& billboard,
		int health,
		int damage,
		float range,
		TEAM_TYPE teamType);

	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	void rotatePlayer(MyVec3 pointDestination);
	int findLivingEntityToBeat();
	void projectile();
	SkinnedMesh1::Instance* getInstance();
};