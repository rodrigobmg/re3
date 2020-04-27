#pragma once

#include "common.h"
#include "config.h"

#include "DMAudio.h"

#include "AudioCollision.h"
#include "PoliceRadio.h"

enum eScriptSounds : int16 {
	SCRIPT_SOUND_0 = 0,
	SCRIPT_SOUND_1 = 1,
	SCRIPT_SOUND_2 = 2,
	SCRIPT_SOUND_3 = 3,
	SCRIPT_SOUND_PARTY_1_LOOP_S = 4,
	SCRIPT_SOUND_PARTY_1_LOOP_L = 5,
	SCRIPT_SOUND_PARTY_2_LOOP_S = 6,
	SCRIPT_SOUND_PARTY_2_LOOP_L = 7,
	SCRIPT_SOUND_PARTY_3_LOOP_S = 8,
	SCRIPT_SOUND_PARTY_3_LOOP_L = 9,
	SCRIPT_SOUND_PARTY_4_LOOP_S = 10,
	SCRIPT_SOUND_PARTY_4_LOOP_L = 11,
	SCRIPT_SOUND_PARTY_5_LOOP_S = 12,
	SCRIPT_SOUND_PARTY_5_LOOP_L = 13,
	SCRIPT_SOUND_PARTY_6_LOOP_S = 14,
	SCRIPT_SOUND_PARTY_6_LOOP_L = 15,
	SCRIPT_SOUND_PARTY_7_LOOP_S = 16,
	SCRIPT_SOUND_PARTY_7_LOOP_L = 17,
	SCRIPT_SOUND_PARTY_8_LOOP_S = 18,
	SCRIPT_SOUND_PARTY_8_LOOP_L = 19,
	SCRIPT_SOUND_PARTY_9_LOOP_S = 20,
	SCRIPT_SOUND_PARTY_9_LOOP_L = 21,
	SCRIPT_SOUND_PARTY_10_LOOP_S = 22,
	SCRIPT_SOUND_PARTY_10_LOOP_L = 23,
	SCRIPT_SOUND_PARTY_11_LOOP_S = 24,
	SCRIPT_SOUND_PARTY_11_LOOP_L = 25,
	SCRIPT_SOUND_PARTY_12_LOOP_S = 26,
	SCRIPT_SOUND_PARTY_12_LOOP_L = 27,
	SCRIPT_SOUND_PARTY_13_LOOP_S = 28,
	SCRIPT_SOUND_PARTY_13_LOOP_L = 29,
	SCRIPT_SOUND_STRIP_CLUB_LOOP_1_S = 30,
	SCRIPT_SOUND_STRIP_CLUB_LOOP_1_L = 31,
	SCRIPT_SOUND_STRIP_CLUB_LOOP_2_S = 32,
	SCRIPT_SOUND_STRIP_CLUB_LOOP_2_L = 33,
	SCRIPT_SOUND_WORK_SHOP_LOOP_S = 34,
	SCRIPT_SOUND_WORK_SHOP_LOOP_L = 35,
	SCRIPT_SOUND_SAWMILL_LOOP_S = 36,
	SCRIPT_SOUND_SAWMILL_LOOP_L = 37,
	SCRIPT_SOUND_38 = 38,
	SCRIPT_SOUND_39 = 39,
	SCRIPT_SOUND_LAUNDERETTE_LOOP_S = 40,
	SCRIPT_SOUND_LAUNDERETTE_LOOP_L = 41,
	SCRIPT_SOUND_CHINATOWN_RESTAURANT_S = 42,
	SCRIPT_SOUND_CHINATOWN_RESTAURANT_L = 43,
	SCRIPT_SOUND_CIPRIANI_RESAURANT_S = 44,
	SCRIPT_SOUND_CIPRIANI_RESAURANT_L = 45,
	SCRIPT_SOUND_46_S = 46,
	SCRIPT_SOUND_47_L = 47,
	SCRIPT_SOUND_MARCO_BISTRO_S = 48,
	SCRIPT_SOUND_MARCO_BISTRO_L = 49,
	SCRIPT_SOUND_AIRPORT_LOOP_S = 50,
	SCRIPT_SOUND_AIRPORT_LOOP_L = 51,
	SCRIPT_SOUND_SHOP_LOOP_S = 52,
	SCRIPT_SOUND_SHOP_LOOP_L = 53,
	SCRIPT_SOUND_CINEMA_LOOP_S = 54,
	SCRIPT_SOUND_CINEMA_LOOP_L = 55,
	SCRIPT_SOUND_DOCKS_LOOP_S = 56,
	SCRIPT_SOUND_DOCKS_LOOP_L = 57,
	SCRIPT_SOUND_HOME_LOOP_S = 58,
	SCRIPT_SOUND_HOME_LOOP_L = 59,
	SCRIPT_SOUND_FRANKIE_PIANO = 60,
	SCRIPT_SOUND_PARTY_1_LOOP = 61,
	SCRIPT_SOUND_PORN_CINEMA_1_S = 62,
	SCRIPT_SOUND_PORN_CINEMA_1_L = 63,
	SCRIPT_SOUND_PORN_CINEMA_2_S = 64,
	SCRIPT_SOUND_PORN_CINEMA_2_L = 65,
	SCRIPT_SOUND_PORN_CINEMA_3_S = 66,
	SCRIPT_SOUND_PORN_CINEMA_3_L = 67,
	SCRIPT_SOUND_BANK_ALARM_LOOP_S = 68,
	SCRIPT_SOUND_BANK_ALARM_LOOP_L = 69,
	SCRIPT_SOUND_POLICE_BALL_LOOP_S = 70,
	SCRIPT_SOUND_POLICE_BALL_LOOP_L = 71,
	SCRIPT_SOUND_RAVE_LOOP_INDUSTRIAL_S = 72,
	SCRIPT_SOUND_RAVE_LOOP_INDUSTRIAL_L = 73,
	SCRIPT_SOUND_74 = 74,
	SCRIPT_SOUND_75 = 75,
	SCRIPT_SOUND_POLICE_CELL_BEATING_LOOP_S = 76,
	SCRIPT_SOUND_POLICE_CELL_BEATING_LOOP_L = 77,
	SCRIPT_SOUND_INJURED_PED_MALE_OUCH_S = 78,
	SCRIPT_SOUND_INJURED_PED_MALE_OUCH_L = 79,
	SCRIPT_SOUND_INJURED_PED_FEMALE_OUCH_S = 80,
	SCRIPT_SOUND_INJURED_PED_FEMALE_OUCH_L = 81,
	SCRIPT_SOUND_EVIDENCE_PICKUP = 82,
	SCRIPT_SOUND_UNLOAD_GOLD = 83,
	SCRIPT_SOUND_RAVE_1_LOOP_S = 84,
	SCRIPT_SOUND_RAVE_1_LOOP_L = 85,
	SCRIPT_SOUND_RAVE_2_LOOP_S = 86,
	SCRIPT_SOUND_RAVE_2_LOOP_L = 87,
	SCRIPT_SOUND_RAVE_3_LOOP_S = 88,
	SCRIPT_SOUND_RAVE_3_LOOP_L = 89,
	SCRIPT_SOUND_MISTY_SEX_S = 90,
	SCRIPT_SOUND_MISTY_SEX_L = 91,
	SCRIPT_SOUND_GATE_START_CLUNK = 92,
	SCRIPT_SOUND_GATE_STOP_CLUNK = 93,
	SCRIPT_SOUND_PART_MISSION_COMPLETE = 94,
	SCRIPT_SOUND_CHUNKY_RUN_SHOUT = 95,
	SCRIPT_SOUND_SECURITY_GUARD_AWAY_SHOUT = 96,
	SCRIPT_SOUND_RACE_START_3 = 97,
	SCRIPT_SOUND_RACE_START_2 = 98,
	SCRIPT_SOUND_RACE_START_1 = 99,
	SCRIPT_SOUND_RACE_START_GO = 100,
	SCRIPT_SOUND_SWAT_PED_SHOUT = 101,
	SCRIPT_SOUND_PRETEND_FIRE_LOOP = 102,
	SCRIPT_SOUND_AMMUNATION_CHAT_1 = 103,
	SCRIPT_SOUND_AMMUNATION_CHAT_2 = 104,
	SCRIPT_SOUND_AMMUNATION_CHAT_3 = 105,
	SCRIPT_SOUND_BULLET_HIT_GROUND_1 = 106,
	SCRIPT_SOUND_BULLET_HIT_GROUND_2 = 107,
	SCRIPT_SOUND_BULLET_HIT_GROUND_3 = 108,
	SCRIPT_SOUND_BULLET_HIT_WATER = 109, //no sound
	SCRIPT_SOUND_110 = 110,
	SCRIPT_SOUND_111 = 111,
	SCRIPT_SOUND_PAYPHONE_RINGING = 112,
	SCRIPT_SOUND_113 = 113,
	SCRIPT_SOUND_GLASS_BREAK_L = 114,
	SCRIPT_SOUND_GLASS_BREAK_S = 115,
	SCRIPT_SOUND_GLASS_CRACK = 116,
	SCRIPT_SOUND_GLASS_LIGHT_BREAK = 117,
	SCRIPT_SOUND_BOX_DESTROYED_1 = 118,
	SCRIPT_SOUND_BOX_DESTROYED_2 = 119,
	SCRIPT_SOUND_METAL_COLLISION = 120,
	SCRIPT_SOUND_TIRE_COLLISION = 121,
	SCRIPT_SOUND_GUNSHELL_DROP = 122,
	SCRIPT_SOUND_GUNSHELL_DROP_SOFT = 123,
};

