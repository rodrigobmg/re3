#pragma

#include "Sprite2d.h"

#define MENUHEADER_POS_X 35.0f
#define MENUHEADER_POS_Y 93.0f
#define MENUHEADER_WIDTH 0.84f
#define MENUHEADER_HEIGHT 1.6f

#define MENUACTION_POS_X 20.0f
#define MENUACTION_POS_Y 37.5f
#define MENUACTION_WIDTH 0.675f
#define MENUACTION_HEIGHT 0.81f

#define MENUCOLUMN_POS_X MENUHEADER_POS_X + 16.0f
#define MENUCOLUMN_MAX_Y 149.0f
#define MENUCOLUMN_MID_Y 100.0f
#define MENUCOLUMN_MIN_Y 110.0f
#define MENUCOLUMN_PAUSE_Y 25.0f
#define MENUCOLUMN_START_Y 9.0f
#define MENUCOLUMN_FEDS 139.0f

#define MENUCOLUMN_SAVE_X 121.0f
#define MENUCOLUMN_SAVE_Y 111.0f

#define MENUCOLUMN_SPACING_MAX 24.0f
#define MENUCOLUMN_SPACING_MIN 20.0f

#define MENUSELECT_BOX_MAX 20.5f
#define MENUSELECT_BOX_MIN 17.0f

#ifndef ASPECT_RATIO_SCALE
#define MENURADIO_ICON_X 31.5f
#else
#define MENURADIO_ICON_X -262.0f
#endif
#define MENURADIO_ICON_Y 29.5f
#define MENURADIO_ICON_W 60.0f
#define MENURADIO_ICON_H 60.0f

#define MENUDROP_COLOR_A 150
#define MENUDROP_COLOR_SIZE -1

#define MENUSLIDER_X 306.0f

#define buf(a) (char*)(a)

enum eLanguages
{
	LANGUAGE_AMERICAN,
	LANGUAGE_FRENCH,
	LANGUAGE_GERMAN,
	LANGUAGE_ITALIAN,
	LANGUAGE_SPANISH,
};

enum eFrontendSprites
{
	FE2_MAINPANEL_UL,
	FE2_MAINPANEL_UR,
	FE2_MAINPANEL_DL,
	FE2_MAINPANEL_DR,
	FE2_MAINPANEL_DR2,
	FE2_TABACTIVE,
	FE_ICONBRIEF,
	FE_ICONSTATS,
	FE_ICONCONTROLS,
	FE_ICONSAVE,
	FE_ICONAUDIO,
	FE_ICONDISPLAY,
	FE_ICONLANGUAGE,
	FE_CONTROLLER,
	FE_CONTROLLERSH,
	FE_ARROWS1,
	FE_ARROWS2,
	FE_ARROWS3,
	FE_ARROWS4,
	FE_RADIO1,
	FE_RADIO2,
	FE_RADIO3,
	FE_RADIO4,
	FE_RADIO5,
	FE_RADIO6,
	FE_RADIO7,
	FE_RADIO8,
	FE_RADIO9,
};

enum eMenuSprites
{
	MENUSPRITE_CONNECTION,
	MENUSPRITE_FINDGAME,
	MENUSPRITE_HOSTGAME,
	MENUSPRITE_MAINMENU,
	MENUSPRITE_PLAYERSET,
	MENUSPRITE_SINGLEPLAYER,
	MENUSPRITE_MULTIPLAYER,
	MENUSPRITE_DMALOGO,
	MENUSPRITE_GTALOGO,
	MENUSPRITE_RSTARLOGO,
	MENUSPRITE_GAMESPY,
	MENUSPRITE_MOUSE,
	MENUSPRITE_MOUSET,
	MENUSPRITE_MP3LOGO,
	MENUSPRITE_DOWNOFF,
	MENUSPRITE_DOWNON,
	MENUSPRITE_UPOFF,
	MENUSPRITE_UPON,
	MENUSPRITE_GTA3LOGO,
};

enum eSaveSlot
{
	SAVESLOT_NONE,
	SAVESLOT_0,
	SAVESLOT_1,
	SAVESLOT_2,
	SAVESLOT_3,
	SAVESLOT_4,
	SAVESLOT_5,
	SAVESLOT_6,
	SAVESLOT_7,
	SAVESLOT_8,
	SAVESLOT_LABEL = 36
};

