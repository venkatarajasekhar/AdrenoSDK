
#include "Hero_Controlled.h"
#include "Hero.h"

//===================================================================================================================
//
// Hero_Controlled class
//
//===================================================================================================================

Hero_Controlled::Hero_Controlled()
	: m_usingSkill(-1)
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
	Quad3D& selectedDecal,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
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

	Hero::init(mesh, bloodBar, selectedDecal, lEnts, lAudios, heroProp, heroInGameProp, team);

	m_notifyPool.init();
}

void Hero_Controlled::update(Timer& timer)
{
	Hero::update(timer);

	for (auto i = m_lEnts->begin(); i != m_lEnts->end(); ++i)
	{
		if ((*i)->getSelected() && ((*i)->getTeamType() == TEAM_TYPE_ENEMY))
		{
			//if ((m_atkTarget != nullptr) && (&m_atkTarget != &(*i))) turnToTarget();
			m_atkTarget = (*i);
			
			if (distance_optimized(getPos(), (*i)->getPos()) > m_atkRange)
			{
				MyVec3 u = (*i)->getPos() - getPos();
				u = normalize_optimized(u);
				m_movingEnt.setTarget((*i)->getPos() - MyVec3(2)*u);
			}
		}
	}

	// States manager
	m_stateMachine->Update();
	m_notifyPool.update(timer);
}

void Hero_Controlled::render(SpriteBatch& spriteBatch, Camera& camera, Light& light)
{
	Hero::render(spriteBatch, camera, light);
	m_notifyPool.render(camera, spriteBatch);
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

void Hero_Controlled::useSkill(int skillID)
{
	m_usingSkill = skillID;
	m_stateMachine->ChangeState(Hero_ControlledState_SkillAttack::instance());
}

void Hero_Controlled::dead()
{
	m_audios[AUDIO_MYHERO_DEATH].play();
	Hero::dead();
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
		hero->turnToTarget();
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
	Notify* data;
	if (tag != nullptr)
	{
		Hero_Controlled* hero = (Hero_Controlled*)tag;
		if (hero->m_atkTarget->getHealth() - hero->m_damage <= 0)
		{
			MyVec3 pos = hero->m_atkTarget->getPos();
			MyVec3 offset = MyVec3(-1, 3, 0);

			if (hero->m_atkTarget->getEntityType() == ENTITY_TYPE_PAWN)
			{
				hero->m_gold += MONEY_PAWN;
				hero->m_exp += EXP_PAWN;
				hero->m_notifyPool.spawnNotify("+" + toString(MONEY_PAWN), pos + offset, MyVec2(0, -0.5f), 1.5f);
			}
			if (hero->m_atkTarget->getEntityType() == ENTITY_TYPE_TOWER)
			{
				hero->m_gold += MONEY_TOWER;
				hero->m_exp += EXP_TOWER;
				hero->m_notifyPool.spawnNotify("+" + toString(MONEY_PAWN), pos + offset, MyVec2(0, -0.5f), 1.5f);
			}
		}

		hero->m_audios[hero->AUDIO_MYHERO_ATTACK].play();

		hero->m_atkTarget->accHealth(-hero->m_damage);
		hero->accHealth(hero->m_healthPerAttack);
	}
}

//===================================================================================================================
//
// Hero_Controlled state skill attack
//
//===================================================================================================================

void Hero_ControlledState_SkillAttack::Enter(Hero_Controlled* hero)
{
	switch (hero->m_usingSkill)
	{
	case 1:
		hero->m_instance->setAction("attack_2", "idle", false, this, 0.95f, hero);
		break;
	case 2:
		hero->m_instance->setAction("attack_3", "idle", false, this, 0.95f, hero);
		break;
	case 3:
		hero->m_instance->setAction("attack_4", "idle", false, this, 0.95f, hero);
		break;
	default:
		break;
	}

	
}

void Hero_ControlledState_SkillAttack::Execute(Hero_Controlled* hero)
{

}

void Hero_ControlledState_SkillAttack::Exit(Hero_Controlled* hero)
{

}

void Hero_ControlledState_SkillAttack::OnPerformAAct(void* tag)
{
	if (tag != nullptr)
	{
		Hero_Controlled* hero = (Hero_Controlled*)tag;
		hero->m_stateMachine->ChangeState(Hero_ControlledState_Idle::instance());
	}
}

#pragma endregion