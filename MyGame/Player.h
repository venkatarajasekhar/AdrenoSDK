#pragma once

#include <MySkinnedMesh2.h>
#include <MyInput.h>
#include "Global.h"

class Player
{
private:
	SkinnedMesh2 m_player;
	SkinnedMesh2::Instance* m_instance;

	MyVec3 m_pointTouch;
	MyVec3 m_direction = MyVec3(0, 0, 0);
	int m_helth;
	int m_dam;
public:

	void init(
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale);

	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light);
	void rotatePlayer(MyVec3 pointDestination);
	void setHelth(int helth);
	int getHelth();
	void setDam(int dam);
	int getDam();
	int findTrooperToBeat();
};