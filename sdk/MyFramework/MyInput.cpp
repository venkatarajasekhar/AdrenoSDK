
#include "MyInput.h"

#pragma region Helpers

//=================================================================================================
//
// Helpers
//
//=================================================================================================

// Convert homogeneous coordinate to screen coordinate.
// Homogeneous coordinate:
//   - Center-screen: (+0, +0)
//   - Upper-left:    (-1, +1)
//   - Upper-right:   (+1, +1)
//   - Lower-left:    (-1, -1)
//   - Lower-right:   (+1, -1)
static MyVec2 homogeneous2Screen(const FRMVECTOR2& coorH, int width, int height)
{
	FLOAT32 x = coorH.x;
	FLOAT32 y = coorH.y;

	return MyVec2(
		(1 + x) * (FLOAT32)width / 2.0f,
		(1 - y) * (FLOAT32)height / 2.0f);
}

#pragma endregion

#pragma region Pointer class

//=================================================================================================
//
// Pointer class
//
//=================================================================================================

UserInput::Pointer::Pointer()
	: m_frmInput(nullptr),
	m_width(0),
	m_height(0)
{

}

void UserInput::Pointer::init(FRM_INPUT& frmInput)
{
	m_frmInput = &frmInput;
}

void UserInput::Pointer::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void UserInput::Pointer::update()
{
	// Get pointer input since the last frame
	UINT32 status;
	FRMVECTOR2 pos;
	m_frmInput->GetPointerState(&status, &pos);

	// Set pointer status
	m_prevStatus = m_currStatus;
	m_currStatus = (status & FRM_INPUT::POINTER_DOWN ? POINTER_DOWN : POINTER_UP);

	// Set pointer position
	if (m_currStatus == POINTER_DOWN)
	{
		m_currPos = homogeneous2Screen(pos, m_width, m_height);
		m_deltaPos = ((m_prevStatus == POINTER_DOWN) ? m_currPos - m_prevPos : MyVec2(0, 0));
		m_prevPos = m_currPos;

		if (m_prevStatus == POINTER_UP)
		{
			m_beginDraggingPos = m_currPos;
		}
	}
}

bool UserInput::Pointer::isPressing(MyVec2& pos)const
{
	pos = m_currPos;
	return ((m_currStatus == POINTER_DOWN) && (m_prevStatus == POINTER_UP));
}

bool UserInput::Pointer::isReleasing(MyVec2& pos)const
{
	pos = m_currPos;
	return (
		(m_currStatus == POINTER_UP) && 
		(m_prevStatus == POINTER_DOWN) && 
		(abs(m_currPos.x - m_beginDraggingPos.x) < 0.1f) &&
		(abs(m_currPos.y - m_beginDraggingPos.y) < 0.1f)
		);
}

bool UserInput::Pointer::isHolding(MyVec2& pos)const
{
	pos = m_currPos;
	return (m_currStatus == POINTER_DOWN);
}

bool UserInput::Pointer::isDragging(MyVec2& delta)const
{
	delta = m_deltaPos;
	return (m_currStatus == POINTER_DOWN);
}

#pragma endregion

//=================================================================================================
//
// UserInput class
//
//=================================================================================================

UserInput::UserInput()
	: m_locked(false)
{
}

UserInput::~UserInput()
{
}

void UserInput::init(FRM_INPUT& frmInput)
{
	m_pointer.init(frmInput);
}

void UserInput::resize(int width, int height)
{
	m_pointer.resize(width, height);
}

void UserInput::update()
{
	if (!m_locked)
	{
		m_pointer.update();
	}
}

void UserInput::lock()
{
	m_locked = true;
}

void UserInput::unlock()
{
	m_locked = false;
}

// Pointer functionalities

bool UserInput::pointer_Pressing(MyVec2& pos)const
{
	return (m_pointer.isPressing(pos) && !m_locked);
}

bool UserInput::pointer_Releasing(MyVec2& pos)const
{
	return (m_pointer.isReleasing(pos) && !m_locked);
}

bool UserInput::pointer_Holding(MyVec2& pos)const
{
	return (m_pointer.isHolding(pos) && !m_locked);
}

bool UserInput::pointer_Dragging(MyVec2& delta)const
{
	return (m_pointer.isDragging(delta) && !m_locked);
}