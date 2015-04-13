
#pragma once

#include "Hero.h"

class Hero_Controlled : public Hero, public IOnPressListener
{
public:
	Hero_Controlled();
	~Hero_Controlled();

	void OnPress(const IOnPressListener::Data& data);

private:

};