#pragma once

#include "Ped.h"

class CCivilianPed : public CPed
{
public:
	CCivilianPed(int, int);
	~CCivilianPed(void) { }

	void ProcessControl(void);
};
static_assert(sizeof(CCivilianPed) == 0x53C, "CCivilianPed: error");
