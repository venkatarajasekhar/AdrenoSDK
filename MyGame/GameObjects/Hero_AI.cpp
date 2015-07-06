
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
	Quad3D& selectedDecal,
	const std::vector<MyVec3>& path,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
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

	m_isEmptyHealth = false;

	Hero::init(mesh, bloodBar, selectedDecal, lEnts, lAudios, heroProp, heroInGameProp, team);
}

void Hero_AI::update(Timer& timer)
{
	Hero::update(timer);

	if ((!m_isEmptyHealth) && (m_health <= 2 * m_maxHealth / 5.0))
	{
		m_isEmptyHealth = true;
		m_atkTarget = nullptr;
		m_movingEnt.setPathPivot(m_movingEnt.getPathPivot() - 1);
		m_movingEnt.setDeltaPathPivot(-1);
		m_movingEnt.reFollowPath();

		m_stateMachine->ChangeState(Hero_AIState_Walk::instance());
	}

	if ((m_isEmptyHealth) && (abs(m_health - m_maxHealth) <= 0.1))
	{
		m_isEmptyHealth = false;
		m_movingEnt.setDeltaPathPivot(1);
		m_movingEnt.setPathPivot(1);
	}

	// States manager
	m_stateMachine->Update();
}

void Hero_AI::dead()
{
	m_audios[AUDIO_ENEMYHERO_DEATH]->play();
	Hero::dead();
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
	//if (!hero->m_isEmptyHealth)
	//{
		if (!hero->m_movingEnt.isMoving())
		{
			hero->m_stateMachine->ChangeState(Hero_AIState_Idle::instance());
		}
		else
		{
			if (!hero->m_isEmptyHealth)
			{
				for (auto i = hero->m_lEnts->begin(); i != hero->m_lEnts->end(); ++i)
				{
					if ((hero != (*i)) &&
						((*i)->inUse()) &&
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
	//}
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
		if (
			(!hero->m_atkTarget->inUse()) ||
			(distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) > hero->m_chasingRange)
			)
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
	hero->turnToTarget();
}

void Hero_AIState_Attack::Execute(Hero_AI* hero)
{
	if (hero->m_atkTarget != nullptr)
	{
		if (!hero->m_atkTarget->inUse())
		{
			hero->m_movingEnt.reFollowPath();
			hero->m_stateMachine->ChangeState(Hero_AIState_Walk::instance());
		}
		else
		{
			if (distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) > hero->m_atkRange)
			{
				hero->m_stateMachine->ChangeState(Hero_AIState_Chase::instance());
			}
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

		if (hero->m_atkTarget->getHealth() - hero->m_damage <= 0)
		{
			if (hero->m_atkTarget->getEntityType() == ENTITY_TYPE_PAWN)
			{
				hero->m_gold += MONEY_PAWN;
				hero->m_exp += EXP_PAWN;
			}
			if (hero->m_atkTarget->getEntityType() == ENTITY_TYPE_TOWER)
			{
				hero->m_gold += MONEY_TOWER;
				hero->m_exp += EXP_TOWER;
			}
		}

		//hero->m_audios[hero->AUDIO_ENEMYHERO_ATTACK]->play();
		hero->m_audios[hero->AUDIO_ENEMYHERO_ATTACK2]->play();

		hero->m_atkTarget->accHealth(-hero->m_damage);
		hero->accHealth(hero->m_healthPerAttack);
	}
}

#pragma endregion