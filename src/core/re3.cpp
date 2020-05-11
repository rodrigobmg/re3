#include <direct.h>
#include <csignal>
#define WITHWINDOWS
#include "common.h"
#include "patcher.h"
#include "Renderer.h"
#include "Credits.h"
#include "Camera.h"
#include "Weather.h"
#include "Clock.h"
#include "World.h"
#include "Vehicle.h"
#include "ModelIndices.h"
#include "Streaming.h"
#include "PathFind.h"
#include "Boat.h"
#include "Heli.h"
#include "Automobile.h"
#include "Ped.h"
#include "Particle.h"
#include "Console.h"
#include "Debug.h"
#include "Hud.h"
#include "SceneEdit.h"
#include "Pad.h"
#include "PlayerPed.h"
#include "Radar.h"
#include "debugmenu.h"
#include "Frontend.h"

#include <list>

#ifdef RWLIBS
extern "C" int vsprintf(char* const _Buffer, char const* const _Format, va_list  _ArgList);
#endif


#ifdef USE_PS2_RAND
unsigned __int64 myrand_seed = 1;
#else
unsigned long int myrand_seed = 1;
#endif

int
myrand(void)
{
#ifdef USE_PS2_RAND
	// Use our own implementation of rand, stolen from PS2
	myrand_seed = 0x5851F42D4C957F2D * myrand_seed + 1;
	return ((myrand_seed >> 32) & 0x7FFFFFFF);
#else
	// or original codewarrior rand
	myrand_seed = myrand_seed * 1103515245 + 12345;
	return((myrand_seed >> 16) & 0x7FFF);
#endif
}

void
mysrand(unsigned int seed)
{
	myrand_seed = seed;
}

#ifdef DEBUGMENU
void WeaponCheat();
void HealthCheat();
void TankCheat();
void BlowUpCarsCheat();
void ChangePlayerCheat();
void MayhemCheat();
void EverybodyAttacksPlayerCheat();
void WeaponsForAllCheat();
void FastTimeCheat();
void SlowTimeCheat();
void MoneyCheat();
void ArmourCheat();
void WantedLevelUpCheat();
void WantedLevelDownCheat();
void SunnyWeatherCheat();
void CloudyWeatherCheat();
void RainyWeatherCheat();
void FoggyWeatherCheat();
void FastWeatherCheat();
void OnlyRenderWheelsCheat();
void ChittyChittyBangBangCheat();
void StrongGripCheat();
void NastyLimbsCheat();

DebugMenuEntry *carCol1;
DebugMenuEntry *carCol2;

void
SpawnCar(int id)
{
	CVector playerpos;
	CStreaming::RequestModel(id, 0);
	CStreaming::LoadAllRequestedModels(false);
	if(CStreaming::HasModelLoaded(id)){
		playerpos = FindPlayerCoors();
		int node;
		if(!CModelInfo::IsBoatModel(id)){
			node = ThePaths.FindNodeClosestToCoors(playerpos, 0, 100.0f, false, false);
			if(node < 0)
				return;
		}

		CVehicle *v;
		if(CModelInfo::IsBoatModel(id))
			v = new CBoat(id, RANDOM_VEHICLE);
		else
			v = new CAutomobile(id, RANDOM_VEHICLE);

		v->bHasBeenOwnedByPlayer = true;
		if(carCol1)
			DebugMenuEntrySetAddress(carCol1, &v->m_currentColour1);
		if(carCol2)
			DebugMenuEntrySetAddress(carCol2, &v->m_currentColour2);

		if(CModelInfo::IsBoatModel(id))
			v->SetPosition(TheCamera.GetPosition() + TheCamera.GetForward()*15.0f);
		else
			v->SetPosition(ThePaths.m_pathNodes[node].GetPosition());

		v->GetMatrix().GetPosition().z += 4.0f;
		v->SetOrientation(0.0f, 0.0f, 3.49f);
		v->SetStatus(STATUS_ABANDONED);
		v->m_nDoorLock = CARLOCK_UNLOCKED;
		CWorld::Add(v);
	}
}

