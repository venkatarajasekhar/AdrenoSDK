
#include "Hero_Controlled.h"

Hero_Controlled::Hero_Controlled()
{
}

Hero_Controlled::~Hero_Controlled()
{
}

void Hero_Controlled::init(
	SkinnedMesh1& mesh,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	HeroProps& heroProp,
	TEAM_TYPE team)
{
	// Moving elements
	m_movingEnt.init(
		heroProp.InitialPos,
		heroProp.InitialPos,
		heroProp.InitialRot,
		heroProp.MovingRotYOffset,
		heroProp.MovingSpeed,
		heroProp.MovingTurnSpeed);

	Hero::init(mesh, bloodBar, lEnts, heroProp, team);
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