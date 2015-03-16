
#pragma once

// Including ExtFrmUtils.h instead of include directly ExtFrmMath.h

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

//===========================================================================================================
//
// Typedef
//
//===========================================================================================================

typedef glm::vec2 MyVec2;
typedef glm::vec3 MyVec3;
typedef glm::vec4 MyVec4;

typedef glm::highp_ivec2 MyIVec2;
typedef glm::highp_ivec3 MyIVec3;
typedef glm::highp_ivec4 MyIVec4;

typedef glm::mat3 MyMat3;
typedef glm::mat4 MyMat4;

typedef MyVec3 MyColor;

//===========================================================================================================
//
// Geometric structures
//
//===========================================================================================================

struct Ray
{
	MyVec3 Directiton;
	MyVec3 Position;
};

struct Plane
{
	// If normal vector point to origin, D is positive.
	// If normal vector point away origin, D is negative.
	float D;
	MyVec3 Normal;
};

struct Rect2D
{
	Rect2D(){}
	Rect2D(MyVec2 _pos, MyVec2 _size)
		: Pos(_pos),
		Size(_size)
	{
	}

	MyVec2 Pos;
	MyVec2 Size;
};

//===========================================================================================================
//
// IO functions
//
//===========================================================================================================

template <typename T>
T& operator>>(T& istr, MyVec2& vec)
{
	istr >> vec.x >> vec.y;
	return istr;
}

template <typename T>
T& operator>>(T& istr, MyVec3& vec)
{
	istr >> vec.x >> vec.y >> vec.z;
	return istr;
}

template <typename T>
T& operator>>(T& istr, MyVec4& vec)
{
	istr >> vec.x >> vec.y >> vec.z >> vec.w;
	return istr;
}

//===========================================================================================================
//
// Algebraic functions
//
//===========================================================================================================

template <typename T>
inline T random(T min, T max)
{
	return glm::linearRand(min, max);
}

template <typename T>
inline T abs(T x)
{
	return (x >= 0 ? x : -x);
}

template <typename T>
inline T min(T x, T y)
{
	return (x > y ? y : x);
}

template <typename T>
inline T max(T x, T y)
{
	return (x > y ? x : y);
}

template <typename T>
inline T clamp(T x, T min, T max)
{
	return (x < min ? min : (x > max ? max : x));
}

template <typename T>
inline T lerp(T start, T end, float f)
{
	return (start * (1.0f - f) + end * f);
}

float dSin(float angleInDeg);
float dCos(float angleInDeg);
float dTan(float angleInDeg);

// Return angle in degree
float dASin(float x);

// Return angle in degree
float dACos(float x);

// Return angle in degree
float dATan(float x);

// Return angle in degree
// y: opposite
// x: adjacent
float dATan2(float y, float x);

//===========================================================================================================
//
// Vector computing functions
//
//===========================================================================================================

MyVec3 normalize(const MyVec3& v);
MyVec3 cross(const MyVec3& v1, const MyVec3& v2);
float dot(const MyVec3& v1, const MyVec3& v2);

//===========================================================================================================
//
// Camera matrix functions
//
//===========================================================================================================

MyMat4 createLookAtMatrix(const MyVec3& eye, const MyVec3& target, const MyVec3& up);
MyMat4 createPerspectiveMatrix(float fovInDeg, float aspect, float n, float f);
MyMat4 createOrthoMatrix(float left, float right, float bottom, float top);

//===========================================================================================================
//
// Transformation matrix functions
//
//===========================================================================================================

MyMat4 createTranslationMatrix(const MyVec3& translate);

MyMat4 createRotationMatrix(const MyVec3& axis, float angleInDeg);
MyMat4 createRotationXMatrix(float angleInDeg);
MyMat4 createRotationYMatrix(float angleInDeg);
MyMat4 createRotationZMatrix(float angleInDeg);
MyMat4 createYawPitchRollMatrix(float yawInDeg, float pitchInDeg, float rollInDeg);

MyMat4 createScaleMatrix(const MyVec3& scale);

//===========================================================================================================
//
// Geometric functions
//
//===========================================================================================================

bool isInside(const MyVec2& point, const Rect2D& rect);

float distance(const MyVec3& p1, const MyVec3& p2);
MyVec3 intersect(const Ray& ray, const Plane& plane);

MyVec2 project(const MyVec3& posW, int w, int h, const MyMat4& view, const MyMat4& proj);
MyVec3 unProject(const MyVec2& screenPos, int w, int h, const MyMat4& view, const MyMat4& proj, float depth);

Ray createRayInWorld(const MyVec2& screenPos, int w, int h, const MyMat4& view, const MyMat4& proj);

//===========================================================================================================
//
// Misc functions
//
//===========================================================================================================

template <typename T>
inline const float* getRawData(const T& t)
{
	return glm::value_ptr(t);
}

//===========================================================================================================
//
// 2 point contact
//
//===========================================================================================================

bool twoPointIsContact(MyVec3 point1, MyVec3 point2, float r);