static void
FixCar(void)
{
	CVehicle *veh = FindPlayerVehicle();
	if(veh == nil)
		return;
	veh->m_fHealth = 1000.0f;
	if(!veh->IsCar())
		return;
	((CAutomobile*)veh)->Damage.SetEngineStatus(0);
	((CAutomobile*)veh)->Fix();
}

#ifdef MENU_MAP
static void
TeleportToWaypoint(void)
{
	if (FindPlayerVehicle()) {
		if (CRadar::TargetMarkerId != -1)
			FindPlayerVehicle()->Teleport(CRadar::TargetMarkerPos + CVector(0.0f, 0.0f, FindPlayerVehicle()->GetColModel()->boundingSphere.center.z));
	} else
		if(CRadar::TargetMarkerId != -1)
			FindPlayerPed()->Teleport(CRadar::TargetMarkerPos + CVector(0.0f, 0.0f, FEET_OFFSET));
}
#endif

static int engineStatus;
static void
SetEngineStatus(void)
{
	CVehicle *veh = FindPlayerVehicle();
	if(veh == nil)
		return;
	if(!veh->IsCar())
		return;
	((CAutomobile*)veh)->Damage.SetEngineStatus(engineStatus);
}

static void
ToggleComedy(void)
{
	CVehicle *veh = FindPlayerVehicle();
	if(veh == nil)
		return;
	veh->bComedyControls = !veh->bComedyControls;
}

static void
PlaceOnRoad(void)
{
	CVehicle *veh = FindPlayerVehicle();
	if(veh == nil)
		return;

	if(veh->IsCar())
		((CAutomobile*)veh)->PlaceOnRoadProperly();
}

static void
ResetCamStatics(void)
{
	TheCamera.Cams[TheCamera.ActiveCam].ResetStatics = true;
}

static const char *carnames[] = {
	"landstal", "idaho", "stinger", "linerun", "peren", "sentinel", "rio", "firetruk", "trash", "stretch", "manana",
	"infernus", "voodoo", "pony", "mule", "cheetah", "ambulan", "fbicar", "moonbeam", "esperant", "taxi", "washing",
	"bobcat", "mrwhoop", "bfinject", "hunter", "police", "enforcer", "securica", "banshee", "predator", "bus",
	"rhino", "barracks", "cuban", "chopper", "angel", "coach", "cabbie", "stallion", "rumpo", "rcbandit", "romero",
	"packer", "sentxs", "admiral", "squalo", "seaspar", "pizzaboy", "gangbur", "airtrain", "deaddodo", "speeder",
	"reefer", "tropic", "flatbed", "yankee", "caddy", "zebra", "topfun", "skimmer", "pcj600", "faggio", "freeway",
	"rcbaron", "rcraider", "glendale", "oceanic", "sanchez", "sparrow", "patriot", "lovefist", "coastg", "dinghy",
	"hermes", "sabre", "sabretur", "pheonix", "walton", "regina", "comet", "deluxo", "burrito", "spand", "marquis",
	"baggage", "kaufman", "maverick", "vcnmav", "rancher", "fbiranch", "virgo", "greenwoo", "jetmax", "hotring",
	"sandking", "blistac", "polmav", "boxville", "benson", "mesa", "rcgoblin", "hotrina", "hotrinb",
	"bloodra", "bloodrb", "vicechee"
};

static std::list<CTweakVar *> TweakVarsList;
static bool bAddTweakVarsNow = false;
static const char *pTweakVarsDefaultPath = NULL;

void CTweakVars::Add(CTweakVar *var)
{
	TweakVarsList.push_back(var);
	
	if ( bAddTweakVarsNow )
		var->AddDBG(pTweakVarsDefaultPath);
}

