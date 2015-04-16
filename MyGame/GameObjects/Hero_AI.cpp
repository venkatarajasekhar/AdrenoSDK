
#include "Hero_AI.h"

Hero_AI::Hero_AI()
{
}

Hero_AI::~Hero_AI()
{
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

	Hero::init(mesh, bloodBar, lEnts, heroProp, team);
}