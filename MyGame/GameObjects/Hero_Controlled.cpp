
#include "Hero_Controlled.h"

//===================================================================================================================
//
// Hero_Controlled class
//
//===================================================================================================================

Hero_Controlled::Hero_Controlled()
{
	m_stateMachine = new StateMachine<Hero_Controlled>(this);
}

Hero_Controlled::~Hero_Controlled()
{
	SAFE_DELETE(m_stateMachine);
}

void Hero_Controlled::init(
	SkinnedMesh1& mesh,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	HeroProps& heroProp,
	HeroInGameProps& heroInGameProp,
	TEAM_TYPE team)
{
	// Moving elements
	m_movingEnt.init(
		heroInGameProp.Pos,
		heroInGameProp.Pos,
		heroInGameProp.Rot,
		heroProp.MovingRotYOffset,
		heroProp.MovingSpeed,
		heroProp.MovingTurnSpeed);

	// States manager
	m_stateMachine->SetCurrentState(Hero_ControlledState_Idle::instance());

	Hero::init(mesh, bloodBar, lEnts, heroProp, heroInGameProp, team);
}

void Hero_Controlled::update(Timer& timer)
{
	Hero::update(timer);

	// States manager
	m_stateMachine->Update();
}

void Hero_Controlled::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "map")
	{
		if (data.Tag != nullptr)
		{
			MyVec3* pos = (MyVec3*)data.Tag;
			m_movingEnt.setTarget(*pos);
		}
	}
}

#pragma region Hero_Controlled states

//===================================================================================================================
//
// Hero_Controlled state idle
//
//===================================================================================================================

void Hero_ControlledState_Idle::Enter(Hero_Controlled* hero)
{
	hero->m_instance->setAction("idle");
}

void Hero_ControlledState_Idle::Execute(Hero_Controlled* hero)
{
	if (hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(Hero_ControlledState_Walk::instance());
	}
	else
	{
		for (auto i = hero->m_lEnts->begin(); i != hero->m_lEnts->end(); ++i)
		{
			if ((hero != (*i)) &&
				((*i)->inUse()) &&
				(hero->getTeamType() != (*i)->getTeamType()) &&
				(distance_optimized(hero->getPos(), (*i)->getPos()) <= hero->m_atkRange))
			{
				hero->m_atkTarget = (*i);
				hero->m_stateMachine->ChangeState(Hero_ControlledState_Attack::instance());
				break;
			}
		}
	}
}

void Hero_ControlledState_Idle::Exit(Hero_Controlled* hero)
{
}

//===================================================================================================================
//
// Hero_Controlled state walk
//
//===================================================================================================================

void Hero_ControlledState_Walk::Enter(Hero_Controlled* hero)
{
	hero->m_instance->setAction("run");
}

void Hero_ControlledState_Walk::Execute(Hero_Controlled* hero)
{
	if (!hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(Hero_ControlledState_Idle::instance());
	}
}

void Hero_ControlledState_Walk::Exit(Hero_Controlled* hero)
{
}

//===================================================================================================================
//
// Hero_Controlled state attack
//
//===================================================================================================================

void Hero_ControlledState_Attack::Enter(Hero_Controlled* hero)
{
	if (hero->m_atkTarget != nullptr)
	{
		hero->m_movingEnt.turnTo(hero->m_atkTarget->getPos());
		hero->m_instance->setAction("attack_1", "", true, this, hero->m_time_PAA_Attack_1, hero);
	}
}

void Hero_ControlledState_Attack::Execute(Hero_Controlled* hero)
{
	if (hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(Hero_ControlledState_Walk::instance());
	}
	else
	{
		if (hero->m_atkTarget != nullptr)
		{
			if (
				(!hero->m_atkTarget->inUse()) ||
				(distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) > hero->m_atkRange)
				)
			{
				hero->m_stateMachine->ChangeState(Hero_ControlledState_Idle::instance());
			}
		}
	}
}

void Hero_ControlledState_Attack::Exit(Hero_Controlled* hero)
{
}

void Hero_ControlledState_Attack::OnPerformAAct(void* tag)
{
	if (tag != nullptr)
	{
		Hero_Controlled* hero = (Hero_Controlled*)tag;
		hero->m_atkTarget->accHealth(-hero->m_damage);
	}
}

#pragma endregion