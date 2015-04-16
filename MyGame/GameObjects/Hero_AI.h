
#pragma once

#include "Hero.h"

class Hero_AI : public Hero
{
public:
	Hero_AI();
	~Hero_AI();

	void init(
		SkinnedMesh1& mesh,
		BloodBar& bloodBar,
		const std::vector<MyVec3>& path,
		std::vector<LivingEntity*>& lEnts,
		HeroProps& heroProp,
		TEAM_TYPE team);

private:

};