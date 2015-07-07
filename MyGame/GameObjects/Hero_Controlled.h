
#pragma once

#include "Hero.h"
#include "StateMachine.h"
#include "Notify.h"

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
		Quad3D& selectedDecal,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		HeroProps& heroProp,
		HeroInGameProps& heroInGameProp,
		TEAM_TYPE team);
	void update(Timer& timer);
	void render(SpriteBatch& spriteBatch, Camera& camera, Light& light);

	void OnPress(const IOnPressListener::Data& data);

	void useSkill(int skillID);

protected:
	virtual void dead();

private:
	// States manager
	StateMachine<Hero_Controlled>* m_stateMachine;
	NotifyPool m_notifyPool;
	int m_usingSkill;

private:
	friend class Hero_ControlledState_Idle;
	friend class Hero_ControlledState_Walk;
	friend class Hero_ControlledState_Attack;
	friend class Hero_ControlledState_SkillAttack;
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

class Hero_ControlledState_SkillAttack : public State<Hero_Controlled>, public SkinnedMesh1::IOnPerformAActListener
{
private:
	Hero_ControlledState_SkillAttack(){}
	Hero_ControlledState_SkillAttack(const Hero_ControlledState_SkillAttack&);
	Hero_ControlledState_SkillAttack& operator=(const Hero_ControlledState_SkillAttack&);

public:
	static Hero_ControlledState_SkillAttack* instance(){ static Hero_ControlledState_SkillAttack ins; return &ins; }

public:
	virtual void Enter(Hero_Controlled* hero);
	virtual void Execute(Hero_Controlled* hero);
	virtual void Exit(Hero_Controlled* hero);

	void OnPerformAAct(void* tag);
};