
#include "FrmPlatform.h"
#include "MyUtils.h"

static const bool LOGGING_ENABLE = true;

void smartLog(const MyString& message)
{
	if (LOGGING_ENABLE)
	{
		FrmLogMessage(message.c_str());
	}
}

void notify(const MyString& message)
{
}

void exitGame()
{
	FrmAssert(0);
}