class tSound
{
public:
	int32 m_nEntityIndex;
	int32 m_nCounter;
	int32 m_nSampleIndex;
	uint8 m_bBankIndex;
	bool m_bIs2D;
	int32 m_nReleasingVolumeModificator;
	int32 m_nFrequency;
	uint8 m_bVolume;
	float m_fDistance;
	int32 m_nLoopCount;
	int32 m_nLoopStart;
	int32 m_nLoopEnd;
	uint8 m_bEmittingVolume;
	float m_fSpeedMultiplier;
	float m_fSoundIntensity;
	bool m_bReleasingSoundFlag;
	CVector m_vecPos;
	bool m_bReverbFlag;
	uint8 m_bLoopsRemaining;
	bool m_bRequireReflection; // Used for oneshots
	uint8 m_bOffset;
	int32 m_nReleasingVolumeDivider;
	bool m_bIsProcessed;
	bool m_bLoopEnded;
	int32 m_nCalculatedVolume;
	int8 m_nVolumeChange;
};

static_assert(sizeof(tSound) == 92, "tSound: error");

class CPhysical;
class CAutomobile;

class tAudioEntity
{
public:
	eAudioType m_nType;
	void *m_pEntity;
	bool m_bIsUsed;
	uint8 m_bStatus;
	int16 m_awAudioEvent[NUM_AUDIOENTITY_EVENTS];
	float m_afVolume[NUM_AUDIOENTITY_EVENTS];
	uint8 m_AudioEvents;
};

