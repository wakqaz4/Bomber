#include "Global.h"

static Global* global = nullptr;

Global* Global::getInstance()
{
	if (global == nullptr)
		global = new Global;
	return global;
}