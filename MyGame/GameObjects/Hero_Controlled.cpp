
#include "Hero_Controlled.h"

Hero_Controlled::Hero_Controlled()
{
}

Hero_Controlled::~Hero_Controlled()
{
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