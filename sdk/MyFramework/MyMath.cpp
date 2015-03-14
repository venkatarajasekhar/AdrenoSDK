
#include "MyMath.h"

//===========================================================================================================
//
// Algebraic functions
//
//===========================================================================================================

float dSin(float angleInDeg)
{
#ifdef GLM_FORCE_RADIANS
	return glm::sin(glm::radians(angleInDeg));
#else
	return glm::sin(angleInDeg);
#endif
}

float dCos(float angleInDeg)
{
#ifdef GLM_FORCE_RADIANS
	return glm::cos(glm::radians(angleInDeg));
#else
	return glm::cos(angleInDeg);
#endif
}

float dTan(float angleInDeg)
{
#ifdef GLM_FORCE_RADIANS
	return glm::tan(glm::radians(angleInDeg));
#else
	return glm::tan(angleInDeg);
#endif
}

// Return angle in degree
float dASin(float x)
{
#ifdef GLM_FORCE_RADIANS
	return glm::degrees(glm::asin(x));
#else
	return glm::asin(x);
#endif
}

// Return angle in degree
float dACos(float x)
{
#ifdef GLM_FORCE_RADIANS
	return glm::degrees(glm::acos(x));
#else
	return glm::acos(x);
#endif
}

// Return angle in degree
float dATan(float x)
{
#ifdef GLM_FORCE_RADIANS
	return glm::degrees(glm::atan(x));
#else
	return glm::atan(x);
#endif
}

// Return angle in degree
// y: opposite
// x: adjacent
float dATan2(float y, float x)
{
#ifdef GLM_FORCE_RADIANS
	return glm::degrees(glm::atan(y, x));
#else
	return glm::atan(y, x);
#endif
}

//===========================================================================================================
//
// Vector computing functions
//
//===========================================================================================================

MyVec3 normalize(const MyVec3& v)
{
	return glm::normalize(v);
}

MyVec3 cross(const MyVec3& v1, const MyVec3& v2)
{
	return glm::cross(v1, v2);
}

float dot(const MyVec3& v1, const MyVec3& v2)
{
	return glm::dot(v1, v2);
}

//===========================================================================================================
//
// Camera matrix functions
//
//===========================================================================================================

MyMat4 createLookAtMatrix(const MyVec3& eye, const MyVec3& target, const MyVec3& up)
{
	return glm::lookAt(eye, target, up);
}

MyMat4 createPerspectiveMatrix(float fovInDeg, float aspect, float n, float f)
{
#ifdef GLM_FORCE_RADIANS
	return glm::perspective(glm::radians(fovInDeg), aspect, n, f);
#else
	return glm::perspective(fovInDeg, aspect, n, f);
#endif
}

MyMat4 createOrthoMatrix(float left, float right, float bottom, float top)
{
	return glm::ortho(left, right, bottom, top);
}

//===========================================================================================================
//
// Transformation matrix functions
//
//===========================================================================================================

MyMat4 createTranslationMatrix(const MyVec3& translate)
{
	return glm::translate(glm::mat4(1.0f), translate);
}

MyMat4 createRotationMatrix(const MyVec3& axis, float angleInDeg)
{
#ifdef GLM_FORCE_RADIANS
	return glm::rotate(glm::mat4(1.0f), glm::radians(angleInDeg), axis);
#else
	return glm::rotate(glm::mat4(1.0f), angleInDeg, axis);
#endif
}

MyMat4 createRotationXMatrix(float angleInDeg)
{
	return createRotationMatrix(MyVec3(1.0f, 0.0f, 0.0f), angleInDeg);
}

MyMat4 createRotationYMatrix(float angleInDeg)
{
	return createRotationMatrix(MyVec3(0.0f, 1.0f, 0.0f), angleInDeg);
}

MyMat4 createRotationZMatrix(float angleInDeg)
{
	return createRotationMatrix(MyVec3(0.0f, 0.0f, 1.0f), angleInDeg);
}

MyMat4 createYawPitchRollMatrix(float yawInDeg, float pitchInDeg, float rollInDeg)
{
	return createRotationXMatrix(pitchInDeg) * createRotationYMatrix(yawInDeg) * createRotationZMatrix(rollInDeg);
}

MyMat4 createScaleMatrix(const MyVec3& scale)
{
	return glm::scale(glm::mat4(1.0f), scale);
}

//===========================================================================================================
//
// Geometric functions
//
//===========================================================================================================

bool isInside(const MyVec2& point, const Rect2D& rect)
{
	return (
		(point.x >= rect.Pos.x) && 
		(point.x <= rect.Pos.x + rect.Size.x) &&
		(point.y >= rect.Pos.y) && 
		(point.y <= rect.Pos.y + rect.Size.y)
		);
}

float distance(const MyVec3& p1, const MyVec3& p2)
{
	return glm::distance(p1, p2);
}

MyVec3 intersect(const Ray& ray, const Plane& plane)
{
	MyVec3 n = normalize(ray.Directiton);
	MyVec3 P0 = ray.Position;

	MyVec3 v = normalize(plane.Normal);
	float d0 = plane.D;

	float numerator = (dot(P0, v) + d0);
	float denominator = dot(n, v);

	if (denominator == 0)
	{
		return MyVec3(0, 0, 0);
	}

	float t = -numerator / denominator;

	return (P0 + t * n);
}

MyVec2 project(const MyVec3& posW, int w, int h, const MyMat4& view, const MyMat4& proj)
{
	float halfW = (float)w / 2.0f;
	float halfH = (float)h / 2.0f;

	MyVec4 posH = proj * view * MyVec4(posW, 1.0f);
	posH = posH / posH.w;

	return MyVec2(halfW * (posH.x + 1), halfH * (1 - posH.y));
}

MyVec3 unProject(const MyVec2& screenPos, int w, int h, const MyMat4& view, const MyMat4& proj, float depth)
{
	MyVec4 viewport = MyVec4(0, 0, w, h);
	MyVec3 _screenPos = MyVec3(screenPos.x, h - screenPos.y - 1, depth);
	return glm::unProject(_screenPos, view, proj, viewport);
}

Ray createRayInWorld(const MyVec2& screenPos, int w, int h, const MyMat4& view, const MyMat4& proj)
{
	MyVec3 P0 = unProject(screenPos, w, h, view, proj, 0.0f);
	MyVec3 P1 = unProject(screenPos, w, h, view, proj, 10.0f);

	return{ P1 - P0, P0 };
}