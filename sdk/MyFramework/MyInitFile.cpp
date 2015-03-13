
#include "MyInitFile.h"

InitFile::InitFile()
{
}

InitFile::~InitFile()
{
}

void InitFile::init(const MyString& filename)
{
	CHAR* fileContent;
	throwIfFailed((TRUE == FrmLoadFile(filename.c_str(), (VOID**)&fileContent)), "Couldn't load file: " + filename);
}