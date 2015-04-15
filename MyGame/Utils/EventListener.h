
#pragma once

#include <MyUtils.h>
#include "BaseEntity.h"

//======================================================================================================
//
// Listeners
//
//======================================================================================================

class IOnGameOverListener
{
public:
	struct Data
	{
		TEAM_TYPE WinnerTeam;

		Data(TEAM_TYPE _winnerTeam)
			: WinnerTeam(_winnerTeam)
		{}
	};

public:
	virtual void OnGameOver(const Data& data) = 0;
};

//======================================================================================================
//
// Listenees
//
//======================================================================================================

class OnGameOverListenee
{
public:
	OnGameOverListenee();
	virtual ~OnGameOverListenee();

	virtual void addGameOverListener(IOnGameOverListener* listener);

protected:
	virtual void throwGameOverEvent(IOnGameOverListener::Data& data);

protected:
	std::vector<IOnGameOverListener*> m_gameOverListeners;
};