
#include "MyUIButton.h"

//===========================================================================================================
//
// UIImageButton class
//
//===========================================================================================================

UIImageButton::UIImageButton()
	: m_background(nullptr)
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

void UIImageButton::render(SpriteBatch& spriteBatch)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	spriteBatch.renderTexture2D(m_background, m_bounding);
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

void UITextButton::render(SpriteBatch& spriteBatch)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	// Render background
	UIImageButton::render(spriteBatch);

	// Render text
	{
		MyVec2 pos = getPos() + 0.5f * (getSize() - m_label.getSize());
		m_label.setPos(pos);
		m_label.render(spriteBatch);
	}
}