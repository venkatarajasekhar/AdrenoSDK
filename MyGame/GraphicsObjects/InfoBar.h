
#pragma once

#include <MyUILabel.h>

class InfoBar
{
public:
	InfoBar();
	~InfoBar();

	void init(Texture& background, Font& font, const MyVec2& pos, const MyColor& textColor, float textSize);
	void update(UserInput& userInput);
	void render(SpriteBatch& spriteBatch);

	void setValue(int value);

private:
	Texture* m_background;
	UILabel m_label;
	MyVec2 m_pos;
};