
#include "MyUILabel.h"

UILabel::UILabel()
	: m_font(nullptr),
	m_size(0.0f)
{}

UILabel::~UILabel()
{}

void UILabel::init(
	const MyString& id,
	const MyVec2& pos,
	Font& font,
	const MyString& text,
	const MyColor& color,
	float size)
{
	m_font = &font;
	m_text = text;
	m_size = size;
	m_color = color;

	m_font->setScale(MyVec2(m_size, m_size));

	UIWidget::init(id, pos, MyVec2(m_font->getTextWidth(m_text), m_font->getTextHeight()));
}

void UILabel::render(SpriteBatch& spriteBatch, const Rect2D* viewport)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	m_font->setScale(MyVec2(m_size, m_size));
	spriteBatch.renderText2D(*m_font, m_text, getPos(), 0.0f, viewport, m_color);
}