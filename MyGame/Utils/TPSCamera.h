
#pragma once

#include <MyPerspectiveCamera.h>

class TPSCamera : public PerspectiveCamera
{
public:
	TPSCamera();
	~TPSCamera();

	void init(
		const MyVec3& playerPos,
		float fovInDeg,
		float n,
		float f);
	void update(Timer& timer, UserInput& userInput);

	void setPlayerPos(const MyVec3& playerPos);

private:

};