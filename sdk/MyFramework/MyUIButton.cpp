
#include "MyUIButton.h"

//===========================================================================================================
//
// Constants
//
//===========================================================================================================

static const float SCALE_WHEN_PRESSED = 0.95f;

//===========================================================================================================
//
// UIImageButton class
//
//===========================================================================================================

UIImageButton::UIImageButton()
	: m_background(nullptr),
	m_scale(1.0f)
{
}

UIImageButton::~UIImageButton()
{
}

void UIImageButton::init(const MyString& id, const MyVec2& pos, Texture& background)
{
	m_background = &background;
	UIWidget::init(id, pos, MyVec2(m_background->getWidth(), m_background->getHeight()));
}

void UIImageButton::update(UserInput& userInput)
{
	m_scale = 1.0f;
	if (m_status == ACTIVE)
	{
		MyVec2 pos;
		if (userInput.pointer_Holding(pos) && isInside(pos, m_bounding))
		{
			m_scale = SCALE_WHEN_PRESSED;
		}
	}

	UIWidget::update(userInput);
}

void UIImageButton::render(SpriteBatch& spriteBatch, const Rect2D* viewport)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	m_bounding.Size *= m_scale;
	spriteBatch.renderTexture2D(m_background, m_bounding, nullptr, 0, viewport);
	m_bounding.Size *= 1.0f / m_scale;
}

//===========================================================================================================
//
// UITextButton class
//
//===========================================================================================================

UITextButton::UITextButton()
{}

UITextButton::~UITextButton()
{}

void UITextButton::init(
	const MyString& id,
	const MyVec2& pos,
	Texture& background,
	const MyString& text,
	Font& textFont,
	const MyColor& textColor,
	float textSize)
{
	m_label.init("", MyVec2(0), textFont, text, textColor, textSize);
	UIImageButton::init(id, pos, background);
}

void UITextButton::render(SpriteBatch& spriteBatch, const Rect2D* viewport)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	// Render background
	UIImageButton::render(spriteBatch, viewport);

	// Render text
	{
		MyVec2 pos = getPos() + 0.5f * (getSize() - m_label.getSize());
		m_label.setPos(pos);
		m_label.accSize(m_scale);
		m_label.render(spriteBatch, viewport);
		m_label.accSize(1.0f / m_scale);
	}
}