void CTweakVars::AddDBG(const char *path)
{
	pTweakVarsDefaultPath = path;

	for(auto i = TweakVarsList.begin(); i != TweakVarsList.end(); ++i)
		(*i)->AddDBG(pTweakVarsDefaultPath);
	
	bAddTweakVarsNow = true;
}

void CTweakSwitch::AddDBG(const char *path)
{		
	DebugMenuEntry *e = DebugMenuAddVar(m_pPath == NULL ? path : m_pPath, m_pVarName, (int32_t *)m_pIntVar, m_pFunc, 1, m_nMin, m_nMax, m_aStr);
	DebugMenuEntrySetWrap(e, true);
}
	
void CTweakFunc::AddDBG  (const char *path) { DebugMenuAddCmd     (m_pPath == NULL ? path : m_pPath, m_pVarName, m_pFunc); }
void CTweakBool::AddDBG  (const char *path) { DebugMenuAddVarBool8(m_pPath == NULL ? path : m_pPath, m_pVarName, (int8_t *)m_pBoolVar,  NULL); }
void CTweakInt8::AddDBG  (const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (int8_t *)m_pIntVar,   NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL); }
void CTweakUInt8::AddDBG (const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (uint8_t *)m_pIntVar,  NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL); }
void CTweakInt16::AddDBG (const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (int16_t *)m_pIntVar,  NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL); }
void CTweakUInt16::AddDBG(const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (uint16_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL); }
void CTweakInt32::AddDBG (const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (int32_t *)m_pIntVar,  NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL); }
void CTweakUInt32::AddDBG(const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (uint32_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL); }
void CTweakFloat::AddDBG (const char *path) { DebugMenuAddVar     (m_pPath == NULL ? path : m_pPath, m_pVarName, (float *)m_pIntVar,    NULL, m_nStep, m_nLoawerBound, m_nUpperBound); }

/*
static const char *wt[] = {
			"Sunny", "Cloudy", "Rainy", "Foggy"
		};

SETTWEAKPATH("TEST");		
TWEAKSWITCH(CWeather::NewWeatherType, 0, 3, wt, NULL);
*/

