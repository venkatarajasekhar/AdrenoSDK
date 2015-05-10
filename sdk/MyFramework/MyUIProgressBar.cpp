
#include "MyUIProgressBar.h"

UIProgressBar::UIProgressBar()
	: m_foreground(nullptr),
	m_background(nullptr),
	m_progress(0.0f)
{
}

UIProgressBar::~UIProgressBar()
{
}

void UIProgressBar::init(
	const MyString& id,
	const MyVec2& pos,
	Texture& foreground,
	Texture& background,
	const MyVec2& scale)
{
	m_foreground = &foreground;
	m_background = &background;

	UIWidget::init(id, pos, scale * MyVec2(background.getWidth(), background.getHeight()));
}

void UIProgressBar::render(SpriteBatch& spriteBatch, const Rect2D* viewport)
{
	if (m_status == HIDDEN)
	{
		return;
	}

	spriteBatch.renderTexture2D(m_background, m_bounding, nullptr, 0, viewport);

	{
		Rect2D bounding = m_bounding;
		bounding.Size.x *= m_progress;

		spriteBatch.renderTexture2D(m_foreground, bounding, nullptr, 0, viewport);
	}
}

void UIProgressBar::setProgress(float progress)
{
	m_progress = clamp(progress, 0.0f, 1.0f);
}