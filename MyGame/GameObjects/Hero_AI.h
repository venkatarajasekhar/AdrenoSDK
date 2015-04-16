
#pragma once

#include "Hero.h"
#include "StateMachine.h"

//===================================================================================================================
//
// Hero_AI class
//
//===================================================================================================================

class Hero_AI : public Hero
{
public:
	Hero_AI();
	~Hero_AI();

	void init(
		SkinnedMesh1& mesh,
		BloodBar& bloodBar,
		const std::vector<MyVec3>& path,
		std::vector<LivingEntity*>& lEnts,
		HeroProps& heroProp,
		TEAM_TYPE team);

	void update(Timer& timer);

private:
	// States manager
	StateMachine<Hero_AI>* m_stateMachine;

private:
	friend class Hero_AIState_Idle;
	friend class Hero_AIState_Walk;
	friend class Hero_AIState_Attack;
};

//===================================================================================================================
//
// Hero_AI states
//
//===================================================================================================================

class Hero_AIState_Idle : public State<Hero_AI>
{
private:
	Hero_AIState_Idle(){}
	Hero_AIState_Idle(const Hero_AIState_Idle&);
	Hero_AIState_Idle& operator=(const Hero_AIState_Idle&);

public:
	static Hero_AIState_Idle* instance(){ static Hero_AIState_Idle ins; return &ins; }

public:
	virtual void Enter(Hero_AI* hero);
	virtual void Execute(Hero_AI* hero);
	virtual void Exit(Hero_AI* hero);
};

class Hero_AIState_Walk : public State<Hero_AI>
{
private:
	Hero_AIState_Walk(){}
	Hero_AIState_Walk(const Hero_AIState_Walk&);
	Hero_AIState_Walk& operator=(const Hero_AIState_Walk&);

public:
	static Hero_AIState_Walk* instance(){ static Hero_AIState_Walk ins; return &ins; }

public:
	virtual void Enter(Hero_AI* hero);
	virtual void Execute(Hero_AI* hero);
	virtual void Exit(Hero_AI* hero);
};

class Hero_AIState_Attack : public State<Hero_AI>
{
private:
	Hero_AIState_Attack(){}
	Hero_AIState_Attack(const Hero_AIState_Attack&);
	Hero_AIState_Attack& operator=(const Hero_AIState_Attack&);

public:
	static Hero_AIState_Attack* instance(){ static Hero_AIState_Attack ins; return &ins; }

public:
	virtual void Enter(Hero_AI* hero);
	virtual void Execute(Hero_AI* hero);
	virtual void Exit(Hero_AI* hero);
};