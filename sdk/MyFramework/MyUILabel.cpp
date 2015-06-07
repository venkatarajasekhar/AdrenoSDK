
#include "MyUILabel.h"

//==========================================================================================================
//
// Helper function
//
//==========================================================================================================

static MyString cutDownText(
	const MyString& text, 
	float maxWidth, 
	UILabel::ACTION_EXCEED_WIDTH action, 
	Font& font)
{
	int prevSpacePos(0);
	int newLinePivot(0);
	MyString textResult = text;

	if (action == UILabel::CUT_DOWN_WITH_ELLIPSIS)
	{
		maxWidth -= font.getTextWidth("...");
	}

	for (int i = 0; i < text.size(); i++)
	{
		if ((text[i] == ' ') || (text[i] == '\n') || (i == text.size() - 1))
		{
			MyString s = text.substr(newLinePivot, i - newLinePivot);
			if (font.getTextWidth(s) > maxWidth)
			{
				if (action == UILabel::CUT_DOWN)
				{
					return text.substr(0, prevSpacePos);
				}
				else if (action == UILabel::CUT_DOWN_WITH_ELLIPSIS)
				{
					return text.substr(0, prevSpacePos) + "...";
				}
				else
				{
					textResult[prevSpacePos] = '\n';
					newLinePivot = prevSpacePos + 1;
				}
			}

			prevSpacePos = i;
		}
		
		if (text[i] == '\n')
		{
			newLinePivot = i + 1;
		}
	}

	return textResult;
}

//==========================================================================================================
//
// UILabel class
//
//==========================================================================================================

UILabel::UILabel()
	: m_font(nullptr),
	m_size(0.0f),
	m_maxWidth(-1.0f),
	m_action(NEW_LINE)
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
	init(id, pos, font, text, -1, NEW_LINE, color, size);
}

void UILabel::init(
	const MyString& id,
	const MyVec2& pos,
	Font& font,
	const MyString& text,
	float maxWidth,
	UILabel::ACTION_EXCEED_WIDTH action,
	const MyColor& color,
	float size)
{
	m_font = &font;
	m_maxWidth = maxWidth;
	m_action = action;
	m_size = size;
	m_color = color;
	setText(text);

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

void UILabel::setText(const MyString& text)
{
	if (m_maxWidth >= 0.0f)
	{
		m_text = cutDownText(text, m_maxWidth, m_action, *m_font);
	}
	else
	{
		m_text = text;
	}
	
	m_bounding.Size.x = m_font->getTextWidth(m_text);
}

void UILabel::accSize(float size)
{
	m_size *= size;
}