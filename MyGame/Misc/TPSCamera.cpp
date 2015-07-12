
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
	const MyVec3& mapCenter,
	const MyVec2& mapSize,
	float fovInDeg,
	float n,
	float f)
{
	m_mapCenter = mapCenter;
	m_mapSize = mapSize;
	PerspectiveCamera::init(playerPos + PLAYER_OFFSET, playerPos, fovInDeg, n, f);
}

void TPSCamera::update(Timer& timer, UserInput& userInput)
{
	{
		MyVec2 delta;
		if (userInput.pointer_Dragging(delta))
		{
			auto eye = getEye();

			auto previewEye = eye;
			previewEye.x -= delta.x * CAM_SENSITIVITY;
			previewEye.z -= delta.y * CAM_SENSITIVITY;

			Rect2D rect;
			MyVec2 half = 0.5f * m_mapSize;
			rect.Pos = MyVec2(m_mapCenter.x - half.x, m_mapCenter.z - half.y);
			rect.Size = m_mapSize;

			if (isInside(MyVec2(previewEye.x, previewEye.z), rect))
			{
				eye = previewEye;
			}

			//eye.x -= delta.x * CAM_SENSITIVITY;
			//eye.z -= delta.y * CAM_SENSITIVITY;

			setEye(eye);
		}
	}

	Camera::update(timer, userInput);
}

void TPSCamera::setPlayerPos(const MyVec3& playerPos)
{
	setEye(playerPos + PLAYER_OFFSET);
}