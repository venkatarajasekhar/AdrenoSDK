
#include "Hero_AI.h"

//===================================================================================================================
//
// Hero_AI class
//
//===================================================================================================================

Hero_AI::Hero_AI()
{
	m_stateMachine = new StateMachine<Hero_AI>(this);
}

Hero_AI::~Hero_AI()
{
	SAFE_DELETE(m_stateMachine);
}

void Hero_AI::init(
	SkinnedMesh1& mesh,
	BloodBar& bloodBar,
	const std::vector<MyVec3>& path,
	std::vector<LivingEntity*>& lEnts,
	HeroProps& heroProp,
	TEAM_TYPE team)
{
	// Moving elements
	m_movingEnt.init(
		path,
		heroProp.MovingRotYOffset,
		heroProp.MovingSpeed,
		heroProp.MovingTurnSpeed);

	// States manager
	m_stateMachine->SetCurrentState(Hero_AIState_Idle::instance());

	Hero::init(mesh, bloodBar, lEnts, heroProp, team);
}

void Hero_AI::update(Timer& timer)
{
	Hero::update(timer);

	// States manager
	m_stateMachine->Update();
}

//===================================================================================================================
//
// Hero_AI state idle
//
//===================================================================================================================

void Hero_AIState_Idle::Enter(Hero_AI* hero)
{
	hero->m_instance->setAction("idle");
}

void Hero_AIState_Idle::Execute(Hero_AI* hero)
{
	if (hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(Hero_AIState_Walk::instance());
	}
}

void Hero_AIState_Idle::Exit(Hero_AI* hero)
{
}

//===================================================================================================================
//
// Hero_Controlled state walk
//
//===================================================================================================================

void Hero_AIState_Walk::Enter(Hero_AI* hero)
{
	hero->m_instance->setAction("run");
}

void Hero_AIState_Walk::Execute(Hero_AI* hero)
{
	if (!hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(Hero_AIState_Idle::instance());
	}
}

void Hero_AIState_Walk::Exit(Hero_AI* hero)
{
}

//===================================================================================================================
//
// Hero_Controlled state attack
//
//===================================================================================================================

void Hero_AIState_Attack::Enter(Hero_AI* hero)
{
}

void Hero_AIState_Attack::Execute(Hero_AI* hero)
{
}

void Hero_AIState_Attack::Exit(Hero_AI* hero)
{
}