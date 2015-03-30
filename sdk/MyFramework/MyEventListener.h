
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
		// ID of listenee
		MyString Id;

		// Coordinate (screen position) of pressed point
		int x, y;

		// Additional infomation that listenee send to listener
		void* Tag;

		Data(const MyString& _id, int _x, int _y, void* _tag = nullptr)
			: Id(_id),
			x(_x),
			y(_y),
			Tag(_tag)
		{}
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