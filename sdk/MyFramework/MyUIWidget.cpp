
#include "MyUIWidget.h"

UIWidget::UIWidget()
	: m_status(ACTIVE)
{
}

UIWidget::~UIWidget()
{
}

void UIWidget::init(const MyString& id, const MyVec2& pos, const MyVec2& size)
{
	m_id = id;
	m_bounding.Pos = pos;
	m_bounding.Size = size;
}

void UIWidget::update(UserInput& userInput)
{
	if (m_status == ACTIVE)
	{
		MyVec2 pos;
		if (userInput.pointer_Releasing(pos) && isInside(pos, m_bounding))
		{
			IOnPressListener::Data data(m_id, (int)pos.x, (int)pos.y);
			throwPressEvent(data);
		}
	}
}

// Getter

MyString UIWidget::getId()const
{
	return m_id;
}

const MyVec2& UIWidget::getSize()const
{
	return m_bounding.Size;
}

const MyVec2& UIWidget::getPos()const
{
	return m_bounding.Pos;
}

// Setter

void UIWidget::setPos(const MyVec2& pos)
{
	m_bounding.Pos = pos;
}

void UIWidget::setStatus(Status status)
{
	m_status = status;
}