
#pragma once

#include "IRenderableEntity.h"
#include <MySkinnedMesh1.h>

#include "MovingEntity.h"
#include "StateMachine.h"

//===================================================================================================================
//
// Hero consists of state:
//	- Idle
//	- Walk
//	- Attack
//	- Skill attack
//	- Die
//
//===================================================================================================================

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

	StateMachine<Hero>* getFSM()const;
	MyVec3 getPos();

	// Controlling hero
	void changeAnimAction(const MyString& action);
	bool isMoving();

protected:
	// Mesh/Appearance elements
	SkinnedMesh1 m_skinnedMesh;
	SkinnedMesh1::Instance* m_skinnedMeshIns;

	// Moving elements
	MovingEntity m_movingEnt;

	// States manager
	StateMachine<Hero>* m_stateMachine;
};

//===================================================================================================================
//
// Hero states
//
//===================================================================================================================

class HeroState_Idle : public State<Hero>
{
private:
	HeroState_Idle(){}
	HeroState_Idle(const HeroState_Idle&);
	HeroState_Idle& operator=(const HeroState_Idle&);

public:
	static HeroState_Idle* instance(){ static HeroState_Idle ins; return &ins; }

public:
	virtual void Enter(Hero* hero);
	virtual void Execute(Hero* hero);
	virtual void Exit(Hero* hero);
};

class HeroState_Walk : public State<Hero>
{
private:
	HeroState_Walk(){}
	HeroState_Walk(const HeroState_Walk&);
	HeroState_Walk& operator=(const HeroState_Walk&);

public:
	static HeroState_Walk* instance(){ static HeroState_Walk ins; return &ins; }

public:
	virtual void Enter(Hero* hero);
	virtual void Execute(Hero* hero);
	virtual void Exit(Hero* hero);
};