enum eMenuScreen
{
	MENUPAGE_DISABLED = -1,
	MENUPAGE_NONE = 0,
	MENUPAGE_STATS = 1,
	MENUPAGE_NEW_GAME = 2,
	MENUPAGE_BRIEFS = 3,
	MENUPAGE_CONTROLLER_SETTINGS = 4,
	MENUPAGE_SOUND_SETTINGS = 5,
	MENUPAGE_GRAPHICS_SETTINGS = 6,
	MENUPAGE_LANGUAGE_SETTINGS = 7,
	MENUPAGE_CHOOSE_LOAD_SLOT = 8,
	MENUPAGE_CHOOSE_DELETE_SLOT = 9,
	MENUPAGE_NEW_GAME_RELOAD = 10,
	MENUPAGE_LOAD_SLOT_CONFIRM = 11,
	MENUPAGE_DELETE_SLOT_CONFIRM = 12,
	MENUPAGE_13 = 13,
	MENUPAGE_LOADING_IN_PROGRESS = 14,
	MENUPAGE_DELETING_IN_PROGRESS = 15,
	MENUPAGE_16 = 16,
	MENUPAGE_DELETE_FAILED = 17,
	MENUPAGE_DEBUG_MENU = 18,
	MENUPAGE_MEMORY_CARD_1 = 19,
	MENUPAGE_MEMORY_CARD_2 = 20,
	MENUPAGE_MULTIPLAYER_MAIN = 21,
	MENUPAGE_SAVE_FAILED_1 = 22,
	MENUPAGE_SAVE_FAILED_2 = 23,
	MENUPAGE_SAVE = 24,
	MENUPAGE_NO_MEMORY_CARD = 25,
	MENUPAGE_CHOOSE_SAVE_SLOT = 26,
	MENUPAGE_SAVE_OVERWRITE_CONFIRM = 27,
	MENUPAGE_MULTIPLAYER_MAP = 28,
	MENUPAGE_MULTIPLAYER_CONNECTION = 29,
	MENUPAGE_MULTIPLAYER_FIND_GAME = 30,
	MENUPAGE_MULTIPLAYER_MODE = 31,
	MENUPAGE_MULTIPLAYER_CREATE = 32,
	MENUPAGE_MULTIPLAYER_START = 33,
	MENUPAGE_SKIN_SELECT_OLD = 34,
	MENUPAGE_CONTROLLER_PC = 35,
	MENUPAGE_CONTROLLER_PC_OLD1 = 36,
	MENUPAGE_CONTROLLER_PC_OLD2 = 37,
	MENUPAGE_CONTROLLER_PC_OLD3 = 38,
	MENUPAGE_CONTROLLER_PC_OLD4 = 39,
	MENUPAGE_CONTROLLER_DEBUG = 40,
	MENUPAGE_OPTIONS = 41,
	MENUPAGE_EXIT = 42,
	MENUPAGE_SAVING_IN_PROGRESS = 43,
	MENUPAGE_SAVE_SUCCESSFUL = 44,
	MENUPAGE_DELETING = 45,
	MENUPAGE_DELETE_SUCCESS = 46,
	MENUPAGE_SAVE_FAILED = 47,
	MENUPAGE_LOAD_FAILED = 48,
	MENUPAGE_LOAD_FAILED_2 = 49,
	MENUPAGE_FILTER_GAME = 50,
	MENUPAGE_START_MENU = 51,
	MENUPAGE_PAUSE_MENU = 52,
	MENUPAGE_CHOOSE_MODE = 53,
	MENUPAGE_SKIN_SELECT = 54,
	MENUPAGE_KEYBOARD_CONTROLS = 55,
	MENUPAGE_MOUSE_CONTROLS = 56,
	MENUPAGE_57 = 57,
	MENUPAGE_58 = 58,
	MENUPAGES
};

