
#include "InfoBar.h"

//=====================================================================================================================
//
// Constants
//
//=====================================================================================================================

static const float TEXT_MARGIN = 15.0f;

//=====================================================================================================================
//
// InfoBar class
//
//=====================================================================================================================

InfoBar::InfoBar()
	: m_background(nullptr)
{
}

InfoBar::~InfoBar()
{
}

void InfoBar::init(Texture& background, Font& font, const MyVec2& pos, const MyColor& textColor, float textSize)
{
	m_background = &background;
	m_pos = pos;
	m_label.init("", MyVec2(), font, "", textColor, textSize);
	setValue(0);
}

void InfoBar::update(UserInput& userInput)
{
	m_label.update(userInput);
}

void InfoBar::render(SpriteBatch& spriteBatch)
{
	spriteBatch.renderTexture2D(m_background, m_pos);
	m_label.render(spriteBatch);
}

void InfoBar::setValue(int value)
{
	MyString text = toString(value);
	m_label.setText(text);

	MyVec2 pos = m_pos + MyVec2(
		m_background->getWidth() - TEXT_MARGIN - m_label.getSize().x,
		0.5f * (m_background->getHeight() - m_label.getSize().y)
		);
	m_label.setPos(pos);
}