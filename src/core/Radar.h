#pragma once
#include "Sprite2d.h"

#define MENU_MAP_LENGTH_UNIT 1190.0f // in game unit
#define MENU_MAP_WIDTH_SCALE 1.112f // in game unit (originally 1.112494151260504f)
#define MENU_MAP_HEIGHT_SCALE 1.119f // in game unit (originally 1.118714268907563f)
#define MENU_MAP_TOP_OFFSET 0.28f // in length unit defined above - ~333 game unit
#define MENU_MAP_LEFT_OFFSET 0.185f // in length unit defined above - ~220 game unit
#define MENU_MAP_LENGTH (4000.f / MENU_MAP_LENGTH_UNIT)

enum eBlipType
{
	BLIP_NONE,
	BLIP_CAR,
	BLIP_CHAR,
	BLIP_OBJECT,
	BLIP_COORD,
	BLIP_CONTACT_POINT
};

enum eBlipDisplay
{
	BLIP_DISPLAY_NEITHER = 0,
	BLIP_DISPLAY_MARKER_ONLY = 1,
	BLIP_DISPLAY_BLIP_ONLY = 2,
	BLIP_DISPLAY_BOTH = 3,
};

enum eRadarSprite
{
#ifdef MENU_MAP
	RADAR_SPRITE_ENTITY_BLIP = -2,
	RADAR_SPRITE_COORD_BLIP = -1,
#endif
	RADAR_SPRITE_NONE = 0,
	RADAR_SPRITE_ASUKA = 1,
	RADAR_SPRITE_BOMB = 2,
	RADAR_SPRITE_CAT = 3,
	RADAR_SPRITE_CENTRE = 4,
	RADAR_SPRITE_COPCAR = 5,
	RADAR_SPRITE_DON = 6,
	RADAR_SPRITE_EIGHT = 7,
	RADAR_SPRITE_EL = 8,
	RADAR_SPRITE_ICE = 9,
	RADAR_SPRITE_JOEY = 10,
	RADAR_SPRITE_KENJI = 11,
	RADAR_SPRITE_LIZ = 12,
	RADAR_SPRITE_LUIGI = 13,
	RADAR_SPRITE_NORTH = 14,
	RADAR_SPRITE_RAY = 15,
	RADAR_SPRITE_SAL = 16,
	RADAR_SPRITE_SAVE = 17,
	RADAR_SPRITE_SPRAY = 18,
	RADAR_SPRITE_TONY = 19,
	RADAR_SPRITE_WEAPON = 20,
	RADAR_SPRITE_COUNT = 21,
};

enum
{
	BLIP_MODE_TRIANGULAR_UP = 0,
	BLIP_MODE_TRIANGULAR_DOWN,
	BLIP_MODE_SQUARE,
};

struct sRadarTrace
{
	uint32 m_nColor;
	uint32 m_eBlipType; // eBlipType
	int32 m_nEntityHandle;
	CVector2D m_vec2DPos;
	CVector m_vecPos;
	uint16 m_BlipIndex;
	bool m_bDim;
	bool m_bInUse;
	bool m_bShortRange;
	float m_Radius;
	int16 m_wScale;
	uint16 m_eBlipDisplay; // eBlipDisplay
	uint16 m_eRadarSprite; // eRadarSprite
};

// Values for screen space
#define RADAR_LEFT (40.0f)
#define RADAR_BOTTOM (47.0f)
#define RADAR_WIDTH (94.0f)
#define RADAR_HEIGHT (76.0f)

