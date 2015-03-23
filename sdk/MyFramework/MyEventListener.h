
#pragma once

#include "MyUtils.h"

//======================================================================================================
//
// Listeners
//
//======================================================================================================

class IOnPressListener
{
public:
	struct Data
	{
		MyString Id;
		int x;
		int y;
	};

public:
	virtual void OnPress(const Data& data) = 0;
};

//======================================================================================================
//
// Listenees
//
//======================================================================================================

class OnPressListenee
{
public:
	OnPressListenee();
	virtual ~OnPressListenee();

	virtual void addPressListener(IOnPressListener* listener);

protected:
	virtual void throwPressEvent(IOnPressListener::Data& data);

protected:
	std::vector<IOnPressListener*> m_pressListeners;
};