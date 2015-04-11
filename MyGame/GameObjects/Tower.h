#pragma once

#include <MySkinnedMesh1.h>
#include <MySpriteSheet.h>
#include <MyInput.h>
#include "Global.h"
#include "Projectile.h"


class Tower : public LivingEntity
{
private:
	FileMesh1 m_tower;
	FileMesh1::Instance* m_instance;
	BloodBar* m_bloodBar;
	Billboard m_billboard;
	int m_idEmemy = -1;
	float m_countTime = 3;
public:

	void init(
		Adreno::Model* model,
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
	int findLivingEntityToBeat();
	FileMesh1::Instance* getInstance();
};