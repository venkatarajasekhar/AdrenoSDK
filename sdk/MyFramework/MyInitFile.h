
#pragma once

#include "MyUtils.h"

class InitFile
{
public:
	InitFile();
	~InitFile();

	void init(const MyString& filename);

	template <typename T>
	void getParam(const MyString& name, T& value);

private:
	StringStringDictionary m_params;
};

template <typename T>
void InitFile::getParam(const MyString& name, T& value)
{

}