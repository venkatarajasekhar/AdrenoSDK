
#pragma once

#include <string>
#include <vector>
#include <map>

#include <sstream>

// Including ExtFrmMath.h is always beneath including stl
#include "MyMath.h"

#include "FrmUtils.h"

//===========================================================================================================
//
// Typedef
//
//===========================================================================================================

typedef unsigned int UINT;
typedef std::string MyString;

typedef std::vector<int> IntArray;
typedef std::vector<UINT> UIntArray;
typedef std::vector<float> FloatArray;

typedef std::map<MyString, int> StringIntDictionary;

//===========================================================================================================
//
// Macro
//
//===========================================================================================================

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_C
#define SAFE_DELETE_C(p)     { if (p) { free (p);       (p) = NULL; } }
#endif

//===========================================================================================================
//
// Functions
//
//===========================================================================================================

void smartLog(const MyString& message);
void notify(const MyString& message);
void exitGame();
void throwIfFailed(bool result, const MyString& message);

template <typename T>
inline MyString toString(const T& t)
{
	std::ostringstream ostr;
	ostr << t;
	return ostr.str();
}

MyString resolveAssetsPath(const MyString& relativePath);
void getWindowDimension(int& width, int& height);