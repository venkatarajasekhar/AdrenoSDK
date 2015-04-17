
#include "Hero_AI.h"

//===================================================================================================================
//
// Hero_AI class
//
//===================================================================================================================

Hero_AI::Hero_AI()
	: m_chasingRange(0)
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
	HeroInGameProps& heroInGameProp,
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

	m_chasingRange = heroProp.ChasingRange;

	Hero::init(mesh, bloodBar, lEnts, heroProp, heroInGameProp, team);
}

void Hero_AI::update(Timer& timer)
{
	Hero::update(timer);

	// States manager
	m_stateMachine->Update();
}

#pragma region Hero_AI states

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
	else
	{
		for (auto i = hero->m_lEnts->begin(); i != hero->m_lEnts->end(); ++i)
		{
			if ((hero != (*i)) &&
				(hero->getTeamType() != (*i)->getTeamType()) &&
				(distance_optimized(hero->getPos(), (*i)->getPos()) <= hero->m_chasingRange))
			{
				hero->m_atkTarget = (*i);
				hero->m_stateMachine->ChangeState(Hero_AIState_Chase::instance());
				break;
			}
		}
	}
}

void Hero_AIState_Walk::Exit(Hero_AI* hero)
{
}

//===================================================================================================================
//
// Hero_Controlled state chase
//
//===================================================================================================================

void Hero_AIState_Chase::Enter(Hero_AI* hero)
{
	if (hero->m_instance->CurrentAction != "run")
	{
		hero->m_instance->setAction("run");
	}
	hero->m_movingEnt.disFollowPath();
}

void Hero_AIState_Chase::Execute(Hero_AI* hero)
{
	if (hero->m_atkTarget != nullptr)
	{
		if (distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) > hero->m_chasingRange)
		{
			hero->m_movingEnt.reFollowPath();
			hero->m_stateMachine->ChangeState(Hero_AIState_Walk::instance());
		}
		else
		{
			if (distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) <= hero->m_atkRange)
			{
				hero->m_stateMachine->ChangeState(Hero_AIState_Attack::instance());
			}
			else
			{
				hero->m_movingEnt.setTarget(hero->m_atkTarget->getPos());
			}
		}
	}
}

void Hero_AIState_Chase::Exit(Hero_AI* hero)
{
}

//===================================================================================================================
//
// Hero_Controlled state attack
//
//===================================================================================================================

void Hero_AIState_Attack::Enter(Hero_AI* hero)
{
	hero->m_instance->setAction("attack_1", "", true, this, hero->m_time_PAA_Attack_1, hero);
	hero->m_movingEnt.setTarget(hero->getPos());
}

void Hero_AIState_Attack::Execute(Hero_AI* hero)
{
	if (hero->m_atkTarget != nullptr)
	{
		if (distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) > hero->m_atkRange)
		{
			hero->m_stateMachine->ChangeState(Hero_AIState_Chase::instance());
		}
	}
}

void Hero_AIState_Attack::Exit(Hero_AI* hero)
{
}

void Hero_AIState_Attack::OnPerformAAct(void* tag)
{
	if (tag != nullptr)
	{
		Hero_AI* hero = (Hero_AI*)tag;
		hero->m_atkTarget->accHealth(-hero->m_damage);
	}
}

#pragma endregion