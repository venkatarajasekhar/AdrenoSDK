
#include "MyUIButton.h"

UIButton::UIButton()
	: m_texture(nullptr)
{
}

UIButton::~UIButton()
{
}

void UIButton::init(const MyVec2& pos, Texture& texture)
{
	m_texture = &texture;
	UIWidget::init(pos, MyVec2(m_texture->getWidth(), m_texture->getHeight()));
}

void UIButton::render(SpriteBatch& spriteBatch)
{
	spriteBatch.renderTexture2D(*m_texture, m_pos, 0, MyVec2(1));
}