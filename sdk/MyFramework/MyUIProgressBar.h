
#pragma once

#include "MyUIWidget.h"

class UIProgressBar : public UIWidget
{
public:
	UIProgressBar();
	~UIProgressBar();

	void init(
		const MyString& id,
		const MyVec2& pos,
		Texture& foreground, 
		Texture& background,
		const MyVec2& scale = MyVec2(1.0f, 1.0f));
	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr);

	void setProgress(float progress);

private:
	Texture* m_foreground;
	Texture* m_background;
	float m_progress;
};