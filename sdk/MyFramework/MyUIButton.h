
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
	virtual ~UIImageButton();

	virtual void init(const MyString& id, const MyVec2& pos, Texture& background);
	virtual void update(UserInput& userInput);
	virtual void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr);

protected:
	Texture* m_background;
	float m_scale;
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
	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr);

private:
	UILabel m_label;
};