void
DebugMenuPopulate(void)
{
	if(1){
		static const char *weathers[] = {
			"Sunny", "Cloudy", "Rainy", "Foggy"
		};
		DebugMenuEntry *e;
		e = DebugMenuAddVar("Time & Weather", "Current Hour", &CClock::GetHoursRef(), nil, 1, 0, 23, nil);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Current Minute", &CClock::GetMinutesRef(),
			[](){ CWeather::InterpolationValue = CClock::GetMinutes()/60.0f; }, 1, 0, 59, nil);
			DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "Old Weather", (int16*)&CWeather::OldWeatherType, nil, 1, 0, 3, weathers);
		DebugMenuEntrySetWrap(e, true);
		e = DebugMenuAddVar("Time & Weather", "New Weather", (int16*)&CWeather::NewWeatherType, nil, 1, 0, 3, weathers);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Time & Weather", "Wind", (float*)&CWeather::Wind, nil, 0.1f, 0.0f, 1.0f);
		DebugMenuAddVar("Time & Weather", "Time scale", (float*)&CTimer::GetTimeScale(), nil, 0.1f, 0.0f, 10.0f);

		DebugMenuAddCmd("Cheats", "Weapons", WeaponCheat);
		DebugMenuAddCmd("Cheats", "Money", MoneyCheat);
		DebugMenuAddCmd("Cheats", "Health", HealthCheat);
		DebugMenuAddCmd("Cheats", "Wanted level up", WantedLevelUpCheat);
		DebugMenuAddCmd("Cheats", "Wanted level down", WantedLevelDownCheat);
		DebugMenuAddCmd("Cheats", "Tank", TankCheat);
		DebugMenuAddCmd("Cheats", "Blow up cars", BlowUpCarsCheat);
		DebugMenuAddCmd("Cheats", "Change player", ChangePlayerCheat);
		DebugMenuAddCmd("Cheats", "Mayhem", MayhemCheat);
		DebugMenuAddCmd("Cheats", "Everybody attacks player", EverybodyAttacksPlayerCheat);
		DebugMenuAddCmd("Cheats", "Weapons for all", WeaponsForAllCheat);
		DebugMenuAddCmd("Cheats", "Fast time", FastTimeCheat);
		DebugMenuAddCmd("Cheats", "Slow time", SlowTimeCheat);
		DebugMenuAddCmd("Cheats", "Armour", ArmourCheat);
		DebugMenuAddCmd("Cheats", "Sunny weather", SunnyWeatherCheat);
		DebugMenuAddCmd("Cheats", "Cloudy weather", CloudyWeatherCheat);
		DebugMenuAddCmd("Cheats", "Rainy weather", RainyWeatherCheat);
		DebugMenuAddCmd("Cheats", "Foggy weather", FoggyWeatherCheat);
		DebugMenuAddCmd("Cheats", "Fast weather", FastWeatherCheat);
		DebugMenuAddCmd("Cheats", "Only render wheels", OnlyRenderWheelsCheat);
		DebugMenuAddCmd("Cheats", "Chitty chitty bang bang", ChittyChittyBangBangCheat);
		DebugMenuAddCmd("Cheats", "Strong grip", StrongGripCheat);
		DebugMenuAddCmd("Cheats", "Nasty limbs", NastyLimbsCheat);

		static int spawnCarId = MI_LANDSTAL;
		e = DebugMenuAddVar("Spawn", "Spawn Car ID", &spawnCarId, nil, 1, MI_LANDSTAL, MI_VICECHEE, carnames);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddCmd("Spawn", "Spawn Car", [](){
			if(spawnCarId == MI_CHOPPER ||
			   spawnCarId == MI_AIRTRAIN ||
			   spawnCarId == MI_DEADDODO)
				return;
			SpawnCar(spawnCarId);
		});
		static uint8 dummy;
		carCol1 = DebugMenuAddVar("Spawn", "First colour", &dummy, nil, 1, 0, 255, nil);
		carCol2 = DebugMenuAddVar("Spawn", "Second colour", &dummy, nil, 1, 0, 255, nil);
		DebugMenuAddCmd("Spawn", "Spawn Stinger", [](){ SpawnCar(MI_STINGER); });
		DebugMenuAddCmd("Spawn", "Spawn Infernus", [](){ SpawnCar(MI_INFERNUS); });
		DebugMenuAddCmd("Spawn", "Spawn Cheetah", [](){ SpawnCar(MI_CHEETAH); });
		DebugMenuAddCmd("Spawn", "Spawn Esperanto", [](){ SpawnCar(MI_ESPERANT); });
		DebugMenuAddCmd("Spawn", "Spawn Stallion", [](){ SpawnCar(MI_STALLION); });
		DebugMenuAddCmd("Spawn", "Spawn Washington", [](){ SpawnCar(MI_WASHING); });
		DebugMenuAddCmd("Spawn", "Spawn Taxi", [](){ SpawnCar(MI_TAXI); });
		DebugMenuAddCmd("Spawn", "Spawn Police", [](){ SpawnCar(MI_POLICE); });
		DebugMenuAddCmd("Spawn", "Spawn Enforcer", [](){ SpawnCar(MI_ENFORCER); });
		DebugMenuAddCmd("Spawn", "Spawn Banshee", [](){ SpawnCar(MI_BANSHEE); });
		DebugMenuAddCmd("Spawn", "Spawn Cuban", [](){ SpawnCar(MI_CUBAN); });
		DebugMenuAddCmd("Spawn", "Spawn Voodoo", [](){ SpawnCar(MI_VOODOO); });
		DebugMenuAddCmd("Spawn", "Spawn Sparrow", [](){ SpawnCar(MI_SPARROW); });
		DebugMenuAddCmd("Spawn", "Spawn Rhino", [](){ SpawnCar(MI_RHINO); });
		DebugMenuAddCmd("Spawn", "Spawn Firetruck", [](){ SpawnCar(MI_FIRETRUCK); });
		DebugMenuAddCmd("Spawn", "Spawn Predator", [](){ SpawnCar(MI_PREDATOR); });

		DebugMenuAddVarBool8("Render", "Draw hud", &CHud::m_Wants_To_Draw_Hud, nil);
		DebugMenuAddVarBool8("Render", "Backface Culling", &gBackfaceCulling, nil);
