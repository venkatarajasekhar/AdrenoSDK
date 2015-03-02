
#pragma once

#include "MyUIWidget.h"

class UIButton : public UIWidget
{
public:
	UIButton();
	~UIButton();

	void init(const MyVec2& pos, Texture& texture);
	void render(SpriteBatch& spriteBatch);

private:
	Texture* m_texture;
};