
#pragma once

#include "MyInput.h"
#include "MySpriteBatch.h"

class UIWidget
{
public:
	UIWidget();
	virtual ~UIWidget();

	virtual void init(const MyVec2& pos, const MyVec2& size);
	virtual void update(UserInput& userInput);
	virtual void render(SpriteBatch& spriteBatch) = 0;

	const MyVec2& getSize()const;
	const MyVec2& getPos()const;

	bool isPressing()const;

protected:
	MyVec2 m_pos;
	MyVec2 m_size;
	bool m_isPressing;
};