
#pragma once

#include "MyInput.h"
#include "MySpriteBatch.h"

class UIWidget
{
public:
	enum Status
	{
		ACTIVE,
		INACTIVE,
		HIDDEN,
	};

public:
	UIWidget();
	virtual ~UIWidget();

	virtual void init(const MyVec2& pos, const MyVec2& size);
	virtual void update(UserInput& userInput);
	virtual void render(SpriteBatch& spriteBatch) = 0;

	// Getter

	const MyVec2& getSize()const;
	const MyVec2& getPos()const;

	bool isPressing()const;

	// Setter

	void setPos(const MyVec2& pos);
	void setStatus(Status status);

protected:
	MyVec2 m_pos;
	MyVec2 m_size;
	bool m_isPressing;

	Status m_status;
};