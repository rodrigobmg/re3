#include "common.h"
#include "patcher.h"
#include "CutsceneMgr.h"

bool &CCutsceneMgr::ms_running = *(bool*)0x95CCF5;
bool &CCutsceneMgr::ms_cutsceneProcessing = *(bool*)0x95CD9F;
