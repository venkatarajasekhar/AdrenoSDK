
#include "EventListener.h"

//======================================================================================================
//
// Game over event
//
//======================================================================================================

OnGameOverListenee::OnGameOverListenee()
{
}

OnGameOverListenee::~OnGameOverListenee()
{
}

void OnGameOverListenee::addGameOverListener(IOnGameOverListener* listener)
{
	if (listener == nullptr) return;

	bool existed(false);
	for (auto i = m_gameOverListeners.begin(); i != m_gameOverListeners.end(); ++i)
	{
		if ((*i) == listener)
		{
			existed = true;
			break;
		}
	}
	if (!existed)
	{
		m_gameOverListeners.push_back(listener);
	}
}

void OnGameOverListenee::throwGameOverEvent(IOnGameOverListener::Data& data)
{
	for (auto i = m_gameOverListeners.begin(); i != m_gameOverListeners.end(); ++i)
	{
		if ((*i) != nullptr)
		{
			(*i)->OnGameOver(data);
		}
	}
}

//======================================================================================================
//
// Buy an item event
//
//======================================================================================================

void OnBuyItemListenee::addBuyItemListener(IOnBuyItemListener* listener)
{
	if (listener == nullptr) return;

	bool existed(false);
	for (auto i = m_buyItemListeners.begin(); i != m_buyItemListeners.end(); ++i)
	{
		if ((*i) == listener)
		{
			existed = true;
			break;
		}
	}
	if (!existed)
	{
		m_buyItemListeners.push_back(listener);
	}
}

void OnBuyItemListenee::throwBuyItemEvent(IOnBuyItemListener::Data& data)
{
	for (auto i = m_buyItemListeners.begin(); i != m_buyItemListeners.end(); ++i)
	{
		if ((*i) != nullptr)
		{
			(*i)->OnBuyItemItem(data);
		}
	}
}