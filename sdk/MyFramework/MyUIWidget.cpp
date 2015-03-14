
#include "MyUIWidget.h"

UIWidget::UIWidget()
	: m_isPressing(false),
	m_status(ACTIVE)
{
}

UIWidget::~UIWidget()
{
}

void UIWidget::init(const MyVec2& pos, const MyVec2& size)
{
	m_rect.Pos = pos;
	m_rect.Size = size;
}

void UIWidget::update(UserInput& userInput)
{
	m_isPressing = false;

	if (m_status == ACTIVE)
	{
		MyVec2 pos;
		if (userInput.pointer_Releasing(pos))
		{
			m_isPressing = isInside(pos, m_rect);
		}
	}
}

// Getter

const MyVec2& UIWidget::getSize()const
{
	return m_rect.Size;
}

const MyVec2& UIWidget::getPos()const
{
	return m_rect.Pos;
}

bool UIWidget::isPressing()const
{
	return (m_isPressing && (m_status == ACTIVE));
}

// Setter

void UIWidget::setPos(const MyVec2& pos)
{
	m_rect.Pos = pos;
}

void UIWidget::setStatus(Status status)
{
	m_status = status;
}