#ifdef LIBRW
		DebugMenuAddVarBool8("Render", "PS2 Alpha test Emu", &gPS2alphaTest, nil);
#endif
		DebugMenuAddVarBool8("Render", "Frame limiter", &FrontEndMenuManager.m_PrefsFrameLimiter, nil);
		DebugMenuAddVarBool8("Render", "VSynch", &FrontEndMenuManager.m_PrefsVsync, nil);
		DebugMenuAddVar("Render", "Max FPS", &RsGlobal.maxFPS, nil, 1, 1, 1000, nil);
		DebugMenuAddVarBool8("Render", "Show Ped Paths", &gbShowPedPaths, nil);
		DebugMenuAddVarBool8("Render", "Show Car Paths", &gbShowCarPaths, nil);
		DebugMenuAddVarBool8("Render", "Show Car Path Links", &gbShowCarPathsLinks, nil);
		DebugMenuAddVarBool8("Render", "Show Collision Lines", &gbShowCollisionLines, nil);
		DebugMenuAddVarBool8("Render", "Show Collision Polys", &gbShowCollisionPolys, nil);
		DebugMenuAddVarBool8("Render", "Don't render Buildings", &gbDontRenderBuildings, nil);
		DebugMenuAddVarBool8("Render", "Don't render Big Buildings", &gbDontRenderBigBuildings, nil);
		DebugMenuAddVarBool8("Render", "Don't render Peds", &gbDontRenderPeds, nil);
		DebugMenuAddVarBool8("Render", "Don't render Vehicles", &gbDontRenderVehicles, nil);
		DebugMenuAddVarBool8("Render", "Don't render Objects", &gbDontRenderObjects, nil);

		DebugMenuAddVarBool8("Debug", "Edit on", &CSceneEdit::m_bEditOn, nil);
#ifdef MENU_MAP
		DebugMenuAddCmd("Debug", "Teleport to map waypoint", TeleportToWaypoint);
#endif
		DebugMenuAddVar("Debug", "Engine Status", &engineStatus, nil, 1, 0, 226, nil);
		DebugMenuAddCmd("Debug", "Set Engine Status", SetEngineStatus);
		DebugMenuAddCmd("Debug", "Fix Car", FixCar);
		DebugMenuAddCmd("Debug", "Toggle Comedy Controls", ToggleComedy);
		DebugMenuAddCmd("Debug", "Place Car on Road", PlaceOnRoad);

		DebugMenuAddVarBool8("Debug", "Catalina Heli On", &CHeli::CatalinaHeliOn, nil);
		DebugMenuAddCmd("Debug", "Catalina Fly By", CHeli::StartCatalinaFlyBy);
		DebugMenuAddCmd("Debug", "Catalina Take Off", CHeli::CatalinaTakeOff);
		DebugMenuAddCmd("Debug", "Catalina Fly Away", CHeli::MakeCatalinaHeliFlyAway);
		DebugMenuAddVarBool8("Debug", "Script Heli On", &CHeli::ScriptHeliOn, nil);

#ifdef TOGGLEABLE_BETA_FEATURES
		DebugMenuAddVarBool8("Debug", "Toggle popping heads on headshot", &CPed::bPopHeadsOnHeadshot, nil);
		DebugMenuAddVarBool8("Debug", "Toggle peds running to phones to report crimes", &CPed::bMakePedsRunToPhonesToReportCrimes, nil);
