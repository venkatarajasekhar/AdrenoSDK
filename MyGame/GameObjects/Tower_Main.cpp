
#include "Tower_Main.h"

Tower_Main::Tower_Main()
{
}

Tower_Main::~Tower_Main()
{
}

void Tower_Main::dead()
{
	{
		TEAM_TYPE winner(TEAM_TYPE_UNDEFINED);
		if (getTeamType() == TEAM_TYPE_MY_TEAM)
		{
			winner = TEAM_TYPE_ENEMY;
		}
		else if (getTeamType() == TEAM_TYPE_ENEMY)
		{
			winner = TEAM_TYPE_MY_TEAM;
		}

		IOnGameOverListener::Data data(winner);
		throwGameOverEvent(data);
	}

	LivingEntity::dead();
}