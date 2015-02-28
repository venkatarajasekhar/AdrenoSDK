
#pragma once

#include "MyUtils.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	// P: Point; V: Vector
	virtual void init(const MyVec3& eyeP, const MyVec3& targetP);
	virtual void resize(int width, int height);
	virtual void update();

	const MyVec3& getEye()const;
	const MyMat4& getView()const;
	const MyMat4& getProj()const;

protected:
	virtual void computeView();
	virtual void computeProj(int width, int height) = 0;

	virtual void updateOrientation();

protected:
	MyMat4 m_view;
	MyMat4 m_proj;

	MyVec3 m_eyeP;
	MyVec3 m_lookV;
	MyVec3 m_upV;
	MyVec3 m_rightV;
};