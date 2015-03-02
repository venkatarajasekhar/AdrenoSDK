
#include "MyUIWidget.h"

UIWidget::UIWidget()
	: m_isPressing(false)
{
}

UIWidget::~UIWidget()
{
}

void UIWidget::init(const MyVec2& pos, const MyVec2& size)
{
	m_pos = pos;
	m_size = size;
}

void UIWidget::update(UserInput& userInput)
{
	m_isPressing = false;

	MyVec2 pos;
	if (userInput.pointer_Releasing(pos))
	{
		float x = pos.x;
		float y = pos.y;
		if ((x >= m_pos.x) && (x <= m_pos.x + m_size.x) &&
			(y >= m_pos.y) && (y <= m_pos.y + m_size.y))
		{
			m_isPressing = true;
		}
	}
}

const MyVec2& UIWidget::getSize()const
{
	return m_size;
}

const MyVec2& UIWidget::getPos()const
{
	return m_pos;
}

bool UIWidget::isPressing()const
{
	return m_isPressing;
}