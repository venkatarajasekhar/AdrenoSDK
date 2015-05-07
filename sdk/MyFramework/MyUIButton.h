
#pragma once

#include "MyUILabel.h"

//===========================================================================================================
//
// UIImageButton class
//
//===========================================================================================================

class UIImageButton : public UIWidget
{
public:
	UIImageButton();
	~UIImageButton();

	void init(const MyString& id, const MyVec2& pos, Texture& background);
	void render(SpriteBatch& spriteBatch);

private:
	Texture* m_background;
};

//===========================================================================================================
//
// UITextButton class
//
//===========================================================================================================

class UITextButton : public UIImageButton
{
public:
	UITextButton();
	~UITextButton();

	void init(
		const MyString& id, 
		const MyVec2& pos, 
		Texture& background,
		const MyString& text,
		Font& textFont,
		const MyColor& textColor = MyColor(1, 1, 1), 
		float textSize = 1.0f);
	void render(SpriteBatch& spriteBatch);

private:
	UILabel m_label;
};