enum eMenuAction
{
	MENUACTION_NOTHING,
	MENUACTION_LABEL,
	MENUACTION_CHANGEMENU,
	MENUACTION_CTRLVIBRATION,
	MENUACTION_CTRLCONFIG,
	MENUACTION_CTRLDISPLAY,
	MENUACTION_FRAMESYNC,
	MENUACTION_FRAMELIMIT,
	MENUACTION_TRAILS,
	MENUACTION_SUBTITLES,
	MENUACTION_WIDESCREEN,
	MENUACTION_BRIGHTNESS,
	MENUACTION_DRAWDIST,
	MENUACTION_MUSICVOLUME,
	MENUACTION_SFXVOLUME,
	MENUACTION_UNK15,
	MENUACTION_RADIO,
	MENUACTION_LANG_ENG,
	MENUACTION_LANG_FRE,
	MENUACTION_LANG_GER,
	MENUACTION_LANG_ITA,
	MENUACTION_LANG_SPA,
	MENUACTION_UPDATESAVE,
	MENUACTION_CHECKSAVE,
	MENUACTION_UNK24,
	MENUACTION_NEWGAME,
	MENUACTION_RELOADIDE,
	MENUACTION_RELOADIPL,
	MENUACTION_SETDBGFLAG,
	MENUACTION_SWITCHBIGWHITEDEBUGLIGHT,
	MENUACTION_PEDROADGROUPS,
	MENUACTION_CARROADGROUPS,
	MENUACTION_COLLISIONPOLYS,
	MENUACTION_REGMEMCARD1,
	MENUACTION_TESTFORMATMEMCARD1,
	MENUACTION_TESTUNFORMATMEMCARD1,
	MENUACTION_CREATEROOTDIR,
	MENUACTION_CREATELOADICONS,
	MENUACTION_FILLWITHGUFF,
	MENUACTION_SAVEONLYTHEGAME,
	MENUACTION_SAVEGAME,
	MENUACTION_SAVEGAMEUNDERGTA,
	MENUACTION_CREATECOPYPROTECTED,
	MENUACTION_TESTSAVE,
	MENUACTION_TESTLOAD,
	MENUACTION_TESTDELETE,
	MENUACTION_PARSEHEAP,
	MENUACTION_SHOWCULL,
	MENUACTION_MEMCARDSAVECONFIRM,
	MENUACTION_UPDATEMEMCARDSAVE,
	MENUACTION_UNK50,
	MENUACTION_DEBUGSTREAM,
	MENUACTION_MPMAP_LIBERTY,
	MENUACTION_MPMAP_REDLIGHT,
	MENUACTION_MPMAP_CHINATOWN,
	MENUACTION_MPMAP_TOWER,
	MENUACTION_MPMAP_SEWER,
	MENUACTION_MPMAP_INDUSTPARK,
	MENUACTION_MPMAP_DOCKS,
	MENUACTION_MPMAP_STAUNTON,
	MENUACTION_MPMAP_DEATHMATCH1,
	MENUACTION_MPMAP_DEATHMATCH2,
	MENUACTION_MPMAP_TEAMDEATH1,
	MENUACTION_MPMAP_TEAMDEATH2,
	MENUACTION_MPMAP_STASH,
	MENUACTION_MPMAP_CAPTURE,
	MENUACTION_MPMAP_RATRACE,
	MENUACTION_MPMAP_DOMINATION,
	MENUACTION_STARTMP,
	MENUACTION_UNK69,
	MENUACTION_UNK70,
	MENUACTION_FINDMP,
	MENUACTION_REDEFCTRL,
	MENUACTION_UNK73,
	MENUACTION_INITMP,
	MENUACTION_MP_PLAYERCOLOR,
	MENUACTION_MP_PLAYERNAME,
	MENUACTION_MP_GAMENAME,
	MENUACTION_GETKEY,
	MENUACTION_SHOWHEADBOB,
	MENUACTION_UNK80,
	MENUACTION_INVVERT,
	MENUACTION_CANCLEGAME,
	MENUACTION_MP_PLAYERNUMBER,
	MENUACTION_MOUSESENS,
	MENUACTION_CHECKMPGAMES,
	MENUACTION_CHECKMPPING,
	MENUACTION_MP_SERVER,
	MENUACTION_MP_MAP,
	MENUACTION_MP_GAMETYPE,
	MENUACTION_MP_LAN,
	MENUACTION_MP_INTERNET,
	MENUACTION_RESUME,
	MENUACTION_DONTCANCLE,
	MENUACTION_SCREENRES,
	MENUACTION_AUDIOHW,
	MENUACTION_SPEAKERCONF,
	MENUACTION_PLAYERSETUP,
	MENUACTION_RESTOREDEF,
	MENUACTION_CTRLMETHOD,
	MENUACTION_DYNAMICACOUSTIC,
	MENUACTION_LOADRADIO,
	MENUACTION_MOUSESTEER,
	MENUACTION_UNK103,
	MENUACTION_UNK104,
	MENUACTION_UNK105,
	MENUACTION_UNK106,
	MENUACTION_UNK107,
	MENUACTION_UNK108,
	MENUACTION_UNK109,
	MENUACTION_UNK110,
};

