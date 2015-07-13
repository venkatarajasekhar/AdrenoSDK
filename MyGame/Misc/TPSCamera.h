
#pragma once

#include <MyPerspectiveCamera.h>

class TPSCamera : public PerspectiveCamera
{
public:
	TPSCamera();
	~TPSCamera();

	void init(
		const MyVec3& playerPos,
		const MyVec3& mapCenter,
		const MyVec2& mapSize,
		float fovInDeg,
		float n,
		float f);
	void update(Timer& timer, UserInput& userInput);

	void setPlayerPos(const MyVec3& playerPos);

private:
	MyVec3 m_mapCenter;
	MyVec2 m_mapSize;
};