static_assert(sizeof(tAudioEntity) == 40, "tAudioEntity: error");

class tPedComment
{
public:
	int32 m_nSampleIndex;
	int32 m_nEntityIndex;
	CVector m_vecPos;
	float m_fDistance;
	uint8 m_bVolume;
	int8 m_nProcess;
};

static_assert(sizeof(tPedComment) == 28, "tPedComment: error");

class cPedComments
{
public:
	tPedComment m_asPedComments[NUM_PED_COMMENTS_BANKS][NUM_PED_COMMENTS_SLOTS];
	uint8 m_nIndexMap[NUM_PED_COMMENTS_BANKS][NUM_PED_COMMENTS_SLOTS];
	uint8 m_nCommentsInBank[NUM_PED_COMMENTS_BANKS];
	uint8 m_nActiveBank;

	cPedComments();
	void Add(tPedComment *com);
	void Process();
};

static_assert(sizeof(cPedComments) == 1164, "cPedComments: error");

class CEntity;

class cMissionAudio
{
public:
	CVector m_vecPos;
	bool m_bPredefinedProperties;
	int m_nSampleIndex;
	uint8 m_bLoadingStatus;
	uint8 m_bPlayStatus;
	uint8 field_22; // todo find a name
	int32 m_nMissionAudioCounter;
	bool m_bIsPlayed;
};