#endif

		DebugMenuAddCmd("Debug", "Start Credits", CCredits::Start);
		DebugMenuAddCmd("Debug", "Stop Credits", CCredits::Stop);

		extern bool PrintDebugCode;
		extern int16 DebugCamMode;
		DebugMenuAddVarBool8("Cam", "Use mouse Cam", &CCamera::m_bUseMouse3rdPerson, nil);
#ifdef FREE_CAM
		DebugMenuAddVarBool8("Cam", "Free Cam", &CCamera::bFreeCam, nil);
#endif
		DebugMenuAddVarBool8("Cam", "Print Debug Code", &PrintDebugCode, nil);
		DebugMenuAddVar("Cam", "Cam Mode", &DebugCamMode, nil, 1, 0, CCam::MODE_EDITOR, nil);
		DebugMenuAddCmd("Cam", "Normal", []() { DebugCamMode = 0; });
		DebugMenuAddCmd("Cam", "Follow Ped With Bind", []() { DebugCamMode = CCam::MODE_FOLLOW_PED_WITH_BIND; });
		DebugMenuAddCmd("Cam", "Reaction", []() { DebugCamMode = CCam::MODE_REACTION; });
		DebugMenuAddCmd("Cam", "Chris", []() { DebugCamMode = CCam::MODE_CHRIS; });
		DebugMenuAddCmd("Cam", "Reset Statics", ResetCamStatics);

		CTweakVars::AddDBG("Debug");
	}
}
#endif

const int   re3_buffsize = 1024;
static char re3_buff[re3_buffsize];

void re3_assert(const char *expr, const char *filename, unsigned int lineno, const char *func)
{
	int nCode;

	strcpy_s(re3_buff, re3_buffsize, "Assertion failed!" );
	strcat_s(re3_buff, re3_buffsize, "\n" );	
	
	strcat_s(re3_buff, re3_buffsize, "File: ");
	strcat_s(re3_buff, re3_buffsize, filename );
	strcat_s(re3_buff, re3_buffsize, "\n" );	

	strcat_s(re3_buff, re3_buffsize, "Line: " );
	_itoa_s( lineno, re3_buff + strlen(re3_buff), re3_buffsize - strlen(re3_buff), 10 );
	strcat_s(re3_buff, re3_buffsize, "\n");
	
	strcat_s(re3_buff, re3_buffsize, "Function: ");
	strcat_s(re3_buff, re3_buffsize, func );
	strcat_s(re3_buff, re3_buffsize, "\n" );	
	
	strcat_s(re3_buff, re3_buffsize, "Expression: ");
	strcat_s(re3_buff, re3_buffsize, expr);
	strcat_s(re3_buff, re3_buffsize, "\n");

	strcat_s(re3_buff, re3_buffsize, "\n" );
	strcat_s(re3_buff, re3_buffsize, "(Press Retry to debug the application)");


	nCode = ::MessageBoxA(nil, re3_buff, "RE3 Assertion Failed!",
		MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

	if (nCode == IDABORT)
	{
		raise(SIGABRT);
		_exit(3);
	}

	if (nCode == IDRETRY)
	{
		__debugbreak();
		return;
	}

	if (nCode == IDIGNORE)
		return;

	abort();
}

void re3_debug(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	vsprintf_s(re3_buff, re3_buffsize, format, va);
	va_end(va);

	printf("%s", re3_buff);
	CDebug::DebugAddText(re3_buff);
}

void re3_trace(const char *filename, unsigned int lineno, const char *func, const char *format, ...)
{
	char buff[re3_buffsize *2];
	va_list va;
	va_start(va, format);
	vsprintf_s(re3_buff, re3_buffsize, format, va);
	va_end(va);
	
	sprintf_s(buff, re3_buffsize * 2, "[%s.%s:%d]: %s", filename, func, lineno, re3_buff);
	
	OutputDebugStringA(buff);
}

#ifdef VALIDATE_SAVE_SIZE
int32 _saveBufCount;
#endif
