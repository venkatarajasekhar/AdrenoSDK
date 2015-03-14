
#pragma once

#include "MySpriteBatch.h"
#include "MyUIButton.h"

class MiniMap
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

private:
	Texture* m_background;
	Texture* m_player;
	UIButton m_closeButton;

	MyVec3 m_mapCenter;
	MyVec2 m_mapSize;

	int m_widthWnd, m_heightWnd;

	Status m_status;

private:
	static const int PADDING_MINI_MAP = 15;
	static const int PADDING_CLOSE_BUTTON = 8;
};