static_assert(sizeof(cMissionAudio) == 32, "cMissionAudio: error");

// name made up
class cAudioScriptObjectManager
{
public:
	int32 m_anScriptObjectEntityIndices[NUM_SCRIPT_MAX_ENTITIES];
	int32 m_nScriptObjectEntityTotal;

	cAudioScriptObjectManager() { m_nScriptObjectEntityTotal = 0; }
	~cAudioScriptObjectManager() { m_nScriptObjectEntityTotal = 0; }
};


class cTransmission;
class CPlane;
class CVehicle;
class CPed;

class cPedParams
{
public:
	bool m_bDistanceCalculated;
	float m_fDistance;
	CPed *m_pPed;
};

class cVehicleParams
{
public:
	bool m_bDistanceCalculated;
	float m_fDistance;
	CVehicle *m_pVehicle;
	cTransmission *m_pTransmission;
	int m_nIndex;
	float m_fVelocityChange;
};

static_assert(sizeof(cVehicleParams) == 0x18, "cVehicleParams: error");

enum {
	/*
	REFLECTION_YMAX = 0, top
	REFLECTION_YMIN = 1, bottom
	REFLECTION_XMIN = 2, left
	REFLECTION_XMAX = 3, right
	REFLECTION_ZMAX = 4,
	*/

	REFLECTION_TOP = 0,
	REFLECTION_BOTTOM,
	REFLECTION_LEFT,
	REFLECTION_RIGHT,
	REFLECTION_UP,
	MAX_REFLECTIONS,
};

class cAudioManager
{
public:
	bool m_bIsInitialised;
	uint8 field_1; // unused
	bool m_bFifthFrameFlag;
	uint8 m_bActiveSamples;
	uint8 field_4; // unused
	bool m_bDynamicAcousticModelingStatus;
	float m_fSpeedOfSound;
	bool m_bTimerJustReset;
	int32 m_nTimer;
	tSound m_sQueueSample;
	bool m_bActiveSampleQueue;
	tSound m_asSamples[NUM_SOUNDS_SAMPLES_BANKS][NUM_SOUNDS_SAMPLES_SLOTS];
	uint8 m_abSampleQueueIndexTable[NUM_SOUNDS_SAMPLES_BANKS][NUM_SOUNDS_SAMPLES_SLOTS];
	uint8 m_bSampleRequestQueuesStatus[NUM_SOUNDS_SAMPLES_BANKS];
	tSound m_asActiveSamples[NUM_SOUNDS_SAMPLES_SLOTS];
	tAudioEntity m_asAudioEntities[NUM_AUDIOENTITIES];
	int32 m_anAudioEntityIndices[NUM_AUDIOENTITIES];
	int32 m_nAudioEntitiesTotal;
	CVector m_avecReflectionsPos[NUM_AUDIO_REFLECTIONS];
	float m_afReflectionsDistances[NUM_AUDIO_REFLECTIONS];
	cAudioScriptObjectManager m_sAudioScriptObjectManager;
	cPedComments m_sPedComments;
	int32 m_nFireAudioEntity;
	int32 m_nWaterCannonEntity;
	int32 m_nPoliceChannelEntity;
	cPoliceRadioQueue m_sPoliceRadioQueue;
	int32 m_nFrontEndEntity;
	int32 m_nCollisionEntity;
	cAudioCollisionManager m_sCollisionManager;
	int32 m_nProjectileEntity;
	int32 m_nBridgeEntity;
	cMissionAudio m_sMissionAudio;
	int32 m_anRandomTable[5];
	uint8 m_bTimeSpent;
	uint8 m_bUserPause;
	uint8 m_bPreviousUserPause;
	uint32 m_FrameCounter;

