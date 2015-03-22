
#include "MyUIButton.h"

UIButton::UIButton()
	: m_texture(nullptr)
{
}

UIButton::~UIButton()
{
}

void UIButton::init(const MyString& id, const MyVec2& pos, Texture& texture)
{
	m_texture = &texture;
	UIWidget::init(id, pos, MyVec2(m_texture->getWidth(), m_texture->getHeight()));
}

void UIButton::render(SpriteBatch& spriteBatch)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	spriteBatch.renderTexture2D(m_texture, m_bounding);
}