enum eCheckHover
{
	HOVEROPTION_0,
	HOVEROPTION_1,
	HOVEROPTION_DEFAULT,
	HOVEROPTION_3,
	HOVEROPTION_4,
	HOVEROPTION_5,
	HOVEROPTION_6,
	HOVEROPTION_7,
	HOVEROPTION_8,
	HOVEROPTION_9,
	HOVEROPTION_10,
	HOVEROPTION_11,
	HOVEROPTION_12,
	HOVEROPTION_13,
	HOVEROPTION_14,
	HOVEROPTION_15,
	HOVEROPTION_16,
	HOVEROPTION_17,
	HOVEROPTION_18,
	HOVEROPTION_19,
	HOVEROPTION_20,
	HOVEROPTION_CHANGESKIN,
	HOVEROPTION_NULL = 42,
};

enum eMenuColumns
{ 
	MENUCOLUMN_LEFT,
	MENUCOLUMN_CENTER,
	MENUCOLUMN_RIGHT,
	MENUCOLUMNS,
};

enum eMenuRow
{
	MENUROW_NONE = -1,
	MENUROW_0,
	MENUROW_1,
	MENUROW_2,
	MENUROW_3,
	MENUROW_4,
	MENUROW_5,
	MENUROW_6,
	MENUROW_7,
	MENUROW_8,
	MENUROW_9,
	MENUROW_10,
	MENUROW_11,
	MENUROW_12,
	MENUROW_13,
	MENUROW_14,
	MENUROW_15,
	MENUROW_16,
	MENUROW_17,
	MENUROWS,
};

struct tSkinInfo
{
	int field_0;
	char skinName[256];
	char currSkinName[256];
	char date[256];
	tSkinInfo *field_304;
};

struct CMenuScreen
{
	char m_ScreenName[8];
	int32 unk;
	int32 m_PreviousPage[2]; // eMenuScreen
	int32 m_ParentEntry[2]; // eMenuRow

	struct CMenuEntry
	{
		int32 m_Action; // eMenuAction
		char m_EntryName[8];
		int32 m_SaveSlot; // eSaveSlot
		int32 m_TargetMenu; // eMenuScreen
	} m_aEntries[MENUROWS];
};

class CMenuManager
{
public:
	int32 m_nPrefsVideoMode;
	int32 m_nDisplayVideoMode;
	int8 m_nPrefsAudio3DProviderIndex;
	bool m_bKeyChangeNotProcessed;
	char m_aSkinName[256];
	int32 m_nHelperTextMsgId;
	bool m_bLanguageLoaded;
	bool m_bMenuActive;
 char field_112;
 char field_113;
	bool m_bStartGameLoading;
	bool m_bFirstTime;
	bool m_bGameNotLoaded;
	int32 m_nMousePosX;
	int32 m_nMousePosY;
	int32 m_nMouseTempPosX;
	int32 m_nMouseTempPosY;
	bool m_bShowMouse;
	tSkinInfo m_sSkin;
	tSkinInfo *m_pSelectedSkin;
 tSkinInfo *field_438;
 float field_43C;
 int field_440;
	int m_nSkinsTotal;
 char _unk0[4];
 int field_44C;
	bool m_bSkinsFound;
	bool m_bQuitGameNoCD;
 char field_452;
	bool m_bSaveMenuActive;
	bool m_bLoadingSavedGame;
 char field_455;
 char field_456;
	bool m_bSpritesLoaded;
	CSprite2d m_aFrontEndSprites[28];
	CSprite2d m_aMenuSprites[20];
 int field_518;
	int m_nMenuFadeAlpha;
 char field_520;
 char field_521;
 char field_522;
 char field_523;
 char field_524;
	int m_CurrCntrlAction;
 char _unk1[4];
 int field_530;
 char field_534;
 char field_535;
 int8 field_536;
	int m_nHelperTextAlpha;
	int m_nMouseOldPosX;
	int m_nMouseOldPosY;
	int m_nHoverOption;
	int m_nCurrScreen;
	int m_nCurrOption;
	int m_nPrevOption;
	int m_nPrevScreen;
 int field_558;
	int m_nCurrSaveSlot;
	int m_nScreenChangeDelayTimer;

