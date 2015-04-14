
#include "Tower_Main.h"

Tower_Main::Tower_Main()
{
}

Tower_Main::~Tower_Main()
{
}

void Tower_Main::dead()
{
	smartLog("Main tower is collapse ..................");
	LivingEntity::dead();
}