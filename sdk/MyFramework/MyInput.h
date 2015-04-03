
#pragma once

#include "MyUtils.h"
#include <FrmInput.h>

class UserInput
{
public:
	UserInput();
	~UserInput();

	void init(FRM_INPUT& frmInput);
	void resize(int width, int height);
	void update();

	void lock();
	void unlock();

	// Pointer functionalities

	bool pointer_Pressing(MyVec2& pos)const;
	bool pointer_Releasing(MyVec2& pos)const;
	bool pointer_Holding(MyVec2& pos)const;
	bool pointer_Dragging(MyVec2& delta)const;

private:

#pragma region Inner class

	class Pointer
	{
	public:
		Pointer();

		void init(FRM_INPUT& frmInput);
		void resize(int width, int height);
		void update();

		bool isPressing(MyVec2& pos)const;
		bool isReleasing(MyVec2& pos)const;
		bool isHolding(MyVec2& pos)const;
		bool isDragging(MyVec2& delta)const;

	private:
		enum POINTER_STATUS
		{
			POINTER_UP,
			POINTER_DOWN,
		};

	private:
		FRM_INPUT* m_frmInput;

		int m_width, m_height;

		POINTER_STATUS m_currStatus, m_prevStatus;
		MyVec2 m_currPos, m_prevPos, m_deltaPos;
	};

#pragma endregion

#pragma region Unused class

	class Keyboard
	{

	};

	class Sensor
	{

	};

#pragma endregion

private:
	Pointer m_pointer;
	bool m_locked;
};