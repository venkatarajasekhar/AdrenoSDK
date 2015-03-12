
#include "MyUtils.h"

//===========================================================================================================
//
// Functions
//
//===========================================================================================================

void throwIfFailed(bool result, const MyString& message)
{
	if (!result)
	{
		smartLog(message);
		notify(message);
		exitGame();
	}
}