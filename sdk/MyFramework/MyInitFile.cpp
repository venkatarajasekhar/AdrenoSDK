
#include "MyInitFile.h"

using namespace std;

static const int MAX_LINE_LENGTH = 64;

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

	istringstream istr(fileContent);

	FrmUnloadFile(fileContent);
	fileContent = nullptr;

	char line[MAX_LINE_LENGTH];
	while (istr.getline(line, MAX_LINE_LENGTH))
	{
		addParam(line);
	}
}

void InitFile::addParam(const MyString& line)
{
	char token = '=';
	auto pos = line.find(token);

	if (pos != string::npos)
	{
		string key = line.substr(0U, pos);
		string value = line.substr(pos + 1);

		m_params[key] = value;
	}
}