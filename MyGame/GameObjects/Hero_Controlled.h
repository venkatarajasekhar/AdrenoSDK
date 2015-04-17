
#pragma once

#include "Hero.h"
#include "StateMachine.h"

//===================================================================================================================
//
// Hero_Controlled class
//
//===================================================================================================================

class Hero_Controlled : public Hero, public IOnPressListener
{
public:
	Hero_Controlled();
	~Hero_Controlled();

	void init(
		SkinnedMesh1& mesh,
		BloodBar& bloodBar,
		std::vector<LivingEntity*>& lEnts,
		HeroProps& heroProp,
		HeroInGameProps& heroInGameProp,
		TEAM_TYPE team);
	void update(Timer& timer);

	void OnPress(const IOnPressListener::Data& data);

private:
	// States manager
	StateMachine<Hero_Controlled>* m_stateMachine;

private:
	friend class Hero_ControlledState_Idle;
	friend class Hero_ControlledState_Walk;
	friend class Hero_ControlledState_Attack;
};

//===================================================================================================================
//
// Hero_Controlled states
//
//===================================================================================================================

class Hero_ControlledState_Idle : public State<Hero_Controlled>
{
private:
	Hero_ControlledState_Idle(){}
	Hero_ControlledState_Idle(const Hero_ControlledState_Idle&);
	Hero_ControlledState_Idle& operator=(const Hero_ControlledState_Idle&);

public:
	static Hero_ControlledState_Idle* instance(){ static Hero_ControlledState_Idle ins; return &ins; }

public:
	virtual void Enter(Hero_Controlled* hero);
	virtual void Execute(Hero_Controlled* hero);
	virtual void Exit(Hero_Controlled* hero);
};

class Hero_ControlledState_Walk : public State<Hero_Controlled>
{
private:
	Hero_ControlledState_Walk(){}
	Hero_ControlledState_Walk(const Hero_ControlledState_Walk&);
	Hero_ControlledState_Walk& operator=(const Hero_ControlledState_Walk&);

public:
	static Hero_ControlledState_Walk* instance(){ static Hero_ControlledState_Walk ins; return &ins; }

public:
	virtual void Enter(Hero_Controlled* hero);
	virtual void Execute(Hero_Controlled* hero);
	virtual void Exit(Hero_Controlled* hero);
};

class Hero_ControlledState_Attack : public State<Hero_Controlled>, public SkinnedMesh1::IOnPerformAActListener
{
private:
	Hero_ControlledState_Attack(){}
	Hero_ControlledState_Attack(const Hero_ControlledState_Attack&);
	Hero_ControlledState_Attack& operator=(const Hero_ControlledState_Attack&);

public:
	static Hero_ControlledState_Attack* instance(){ static Hero_ControlledState_Attack ins; return &ins; }

public:
	virtual void Enter(Hero_Controlled* hero);
	virtual void Execute(Hero_Controlled* hero);
	virtual void Exit(Hero_Controlled* hero);

	void OnPerformAAct(void* tag);
};