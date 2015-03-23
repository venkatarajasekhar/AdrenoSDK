
#include "MyEventListener.h"

OnPressListenee::OnPressListenee()
{
}

OnPressListenee::~OnPressListenee()
{
}

void OnPressListenee::addPressListener(IOnPressListener* listener)
{
	if (listener == nullptr) return;

	bool existed(false);
	for (auto i = m_pressListeners.begin(); i != m_pressListeners.end(); ++i)
	{
		if ((*i) == listener)
		{
			existed = true;
			break;
		}
	}
	if (!existed)
	{
		m_pressListeners.push_back(listener);
	}
}

void OnPressListenee::throwPressEvent(IOnPressListener::Data& data)
{
	for (auto i = m_pressListeners.begin(); i != m_pressListeners.end(); ++i)
	{
		if ((*i) != nullptr)
		{
			(*i)->OnPress(data);
		}
	}
}