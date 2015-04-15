
#include "EventListener.h"

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