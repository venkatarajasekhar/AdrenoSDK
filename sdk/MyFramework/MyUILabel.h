
#pragma once

#include "MyUIWidget.h"

class UILabel : public UIWidget
{
public:
	UILabel();
	~UILabel();

	void init(
		const MyString& id, 
		const MyVec2& pos, 
		Font& font, 
		const MyString& text, 
		const MyColor& color = MyColor(1, 1, 1), 
		float size = 1.0f);
	void render(SpriteBatch& spriteBatch);

private:
	Font m_font;
	MyString m_text;
	float m_size;
	MyColor m_color;
};