
#pragma once

#include "MyInput.h"
#include "MySpriteBatch.h"
#include "MyEventListener.h"

class UIWidget : public OnPressListenee
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

	virtual void init(const MyString& id, const MyVec2& pos, const MyVec2& size);
	virtual void update(UserInput& userInput);
	virtual void render(SpriteBatch& spriteBatch) = 0;

	// Getter

	MyString getId()const;
	const MyVec2& getSize()const;
	const MyVec2& getPos()const;

	// Setter

	void setPos(const MyVec2& pos);
	void setStatus(Status status);

protected:
	MyString m_id;
	Rect2D m_bounding;
	Status m_status;
};