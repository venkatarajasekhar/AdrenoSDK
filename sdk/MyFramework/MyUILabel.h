
#pragma once

#include "MyUIWidget.h"

class UILabel : public UIWidget
{
public:
	enum ACTION_EXCEED_WIDTH
	{
		NEW_LINE,
		CUT_DOWN,
		CUT_DOWN_WITH_ELLIPSIS,
	};

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
	void init(
		const MyString& id,
		const MyVec2& pos,
		Font& font,
		const MyString& text,
		float maxWidth,
		UILabel::ACTION_EXCEED_WIDTH action = UILabel::NEW_LINE,
		const MyColor& color = MyColor(1, 1, 1),
		float size = 1.0f);
	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr);

	void setText(const MyString& text);

private:
	Font* m_font;
	MyString m_text;
	float m_size;
	MyColor m_color;
	float m_maxWidth;
	ACTION_EXCEED_WIDTH m_action;
};