	cAudioManager();
	~cAudioManager();

	// getters
	uint32 GetFrameCounter() const { return m_FrameCounter; }
	float GetReflectionsDistance(int32 idx) const { return m_afReflectionsDistances[idx]; }
	int32 GetRandomNumber(int32 idx) const { return m_anRandomTable[idx]; }
	bool IsMissionAudioPlaying() const { return m_sMissionAudio.m_bPlayStatus == 1; }

	// "Should" be in alphabetic order, except "getXTalkSfx"
	void AddDetailsToRequestedOrderList(uint8 sample);
	void AddPlayerCarSample(uint8 emittingVolume, int32 freq, uint32 sample, uint8 unk1,
	                        uint8 counter, bool notLooping);
	void AddReflectionsToRequestedQueue();
	void AddReleasingSounds();
	void AddSampleToRequestedQueue();
	void AgeCrimes();

	void CalculateDistance(bool &condition, float dist);
	bool CheckForAnAudioFileOnCD() const;
	void ClearActiveSamples();
	void ClearMissionAudio();
	void ClearRequestedQueue();
	int32 ComputeDopplerEffectedFrequency(uint32 oldFreq, float position1, float position2,
	                                      float speedMultiplier) const;
	int32 ComputePan(float, CVector *);
	uint8 ComputeVolume(uint8 emittingVolume, float soundIntensity, float distance) const;
	int32 CreateEntity(int32 type, void *entity);

	void DestroyAllGameCreatedEntities();
	void DestroyEntity(int32 id);
	void DoJumboVolOffset() const;
	void DoPoliceRadioCrackle();

