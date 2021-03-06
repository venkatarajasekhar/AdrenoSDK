
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
		Quad3D& selectedDecal,
		const std::vector<MyVec3>& path,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		HeroProps& heroProp,
		HeroInGameProps& heroInGameProp,
		TEAM_TYPE team);

	void update(Timer& timer);
	void beginMatch();

protected:
	virtual void dead();

private:
	// States manager
	StateMachine<Hero_AI>* m_stateMachine;

	float m_chasingRange;
	bool m_isEmptyHealth;

private:
	friend class Hero_AIState_Idle;
	friend class Hero_AIState_Walk;
	friend class Hero_AIState_Chase;
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

class Hero_AIState_Chase : public State<Hero_AI>
{
private:
	Hero_AIState_Chase(){}
	Hero_AIState_Chase(const Hero_AIState_Chase&);
	Hero_AIState_Chase& operator=(const Hero_AIState_Chase&);

public:
	static Hero_AIState_Chase* instance(){ static Hero_AIState_Chase ins; return &ins; }

public:
	virtual void Enter(Hero_AI* hero);
	virtual void Execute(Hero_AI* hero);
	virtual void Exit(Hero_AI* hero);
};

class Hero_AIState_Attack : public State<Hero_AI>, public SkinnedMesh1::IOnPerformAActListener
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

	void OnPerformAAct(void* tag);
};