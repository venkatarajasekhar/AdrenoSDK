
#pragma once

#include <MySpriteBatch.h>
#include <MyUIButton.h>

class MiniMap : public IOnPressListener, public OnPressListenee
{
private:
	enum Status
	{
		SMALL,
		LARGE,
	};

public:
	MiniMap();
	~MiniMap();

	void init(
		Texture& background, 
		Texture& player, 
		Texture& closeButton, 
		const MyVec3& mapCenter, 
		const MyVec2& mapSize);
	void resize(int width, int height);
	void update(UserInput& userInput);
	void render(SpriteBatch& spriteBatch, const MyVec3& playerPos);

	void OnPress(const IOnPressListener::Data& data);

	MyVec2 getSmallSize();
	MyVec2 getPos();

private:
	void setStatus(Status status);
	void updateBounding();

private:
	Texture* m_background;
	Texture* m_player;
	UIImageButton m_closeButton;

	Rect2D m_bounding;

	MyVec3 m_mapCenter;
	MyVec2 m_mapSize;

	int m_widthWnd, m_heightWnd;

	Status m_status;
};