
#pragma once

#include "Hero.h"

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
		TEAM_TYPE team);

	void OnPress(const IOnPressListener::Data& data);

private:

};