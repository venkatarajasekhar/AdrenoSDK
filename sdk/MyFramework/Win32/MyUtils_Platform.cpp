
#include "FrmPlatform.h"
#include "MyUtils.h"
#include <iostream>

using namespace std;

static const bool LOGGING_ENABLE = true;

void smartLog(const MyString& message)
{
	if (LOGGING_ENABLE)
	{
		cout << message << endl;
	}
}

void notify(const MyString& message)
{
	MessageBoxA(NULL, message.c_str(), "", MB_ICONERROR);
}

void exitGame()
{
	exit(0);
}