	// functions returning talk sfx,
	// order from GetPedCommentSfx
	uint32 GetPlayerTalkSfx(int16 sound);
	uint32 GetCopTalkSfx(int16 sound);
	uint32 GetSwatTalkSfx(int16 sound);
	uint32 GetFBITalkSfx(int16 sound);
	uint32 GetArmyTalkSfx(int16 sound);
	uint32 GetMedicTalkSfx(int16 sound);
	uint32 GetFiremanTalkSfx(int16 sound);
	uint32 GetNormalMaleTalkSfx(int16 sound);
	uint32 GetTaxiDriverTalkSfx(int16 sound);
	uint32 GetPimpTalkSfx(int16 sound);
	uint32 GetMafiaTalkSfx(int16 sound);
	uint32 GetTriadTalkSfx(int16 sound);
	uint32 GetDiabloTalkSfx(int16 sound);
	uint32 GetYakuzaTalkSfx(int16 sound);
	uint32 GetYardieTalkSfx(int16 sound);
	uint32 GetColumbianTalkSfx(int16 sound);
	uint32 GetHoodTalkSfx(int16 sound);
	uint32 GetBlackCriminalTalkSfx(int16 sound);
	uint32 GetWhiteCriminalTalkSfx(int16 sound);
	uint32 GetMaleNo2TalkSfx(int16 sound);
	uint32 GetBlackProjectMaleTalkSfx(int16 sound, int32 model);
	uint32 GetWhiteFatMaleTalkSfx(int16 sound);
	uint32 GetBlackFatMaleTalkSfx(int16 sound);
	uint32 GetBlackCasualFemaleTalkSfx(int16 sound);
	uint32 GetWhiteCasualFemaleTalkSfx(int16 sound);
	uint32 GetFemaleNo3TalkSfx(int16 sound);
	uint32 GetBlackFatFemaleTalkSfx(int16 sound);
	uint32 GetWhiteFatFemaleTalkSfx(int16 sound);
	uint32 GetBlackFemaleProstituteTalkSfx(int16 sound);
	uint32 GetWhiteFemaleProstituteTalkSfx(int16 sound);
	uint32 GetBlackProjectFemaleOldTalkSfx(int16 sound);
	uint32 GetBlackProjectFemaleYoungTalkSfx(int16 sound);
	uint32 GetChinatownMaleOldTalkSfx(int16 sound);
	uint32 GetChinatownMaleYoungTalkSfx(int16 sound);
	uint32 GetChinatownFemaleOldTalkSfx(int16 sound);
	uint32 GetChinatownFemaleYoungTalkSfx(int16 sound);
	uint32 GetLittleItalyMaleTalkSfx(int16 sound);
	uint32 GetLittleItalyFemaleOldTalkSfx(int16 sound);
	uint32 GetLittleItalyFemaleYoungTalkSfx(int16 sound);
	uint32 GetWhiteDockerMaleTalkSfx(int16 sound);
	uint32 GetBlackDockerMaleTalkSfx(int16 sound);
	uint32 GetScumMaleTalkSfx(int16 sound);
	uint32 GetScumFemaleTalkSfx(int16 sound);
	uint32 GetWhiteWorkerMaleTalkSfx(int16 sound);
	uint32 GetBlackWorkerMaleTalkSfx(int16 sound);
	uint32 GetBusinessMaleYoungTalkSfx(int16 sound, int32 model);
	uint32 GetBusinessMaleOldTalkSfx(int16 sound);
	uint32 GetWhiteBusinessFemaleTalkSfx(int16 sound, int32 model);
	uint32 GetBlackBusinessFemaleTalkSfx(int16 sound);
	uint32 GetSupermodelMaleTalkSfx(int16 sound);
	uint32 GetSupermodelFemaleTalkSfx(int16 sound);
	uint32 GetStewardMaleTalkSfx(int16 sound);
	uint32 GetStewardFemaleTalkSfx(int16 sound);
	uint32 GetFanMaleTalkSfx(int16 sound, int32 model);
	uint32 GetFanFemaleTalkSfx(int16 sound);
	uint32 GetHospitalMaleTalkSfx(int16 sound);
	uint32 GetHospitalFemaleTalkSfx(int16 sound);
	uint32 GetWhiteConstructionWorkerTalkSfx(int16 sound);
	uint32 GetBlackConstructionWorkerTalkSfx(int16 sound);
	uint32 GetShopperFemaleTalkSfx(int16 sound, int32 model);
	uint32 GetStudentMaleTalkSfx(int16 sound);
	uint32 GetStudentFemaleTalkSfx(int16 sound);
	uint32 GetCasualMaleOldTalkSfx(int16 sound);

	uint32 GetSpecialCharacterTalkSfx(int32 modelIndex, int32 sound);
	uint32 GetEightTalkSfx(int16 sound);
	uint32 GetFrankieTalkSfx(int16 sound);
	uint32 GetMistyTalkSfx(int16 sound);
	uint32 GetOJGTalkSfx(int16 sound);
	uint32 GetCatatalinaTalkSfx(int16 sound);
	uint32 GetBomberTalkSfx(int16 sound);
	uint32 GetSecurityGuardTalkSfx(int16 sound);
	uint32 GetChunkyTalkSfx(int16 sound);

	uint32 GetGenericMaleTalkSfx(int16 sound);
	uint32 GetGenericFemaleTalkSfx(int16 sound);
	// end of functions returning talk sfx

