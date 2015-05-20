
#pragma once

#include <MyUtils.h>
#include "BaseEntity.h"
#include "HeroItem.h"

//======================================================================================================
//
// Game over event
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

//======================================================================================================
//
// Buy an item event
//
//======================================================================================================

class IOnBuyItemListener
{
public:
	struct Data
	{
		// ID of listenee
		MyString Id;

		// Bought item
		HeroItem* BoughtItem;

		Data(const MyString& _id, HeroItem* _boughtItem)
			: Id(_id),
			BoughtItem(_boughtItem)
		{}
	};

public:
	virtual void OnBuyItemItem(const Data& data) = 0;
};

class OnBuyItemListenee
{
public:
	OnBuyItemListenee(){}
	virtual ~OnBuyItemListenee(){}

	virtual void addBuyItemListener(IOnBuyItemListener* listener);

protected:
	virtual void throwBuyItemEvent(IOnBuyItemListener::Data& data);

protected:
	std::vector<IOnBuyItemListener*> m_buyItemListeners;
};