class CRadar
{
public:
	static float m_radarRange;
	static sRadarTrace ms_RadarTrace[NUMRADARBLIPS];
	static CSprite2d AsukaSprite;
	static CSprite2d BombSprite;
	static CSprite2d CatSprite;
	static CSprite2d CentreSprite;
	static CSprite2d CopcarSprite;
	static CSprite2d DonSprite;
	static CSprite2d EightSprite;
	static CSprite2d ElSprite;
	static CSprite2d IceSprite;
	static CSprite2d JoeySprite;
	static CSprite2d KenjiSprite;
	static CSprite2d LizSprite;
	static CSprite2d LuigiSprite;
	static CSprite2d NorthSprite;
	static CSprite2d RaySprite;
	static CSprite2d SalSprite;
	static CSprite2d SaveSprite;
	static CSprite2d SpraySprite;
	static CSprite2d TonySprite;
	static CSprite2d WeaponSprite;
	static CSprite2d *RadarSprites[21];
	static float cachedCos;
	static float cachedSin;
#ifdef MENU_MAP
#define NUM_MAP_LEGENDS 75
	static CRGBA ArrowBlipColour1;
	static CRGBA ArrowBlipColour2;
	static int16 MapLegendList[NUM_MAP_LEGENDS];
	static uint16 MapLegendCounter;
	static int TargetMarkerId;
	static CVector TargetMarkerPos;

	static void InitFrontEndMap();
	static void DrawYouAreHereSprite(float, float);
	static void ToggleTargetMarker(float, float);
#endif
	static uint8 CalculateBlipAlpha(float dist);
	static void ChangeBlipBrightness(int32 i, int32 bright);
	static void ChangeBlipColour(int32 i, int32);
	static void ChangeBlipDisplay(int32 i, eBlipDisplay display);
	static void ChangeBlipScale(int32 i, int32 scale);
	static void ClearBlip(int32 i);
	static void ClearBlipForEntity(eBlipType type, int32 id);
	static int ClipRadarPoly(CVector2D *out, const CVector2D *in);
	static bool DisplayThisBlip(int32 i);
	static void Draw3dMarkers();
	static void DrawBlips();
	static void DrawMap();
	static void DrawRadarMap();
	static void DrawRadarMask();
	static void DrawRadarSection(int32 x, int32 y);
	static void DrawRadarSprite(uint16 sprite, float x, float y, uint8 alpha);
	static void DrawRotatingRadarSprite(CSprite2d* sprite, float x, float y, float angle, int32 alpha);
	static int32 GetActualBlipArrayIndex(int32 i);
	static int32 GetNewUniqueBlipIndex(int32 i);
	static uint32 GetRadarTraceColour(uint32 color, bool bright);
	static void Initialise();
	static float LimitRadarPoint(CVector2D &point);
	static void LoadAllRadarBlips(uint8 *buf, uint32 size);
	static void LoadTextures();
	static void RemoveRadarSections();
	static void SaveAllRadarBlips(uint8*, uint32*);
	static void SetBlipSprite(int32 i, int32 icon);
	static int32 SetCoordBlip(eBlipType type, CVector pos, int32, eBlipDisplay);
	static int32 SetEntityBlip(eBlipType type, int32, int32, eBlipDisplay);
	static int32 SetShortRangeCoordBlip(eBlipType type, CVector pos, int32, eBlipDisplay);
	static void SetRadarMarkerState(int32 i, bool flag);
	static void ShowRadarMarker(CVector pos, uint32 color, float radius);
	static void ShowRadarTrace(float x, float y, uint32 size, uint8 red, uint8 green, uint8 blue, uint8 alpha);
	static void ShowRadarTraceWithHeight(float x, float y, uint32 size, uint8 red, uint8 green, uint8 blue, uint8 alpha, uint8 mode);
	static void Shutdown();
	static void StreamRadarSections(const CVector &posn);
	static void StreamRadarSections(int32 x, int32 y);
	static void TransformRealWorldToTexCoordSpace(CVector2D &out, const CVector2D &in, int32 x, int32 y);
	static void TransformRadarPointToRealWorldSpace(CVector2D &out, const CVector2D &in);
	static void TransformRadarPointToScreenSpace(CVector2D &out, const CVector2D &in);
	static void TransformRealWorldPointToRadarSpace(CVector2D &out, const CVector2D &in);

	// no in CRadar in the game:	
	static void CalculateCachedSinCos();
};