	void GenerateIntegerRandomNumberTable();
	char *Get3DProviderName(uint8 id) const;
	uint8 GetCDAudioDriveLetter() const;
	int8 GetCurrent3DProviderIndex() const;
	float GetCollisionLoopingRatio(uint32 a, uint32 b, float c) const; // not used
	float GetCollisionOneShotRatio(int32 a, float b) const;
	float GetCollisionRatio(float a, float b, float c, float d) const;
	float GetDistanceSquared(CVector *v) const;
	int32 GetJumboTaxiFreq() const;
	bool GetMissionAudioLoadingStatus() const;
	int8 GetMissionScriptPoliceAudioPlayingStatus() const;
	uint8 GetNum3DProvidersAvailable() const;
	int32 GetPedCommentSfx(CPed *ped, int32 sound);
	void GetPhrase(uint32 *phrase, uint32 *prevPhrase, uint32 sample, uint32 maxOffset) const;
	float GetVehicleDriveWheelSkidValue(uint8 wheel, CAutomobile *automobile,
	                                    cTransmission *transmission, float velocityChange);
	float GetVehicleNonDriveWheelSkidValue(uint8 wheel, CAutomobile *automobile,
	                                       cTransmission *transmission, float velocityChange);

	bool HasAirBrakes(int32 model) const;

	void Initialise();
	void InitialisePoliceRadio();
	void InitialisePoliceRadioZones();
	void InterrogateAudioEntities();
	bool IsAudioInitialised() const;
	bool IsMissionAudioSampleFinished();
	bool IsMP3RadioChannelAvailable() const;

	bool MissionScriptAudioUsesPoliceChannel(int32 soundMission) const;

	void PlayLoadedMissionAudio();
	void PlayOneShot(int32 index, int16 sound, float vol);
	void PlaySuspectLastSeen(float x, float y, float z);
	void PlayerJustGotInCar() const;
	void PlayerJustLeftCar() const;
	void PostInitialiseGameSpecificSetup();
	void PostTerminateGameSpecificShutdown();
	void PreInitialiseGameSpecificSetup() const;
	void PreloadMissionAudio(const char *name);
	void PreTerminateGameSpecificShutdown();
	/// processX - main logic of adding new sounds
	void ProcessActiveQueues();
	bool ProcessAirBrakes(cVehicleParams *params);
	void ProcessAirportScriptObject(uint8 sound);
	bool ProcessBoatEngine(cVehicleParams *params);
	bool ProcessBoatMovingOverWater(cVehicleParams *params);
	void ProcessBridge();
	void ProcessBridgeMotor();
	void ProcessBridgeOneShots();
	void ProcessBridgeWarning();
	bool ProcessCarBombTick(cVehicleParams *params);
	void ProcessCesna(cVehicleParams *params);
	void ProcessCinemaScriptObject(uint8 sound);
	void ProcessCrane();
	void ProcessDocksScriptObject(uint8 sound);
	bool ProcessEngineDamage(cVehicleParams *params);
	void ProcessEntity(int32 sound);
	void ProcessExplosions(int32 explosion);
	void ProcessFireHydrant();
	void ProcessFires(int32 entity);
	void ProcessFrontEnd();
	void ProcessGarages();
	bool ProcessHelicopter(cVehicleParams *params);
	void ProcessHomeScriptObject(uint8 sound);
	void ProcessJumbo(cVehicleParams *);
	void ProcessJumboAccel(CPlane *plane);
	void ProcessJumboDecel(CPlane *plane);
	void ProcessJumboFlying();
	void ProcessJumboLanding(CPlane *plane);
	void ProcessJumboTakeOff(CPlane *plane);
	void ProcessJumboTaxi();
	void ProcessLaunderetteScriptObject(uint8 sound);
	void ProcessLoopingScriptObject(uint8 sound);
	void ProcessMissionAudio();
	void ProcessModelCarEngine(cVehicleParams *params);
	void ProcessOneShotScriptObject(uint8 sound);
	void ProcessPed(CPhysical *ped);
	void ProcessPedHeadphones(cPedParams *params);
	void ProcessPedOneShots(cPedParams *params);
	void ProcessPhysical(int32 id);
	void ProcessPlane(cVehicleParams *params);
	void ProcessPlayersVehicleEngine(cVehicleParams *params, CAutomobile *automobile);
	void ProcessPoliceCellBeatingScriptObject(uint8 sound);
	void ProcessPornCinema(uint8 sound);
	void ProcessProjectiles();
	void ProcessRainOnVehicle(cVehicleParams *params);
	void ProcessReverb() const;
	bool ProcessReverseGear(cVehicleParams *params);
	void ProcessSawMillScriptObject(uint8 sound);
	void ProcessScriptObject(int32 id);
	void ProcessShopScriptObject(uint8 sound);
	void ProcessSpecial();
	bool ProcessTrainNoise(cVehicleParams *params);
	void ProcessVehicle(CVehicle *vehicle);
	bool ProcessVehicleDoors(cVehicleParams *params);
	void ProcessVehicleEngine(cVehicleParams *params);
	void ProcessVehicleHorn(cVehicleParams *params);
	void ProcessVehicleOneShots(cVehicleParams *params);
	bool ProcessVehicleReverseWarning(cVehicleParams *params);
	bool ProcessVehicleRoadNoise(cVehicleParams *params);
	void ProcessVehicleSirenOrAlarm(cVehicleParams *params);
	void ProcessVehicleSkidding(cVehicleParams *params);
	void ProcessWaterCannon(int32);
	void ProcessWeather(int32 id);
	bool ProcessWetRoadNoise(cVehicleParams *params);
	void ProcessWorkShopScriptObject(uint8 sound);

