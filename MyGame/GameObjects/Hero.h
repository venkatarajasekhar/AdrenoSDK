
#pragma once

#include "IRenderableEntity.h"
#include <MySkinnedMesh1.h>

#include "MovingEntity.h"

// Hero consists of state:
//	- Idle
//	- Walk
//	- Attack
//	- Skill attack
//	- Die

class Hero : public IRenderableEntity
{
public:
	Hero();
	virtual ~Hero();

	// Core functions
	virtual void init(
		Adreno::Model* model,
		Adreno::Animation* anim,
		Texture** modelTexture,
		Shader& shader,
		Material& material,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		std::map<MyString, SkinnedMesh1::AnimAction>& animActions,
		FLOAT32 speedFactor = 1.0f);
	virtual void update(UserInput& userInput, Timer& timer);
	virtual void render(Camera& camera, Light& light);

protected:
	// Mesh/Appearance elements
	SkinnedMesh1 m_skinnedMesh;
	SkinnedMesh1::Instance* m_skinnedMeshIns;

	// Moving elements
	MovingEntity m_movingEnt;
};