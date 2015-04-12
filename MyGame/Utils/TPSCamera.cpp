
#include "TPSCamera.h"

//==============================================================================================================
//
// Constants
//
//==============================================================================================================

static const MyVec3 PLAYER_OFFSET   = MyVec3(0, 20, 15);
static const float  CAM_SENSITIVITY = 0.05f;

//==============================================================================================================
//
// TPSCamera class
//
//==============================================================================================================

TPSCamera::TPSCamera()
{
}

TPSCamera::~TPSCamera()
{
}

void TPSCamera::init(
	const MyVec3& playerPos,
	float fovInDeg,
	float n,
	float f)
{
	PerspectiveCamera::init(playerPos + PLAYER_OFFSET, playerPos, fovInDeg, n, f);
}

void TPSCamera::update(Timer& timer, UserInput& userInput)
{
	{
		MyVec2 delta;
		if (userInput.pointer_Dragging(delta))
		{
			auto eye = getEye();

			eye.x -= delta.x * CAM_SENSITIVITY;
			eye.z -= delta.y * CAM_SENSITIVITY;

			setEye(eye);
		}
	}

	Camera::update(timer, userInput);
}

void TPSCamera::setPlayerPos(const MyVec3& playerPos)
{
	setEye(playerPos + PLAYER_OFFSET);
}