	int32 RandomDisplacement(uint32 seed) const;
	void ReacquireDigitalHandle() const;
	void ReleaseDigitalHandle() const;
	void ReportCollision(CEntity *entity1, CEntity *entity2, uint8 surface1, uint8 surface2,
	                     float collisionPower, float intensity2);
	void ReportCrime(int32 crime, const CVector *pos);
	void ResetAudioLogicTimers(uint32 timer);
	void ResetPoliceRadio();
	void ResetTimers(uint32 time);

	void Service();
	void ServiceCollisions();
	void ServicePoliceRadio();
	void ServicePoliceRadioChannel(int32 wantedLevel);
	void ServiceSoundEffects();
	int8 SetCurrent3DProvider(uint8 which);
	void SetDynamicAcousticModelingStatus(bool status);
	void SetEffectsFadeVolume(uint8 volume) const;
	void SetEffectsMasterVolume(uint8 volume) const;
	void SetEntityStatus(int32 id, uint8 status);
	uint32 SetLoopingCollisionRequestedSfxFreqAndGetVol(cAudioCollision *audioCollision);
	void SetMissionAudioLocation(float x, float y, float z);
	void SetMissionScriptPoliceAudio(int32 sfx) const;
	void SetMonoMode(uint8); // todo (mobile)
	void SetMusicFadeVolume(uint8 volume) const;
	void SetMusicMasterVolume(uint8 volume) const;
	void SetSpeakerConfig(int32 conf) const;
	void SetUpLoopingCollisionSound(cAudioCollision *col, uint8 counter);
	void SetUpOneShotCollisionSound(cAudioCollision *col);
	bool SetupCrimeReport();
	bool SetupJumboEngineSound(uint8 vol, int32 freq);
	bool SetupJumboFlySound(uint8 emittingVol);
	bool SetupJumboRumbleSound(uint8 emittingVol);
	bool SetupJumboTaxiSound(uint8 vol);
	bool SetupJumboWhineSound(uint8 emittingVol, int32 freq);
	void SetupPedComments(cPedParams *params, uint32 sound);
	void SetupSuspectLastSeenReport();

	void Terminate();
	void TranslateEntity(CVector *v1, CVector *v2) const;

	void UpdateGasPedalAudio(CAutomobile *automobile);
	void UpdateReflections();
	bool UsesReverseWarning(int32 model) const;
	bool UsesSiren(int32 model) const;
	bool UsesSirenSwitching(int32 model) const;

	// only used in pc
	void AdjustSamplesVolume();
	uint8 ComputeEmittingVolume(uint8 emittingVolume, float intensity, float dist);
};

//dstatic_assert(sizeof(cAudioManager) == 19220, "cAudioManager: error");

extern cAudioManager AudioManager;
