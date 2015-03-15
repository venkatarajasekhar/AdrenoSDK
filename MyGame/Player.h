#pragma once

#include <MySkinnedMesh2.h>
#include <MyInput.h>
#include "Global.h"

const int MaxHealthPlayer = 1000;

class Player
{
private:
	SkinnedMesh2 m_player;
	SkinnedMesh2::Instance* m_instance;
	BloodBar* m_bloodBar;

	MyVec3 m_pointTouch;
	MyVec3 m_direction = MyVec3(0, 0, 0);
	int m_health;
	int m_dam;
	float m_countTime = 0;
public:

	void init(
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		BloodBar* bloodBar);

	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	void rotatePlayer(MyVec3 pointDestination);
	void setHealth(int health);
	int getHealth();
	void setDam(int dam);
	int getDam();
	int findTrooperToBeat();
};