	static int32 &OS_Language;
	static int8 &m_PrefsUseVibration;
	static int8 &m_DisplayControllerOnFoot;
	static int8 &m_PrefsUseWideScreen;
	static int8 &m_PrefsRadioStation;
	static int8 &m_PrefsVsync;
	static int8 &m_PrefsVsyncDisp;
	static int8 &m_PrefsFrameLimiter;
	static int8 &m_PrefsShowSubtitles;
	static int8 &m_PrefsSpeakers;
	static int8 &m_ControlMethod;
	static int8 &m_PrefsDMA;
	static int8 &m_PrefsLanguage;
	static int8 &m_bDisableMouseSteering;
	static int32 &m_PrefsBrightness;
	static float &m_PrefsLOD;
	static int8 &m_bFrontEnd_ReloadObrTxtGxt;
	static int32 &m_PrefsMusicVolume;
	static int32 &m_PrefsSfxVolume;
	static char *m_PrefsSkinFile;
	static int32 &m_KeyPressedCode;

	static bool &m_bStartUpFrontEndRequested;
	static bool &m_bShutDownFrontEndRequested;
	static bool &m_PrefsAllowNastyGame;

public:
	void BuildStatLine(char *text, float *stat, bool aFloat, float* stat2);
	static void CentreMousePointer();
	int CheckCodesForControls(int32);
	bool CheckHover(int x1, int x2, int y1, int y2);
	void CheckSliderMovement(int);
	int CostructStatLine(int);
	void DisplayHelperText();
	float DisplaySlider(float, float, float, float, float, float);
	void DoSettingsBeforeStartingAGame();
	void Draw();
	void DrawControllerBound(int, int, int, uint8);
	void DrawControllerScreenExtraText(int, int, int);
	void DrawControllerSetupScreen();
	void DrawFrontEnd();
	void DrawFrontEndNormal();
	void DrawPlayerSetupScreen();
	int FadeIn(int alpha);
	void FilterOutColorMarkersFromString(uint16, CRGBA &);
	int GetStartOptionsCntrlConfigScreens();
	static void InitialiseChangedLanguageSettings();
	void LoadAllTextures();
	void LoadSettings();
	static void MessageScreen(char *);
	static void PickNewPlayerColour();
	void PrintBriefs();
	static void PrintErrorMessage();
	void PrintStats();
	void Process();
	void ProcessButtonPresses();
	void ProcessOnOffMenuOptions();
	static void RequestFrontEndShutdown();
	static void RequestFrontEndStartUp();
	void ResetHelperText();
	void SaveLoadFileError_SetUpErrorScreen();
	void SaveSettings();
	void SetHelperText(int text);
	void ShutdownJustMenu();
	static float StretchX(float);
	static float StretchY(float);
	void SwitchMenuOnAndOff();
	void UnloadTextures();
	void WaitForUserCD();

	// New content:
	uint8 GetNumberOfMenuOptions();
	void SwitchToNewScreen(int8 screen);
	void SetDefaultPreferences(int8 screen);

};

static_assert(sizeof(CMenuManager) == 0x564, "CMenuManager: error");

extern CMenuManager &FrontEndMenuManager;
