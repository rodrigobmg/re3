#pragma once
#include "common.h"
#include "AudioSamples.h"

#define MAX_VOLUME 127
#define MAX_FREQ DIGITALRATE

struct tSample {
	int32 nOffset;
	uint32 nSize;
	int32 nFrequency;
	int32 nLoopStart;
	int32 nLoopEnd;
};

enum
{
	SAMPLEBANK_MAIN,
	SAMPLEBANK_PED,
	MAX_SAMPLEBANKS,
	SAMPLEBANK_INVALID
};

#define MAX_PEDSFX                 7
#define PED_BLOCKSIZE              79000

#define MAXPROVIDERS               64

#define MAXCHANNELS                28
#define MAXCHANNELS_SURROUND       24
#define MAX2DCHANNELS              1
#define CHANNEL2D                  MAXCHANNELS

#define MAX_STREAMS                2

#define DIGITALRATE                32000
#define DIGITALBITS                16
#define DIGITALCHANNELS            2

#define MAX_DIGITAL_MIXER_CHANNELS 32

class cSampleManager
{
	uint8   m_nEffectsVolume;
	uint8   m_nMusicVolume;
	uint8   m_nEffectsFadeVolume;
	uint8   m_nMusicFadeVolume;
	uint8   m_nMonoMode;
	char	unk;
	char    m_szCDRomRootPath[80];
	bool    m_bInitialised;
	uint8   m_nNumberOfProviders;
	char   *m_aAudioProviders[MAXPROVIDERS];
	tSample m_aSamples[TOTAL_AUDIO_SAMPLES];

public:
	
	

	cSampleManager(void);
	~cSampleManager(void);
	
	void SetSpeakerConfig(int32 nConfig);
	uint32 GetMaximumSupportedChannels(void);
	
	uint32 GetNum3DProvidersAvailable(void);
	void SetNum3DProvidersAvailable(uint32 num);
	
	char *Get3DProviderName(uint8 id);
	void Set3DProviderName(uint8 id, char *name);
	
	int8 GetCurrent3DProviderIndex(void);
	int8 SetCurrent3DProvider(uint8 which);
	
	bool IsMP3RadioChannelAvailable(void);
	
	void ReleaseDigitalHandle  (void);
	void ReacquireDigitalHandle(void);
	
	bool Initialise(void);
	void Terminate (void);
	
#ifdef AUDIO_OAL
	void UpdateSoundBuffers(void);
#endif
	
	bool CheckForAnAudioFileOnCD(void);
	char GetCDAudioDriveLetter  (void);
	
	void UpdateEffectsVolume(void);

	void SetEffectsMasterVolume(uint8 nVolume);
	void SetMusicMasterVolume  (uint8 nVolume);
	void SetEffectsFadeVolume  (uint8 nVolume);
	void SetMusicFadeVolume    (uint8 nVolume);
	void SetMonoMode           (uint8 nMode);
	
	bool LoadSampleBank    (uint8 nBank);
	void UnloadSampleBank  (uint8 nBank);
	bool IsSampleBankLoaded(uint8 nBank);
	
	bool IsPedCommentLoaded(uint32 nComment);
	bool LoadPedComment    (uint32 nComment);
	int32 GetBankContainingSound(uint32 offset);

	int32 _GetPedCommentSlot(uint32 nComment);
	
	int32  GetSampleBaseFrequency  (uint32 nSample);
	int32  GetSampleLoopStartOffset(uint32 nSample);
	int32  GetSampleLoopEndOffset  (uint32 nSample);
	uint32 GetSampleLength         (uint32 nSample);
	
	bool  UpdateReverb(void);
	
	void  SetChannelReverbFlag    (uint32 nChannel, uint8 nReverbFlag);
	bool  InitialiseChannel       (uint32 nChannel, uint32 nSfx, uint8 nBank);
	void  SetChannelEmittingVolume(uint32 nChannel, uint32 nVolume);
	void  SetChannel3DPosition    (uint32 nChannel, float fX, float fY, float fZ);
	void  SetChannel3DDistances   (uint32 nChannel, float fMax, float fMin);
	void  SetChannelVolume        (uint32 nChannel, uint32 nVolume);
	void  SetChannelPan           (uint32 nChannel, uint32 nPan);
	void  SetChannelFrequency     (uint32 nChannel, uint32 nFreq);
	void  SetChannelLoopPoints    (uint32 nChannel, uint32 nLoopStart, int32 nLoopEnd);
	void  SetChannelLoopCount     (uint32 nChannel, uint32 nLoopCount);
	bool  GetChannelUsedFlag      (uint32 nChannel);
	void  StartChannel            (uint32 nChannel);
	void  StopChannel             (uint32 nChannel);
	
	void  PreloadStreamedFile                                     (uint8 nFile, uint8 nStream);
	void  PauseStream                                        (uint8 nPauseFlag, uint8 nStream);
	void  StartPreloadedStreamedFile                                           (uint8 nStream);
	bool  StartStreamedFile                          (uint8 nFile, uint32 nPos, uint8 nStream);
	void  StopStreamedFile                                                     (uint8 nStream);
	int32 GetStreamedFilePosition                                              (uint8 nStream);
	void  SetStreamedVolumeAndPan(uint8 nVolume, uint8 nPan, uint8 nEffectFlag, uint8 nStream);
	int32 GetStreamedFileLength                                                (uint8 nStream);
	bool  IsStreamPlaying                                                      (uint8 nStream);
#ifdef AUDIO_OAL
	void  Service(void);
#endif
	bool  InitialiseSampleBanks(void);
};

extern cSampleManager SampleManager;
extern uint32 BankStartOffset[MAX_SAMPLEBANKS];

static char StreamedNameTable[][25]=
{
	"AUDIO\\WILD.ADF",
	"AUDIO\\FLASH.ADF",
	"AUDIO\\KCHAT.ADF",
	"AUDIO\\FEVER.ADF",
	"AUDIO\\VROCK.ADF",
	"AUDIO\\VCPR.ADF",
	"AUDIO\\ESPANT.ADF",
	"AUDIO\\EMOTION.ADF",
	"AUDIO\\WAVE.ADF",
	"AUDIO\\MISCOM.MP3",
	"AUDIO\\CITY.MP3",
	"AUDIO\\WATER.MP3",
	"AUDIO\\BEACHAMB.MP3",
	"AUDIO\\HCITY.MP3",
	"AUDIO\\HWATER.MP3",
	"AUDIO\\HBEACH.MP3",
	"AUDIO\\MALLAMB.MP3",
	"AUDIO\\STRIP.MP3",
	"AUDIO\\MALIBU.MP3",
	"AUDIO\\HOTEL.MP3",
	"AUDIO\\DIRTRING.MP3",
	"AUDIO\\LAW4RIOT.MP3",
	"AUDIO\\AMBSIL.MP3",
	"AUDIO\\POLICE.MP3",
	"AUDIO\\TAXI.MP3",
	"AUDIO\\BCLOSED.MP3",
	"AUDIO\\BOPEN.MP3",
	"AUDIO\\ASS_1.MP3",
	"AUDIO\\ASS_2.MP3",
	"AUDIO\\BANK_1.MP3",
	"AUDIO\\BANK_2A.MP3",
	"AUDIO\\BANK_2B.MP3",
	"AUDIO\\BANK_3A.MP3",
	"AUDIO\\BANK_3B.MP3",
	"AUDIO\\BANK_4.MP3",
	"AUDIO\\BIKE_1.MP3",
	"AUDIO\\BIKE_2.MP3",
	"AUDIO\\BIKE_3.MP3",
	"AUDIO\\BUD_1.MP3",
	"AUDIO\\BUD_2.MP3",
	"AUDIO\\BUD_3.MP3",
	"AUDIO\\CAP_1.MP3",
	"AUDIO\\CAR_1.MP3",
	"AUDIO\\CNT_1A.MP3",
	"AUDIO\\CNT_1B.MP3",
	"AUDIO\\CNT_2.MP3",
	"AUDIO\\COK_1.MP3",
	"AUDIO\\COK_2A.MP3",
	"AUDIO\\COK_2B.MP3",
	"AUDIO\\COK_3.MP3",
	"AUDIO\\COK_4A.MP3",
	"AUDIO\\COK_4A2.MP3",
	"AUDIO\\COK_4B.MP3",
	"AUDIO\\COL_1.MP3",
	"AUDIO\\COL_2.MP3",
	"AUDIO\\COL_3A.MP3",
	"AUDIO\\COL_4A.MP3",
	"AUDIO\\COL_5A.MP3",
	"AUDIO\\COL_5B.MP3",
	"AUDIO\\CUB_1.MP3",
	"AUDIO\\CUB_2.MP3",
	"AUDIO\\CUB_3.MP3",
	"AUDIO\\CUB_4.MP3",
	"AUDIO\\DRUG_1.MP3",
	"AUDIO\\FIN.MP3",
	"AUDIO\\FIN2.MP3",
	"AUDIO\\FINALE.MP3",
	"AUDIO\\HAT_1.MP3",
	"AUDIO\\HAT_2.MP3",
	"AUDIO\\HAT_3.MP3",
	"AUDIO\\ICE_1.MP3",
	"AUDIO\\INT_A.MP3",
	"AUDIO\\INT_B.MP3",
	"AUDIO\\INT_D.MP3",
	"AUDIO\\INT_M.MP3",
	"AUDIO\\LAW_1A.MP3",
	"AUDIO\\LAW_1B.MP3",
	"AUDIO\\LAW_2A.MP3",
	"AUDIO\\LAW_2B.MP3",
	"AUDIO\\LAW_2C.MP3",
	"AUDIO\\LAW_3.MP3",
	"AUDIO\\LAW_4.MP3",
	"AUDIO\\PHIL_1.MP3",
	"AUDIO\\PHIL_2.MP3",
	"AUDIO\\PORN_1.MP3",
	"AUDIO\\PORN_2.MP3",
	"AUDIO\\PORN_3.MP3",
	"AUDIO\\PORN_4.MP3",
	"AUDIO\\RESC_1A.MP3",
	"AUDIO\\ROK_1.MP3",
	"AUDIO\\ROK_2.MP3",
	"AUDIO\\ROK_3A.MP3",
	"AUDIO\\STRIPA.MP3",
	"AUDIO\\TAX_1.MP3",
	"AUDIO\\TEX_1.MP3",
	"AUDIO\\TEX_2.MP3",
	"AUDIO\\TEX_3.MP3",
	"AUDIO\\GLIGHT.MP3",
	"AUDIO\\FIST.MP3",
	"AUDIO\\MISCOM.MP3",
	"AUDIO\\MISCOM.MP3",
	"AUDIO\\MISCOM.MP3",
	"AUDIO\\MISCOM.MP3",
	"AUDIO\\MOBR1.WAV",
	"AUDIO\\PAGER.WAV",
	"AUDIO\\CARREV.WAV",
	"AUDIO\\BIKEREV.WAV",
	"AUDIO\\LIFTOP.WAV",
	"AUDIO\\LIFTCL.WAV",
	"AUDIO\\LIFTRUN.WAV",
	"AUDIO\\LIFTBEL.WAV",
	"AUDIO\\INLIFT.WAV",
	"AUDIO\\SFX_01.WAV",
	"AUDIO\\SFX_02.WAV",
	"AUDIO\\CAMERAL.WAV",
	"AUDIO\\CAMERAR.WAV",
	"AUDIO\\CHEER1.WAV",
	"AUDIO\\CHEER2.WAV",
	"AUDIO\\CHEER3.WAV",
	"AUDIO\\CHEER4.WAV",
	"AUDIO\\OOH1.WAV",
	"AUDIO\\OOH2.WAV",
	"AUDIO\\RACE1.WAV",
	"AUDIO\\RACE2.WAV",
	"AUDIO\\RACE3.WAV",
	"AUDIO\\RACE4.WAV",
	"AUDIO\\RACE5.WAV",
	"AUDIO\\RACE6.WAV",
	"AUDIO\\RACE7.WAV",
	"AUDIO\\RACE8.WAV",
	"AUDIO\\RACE9.WAV",
	"AUDIO\\RACE10.WAV",
	"AUDIO\\RACE11.WAV",
	"AUDIO\\RACE12.WAV",
	"AUDIO\\RACE13.WAV",
	"AUDIO\\RACE14.WAV",
	"AUDIO\\RACE15.WAV",
	"AUDIO\\HOT1.WAV",
	"AUDIO\\HOT2.WAV",
	"AUDIO\\HOT3.WAV",
	"AUDIO\\HOT4.WAV",
	"AUDIO\\HOT5.WAV",
	"AUDIO\\HOT6.WAV",
	"AUDIO\\HOT7.WAV",
	"AUDIO\\HOT8.WAV",
	"AUDIO\\HOT9.WAV",
	"AUDIO\\HOT10.WAV",
	"AUDIO\\HOT11.WAV",
	"AUDIO\\HOT12.WAV",
	"AUDIO\\HOT13.WAV",
	"AUDIO\\HOT14.WAV",
	"AUDIO\\HOT15.WAV",
	"AUDIO\\LANSTP1.WAV",
	"AUDIO\\LANSTP2.WAV",
	"AUDIO\\LANAMU1.WAV",
	"AUDIO\\LANAMU2.WAV",
	"AUDIO\\AIRHORNL.WAV",
	"AUDIO\\AIRHORNR.WAV",
	"AUDIO\\SNIPSCRL.WAV",
	"AUDIO\\SNIPSHORT.WAV",
	"AUDIO\\BLOWROOF.WAV",
	"AUDIO\\ASS_1.WAV",
	"AUDIO\\ASS_2.WAV",
	"AUDIO\\ASS_3.WAV",
	"AUDIO\\ASS_4.WAV",
	"AUDIO\\ASS_5.WAV",
	"AUDIO\\ASS_6.WAV",
	"AUDIO\\ASS_7.WAV",
	"AUDIO\\ASS_8.WAV",
	"AUDIO\\ASS_9.WAV",
	"AUDIO\\ASS_10.WAV",
	"AUDIO\\ASS_11.WAV",
	"AUDIO\\ASS_12.WAV",
	"AUDIO\\ASS_13.WAV",
	"AUDIO\\ASS_14.WAV",
	"AUDIO\\BIKE1_1.WAV",
	"AUDIO\\BIKE1_2.WAV",
	"AUDIO\\BIKE1_3.WAV",
	"AUDIO\\BNK1_1.WAV",
	"AUDIO\\BNK1_2.WAV",
	"AUDIO\\BNK1_3.WAV",
	"AUDIO\\BNK1_4.WAV",
	"AUDIO\\BNK1_5.WAV",
	"AUDIO\\BNK1_6.WAV",
	"AUDIO\\BNK1_7.WAV",
	"AUDIO\\BNK1_8.WAV",
	"AUDIO\\BNK1_10.WAV",
	"AUDIO\\BNK1_11.WAV",
	"AUDIO\\BNK1_12.WAV",
	"AUDIO\\BNK1_13.WAV",
	"AUDIO\\BNK1_14.WAV",
	"AUDIO\\BNK2_1.WAV",
	"AUDIO\\BNK2_2.WAV",
	"AUDIO\\BNK2_3.WAV",
	"AUDIO\\BNK2_4.WAV",
	"AUDIO\\BNK2_5.WAV",
	"AUDIO\\BNK2_6.WAV",
	"AUDIO\\BNK2_7.WAV",
	"AUDIO\\BNK2_8.WAV",
	"AUDIO\\BNK2_9.WAV",
	"AUDIO\\BNK3_1.WAV",
	"AUDIO\\BNK3_2.WAV",
	"AUDIO\\BNK3_3A.WAV",
	"AUDIO\\BNK3_3B.WAV",
	"AUDIO\\BNK3_3C.WAV",
	"AUDIO\\BNK3_4A.WAV",
	"AUDIO\\BNK3_4B.WAV",
	"AUDIO\\BNK3_4C.WAV",
	"AUDIO\\BNK4_1.WAV",
	"AUDIO\\BNK4_2.WAV",
	"AUDIO\\BNK4_3A.WAV",
	"AUDIO\\BNK4_3B.WAV",
	"AUDIO\\BNK4_3C.WAV",
	"AUDIO\\BNK4_3D.WAV",
	"AUDIO\\BNK4_3E.WAV",
	"AUDIO\\BNK4_3F.WAV",
	"AUDIO\\BNK4_3G.WAV",
	"AUDIO\\BNK4_3H.WAV",
	"AUDIO\\BNK4_3I.WAV",
	"AUDIO\\BNK4_3J.WAV",
	"AUDIO\\BNK4_3K.WAV",
	"AUDIO\\BNK4_3M.WAV",
	"AUDIO\\BNK4_3O.WAV",
	"AUDIO\\BNK4_3P.WAV",
	"AUDIO\\BNK4_3Q.WAV",
	"AUDIO\\BNK4_3R.WAV",
	"AUDIO\\BNK4_3S.WAV",
	"AUDIO\\BNK4_3T.WAV",
	"AUDIO\\BNK4_3U.WAV",
	"AUDIO\\BNK4_3V.WAV",
	"AUDIO\\BNK4_4A.WAV",
	"AUDIO\\BNK4_4B.WAV",
	"AUDIO\\BNK4_5.WAV",
	"AUDIO\\BNK4_6.WAV",
	"AUDIO\\BNK4_7.WAV",
	"AUDIO\\BNK4_8.WAV",
	"AUDIO\\BNK4_9.WAV",
	"AUDIO\\BNK4_10.WAV",
	"AUDIO\\BNK4_11.WAV",
	"AUDIO\\BK4_12A.WAV",
	"AUDIO\\BK4_12B.WAV",
	"AUDIO\\BK4_12C.WAV",
	"AUDIO\\BNK4_13.WAV",
	"AUDIO\\BK4_14A.WAV",
	"AUDIO\\BK4_14B.WAV",
	"AUDIO\\BNK4_15.WAV",
	"AUDIO\\BNK4_16.WAV",
	"AUDIO\\BNK4_17.WAV",
	"AUDIO\\BNK4_18.WAV",
	"AUDIO\\BK4_19A.WAV",
	"AUDIO\\BK4_19B.WAV",
	"AUDIO\\BK4_20A.WAV",
	"AUDIO\\BK4_20B.WAV",
	"AUDIO\\BNK4_21.WAV",
	"AUDIO\\BNK422A.WAV",
	"AUDIO\\BNK422B.WAV",
	"AUDIO\\BK4_23A.WAV",
	"AUDIO\\BK4_23B.WAV",
	"AUDIO\\BK4_23C.WAV",
	"AUDIO\\BK4_23D.WAV",
	"AUDIO\\BK4_24A.WAV",
	"AUDIO\\BK4_24B.WAV",
	"AUDIO\\BNK4_25.WAV",
	"AUDIO\\BNK4_26.WAV",
	"AUDIO\\BNK4_27.WAV",
	"AUDIO\\BNK4_28.WAV",
	"AUDIO\\BNK4_29.WAV",
	"AUDIO\\BNK4_30.WAV",
	"AUDIO\\BK4_31A.WAV",
	"AUDIO\\BK4_31B.WAV",
	"AUDIO\\BNK4_32.WAV",
	"AUDIO\\BK4_34A.WAV",
	"AUDIO\\BK4_34B.WAV",
	"AUDIO\\BK4_35A.WAV",
	"AUDIO\\BK4_35B.WAV",
	"AUDIO\\BNK4_36.WAV",
	"AUDIO\\BNK4_37.WAV",
	"AUDIO\\BNK4_38.WAV",
	"AUDIO\\BNK4_39.WAV",
	"AUDIO\\BK4_40A.WAV",
	"AUDIO\\BK4_40B.WAV",
	"AUDIO\\BNK4_41.WAV",
	"AUDIO\\BNK4_42.WAV",
	"AUDIO\\BNK4_43.WAV",
	"AUDIO\\BNK4_44.WAV",
	"AUDIO\\BNK4_45.WAV",
	"AUDIO\\BNK4_46.WAV",
	"AUDIO\\BNK4_47.WAV",
	"AUDIO\\BNK4_48.WAV",
	"AUDIO\\BNK4_49.WAV",
	"AUDIO\\BNK450A.WAV",
	"AUDIO\\BNK450B.WAV",
	"AUDIO\\BNK4_51.WAV",
	"AUDIO\\BNK4_94.WAV",
	"AUDIO\\BNK4_95.WAV",
	"AUDIO\\BNK4_96.WAV",
	"AUDIO\\BNK4_97.WAV",
	"AUDIO\\BNK4_98.WAV",
	"AUDIO\\BNK4_99.WAV",
	"AUDIO\\BUD1_1.WAV",
	"AUDIO\\BUD1_2.WAV",
	"AUDIO\\BUD1_3.WAV",
	"AUDIO\\BUD1_4.WAV",
	"AUDIO\\BUD1_5.WAV",
	"AUDIO\\BUD1_9.WAV",
	"AUDIO\\BUD1_10.WAV",
	"AUDIO\\BUD2_1.WAV",
	"AUDIO\\BUD2_2.WAV",
	"AUDIO\\BUD2_3.WAV",
	"AUDIO\\BUD2_4.WAV",
	"AUDIO\\BUD2_5.WAV",
	"AUDIO\\BUD2_6.WAV",
	"AUDIO\\BUD2_7.WAV",
	"AUDIO\\BUD3_1.WAV",
	"AUDIO\\BUD3_1A.WAV",
	"AUDIO\\BUD3_1B.WAV",
	"AUDIO\\BUD3_1C.WAV",
	"AUDIO\\BUD3_2.WAV",
	"AUDIO\\BUD3_3.WAV",
	"AUDIO\\BUD3_4.WAV",
	"AUDIO\\BUD3_5.WAV",
	"AUDIO\\BUD3_6.WAV",
	"AUDIO\\BUD3_7.WAV",
	"AUDIO\\BUD3_8A.WAV",
	"AUDIO\\BUD3_8B.WAV",
	"AUDIO\\BUD3_8C.WAV",
	"AUDIO\\BUD3_9A.WAV",
	"AUDIO\\BUD3_9B.WAV",
	"AUDIO\\BUD3_9C.WAV",
	"AUDIO\\CAP1_2.WAV",
	"AUDIO\\CAP1_3.WAV",
	"AUDIO\\CAP1_4.WAV",
	"AUDIO\\CAP1_5.WAV",
	"AUDIO\\CAP1_6.WAV",
	"AUDIO\\CAP1_7.WAV",
	"AUDIO\\CAP1_8.WAV",
	"AUDIO\\CAP1_9.WAV",
	"AUDIO\\CAP1_10.WAV",
	"AUDIO\\CAP1_11.WAV",
	"AUDIO\\CAP1_12.WAV",
	"AUDIO\\CNT1_1.WAV",
	"AUDIO\\CNT1_2.WAV",
	"AUDIO\\CNT1_3.WAV",
	"AUDIO\\CNT1_4.WAV",
	"AUDIO\\CNT1_5.WAV",
	"AUDIO\\CNT2_1.WAV",
	"AUDIO\\CNT2_2.WAV",
	"AUDIO\\CNT2_3.WAV",
	"AUDIO\\CNT2_4.WAV",
	"AUDIO\\COK1_1.WAV",
	"AUDIO\\COK1_2.WAV",
	"AUDIO\\COK1_3.WAV",
	"AUDIO\\COK1_4.WAV",
	"AUDIO\\COK1_5.WAV",
	"AUDIO\\COK1_6.WAV",
	"AUDIO\\COK2_1.WAV",
	"AUDIO\\COK2_2.WAV",
	"AUDIO\\COK2_3.WAV",
	"AUDIO\\COK2_4.WAV",
	"AUDIO\\COK2_5.WAV",
	"AUDIO\\COK2_6.WAV",
	"AUDIO\\COK2_7A.WAV",
	"AUDIO\\COK2_7B.WAV",
	"AUDIO\\COK2_7C.WAV",
	"AUDIO\\COK2_8A.WAV",
	"AUDIO\\COK2_8B.WAV",
	"AUDIO\\COK2_8C.WAV",
	"AUDIO\\COK2_8D.WAV",
	"AUDIO\\COK2_9.WAV",
	"AUDIO\\COK210A.WAV",
	"AUDIO\\COK210B.WAV",
	"AUDIO\\COK210C.WAV",
	"AUDIO\\COK212A.WAV",
	"AUDIO\\COK212B.WAV",
	"AUDIO\\COK2_13.WAV",
	"AUDIO\\COK2_14.WAV",
	"AUDIO\\COK2_15.WAV",
	"AUDIO\\COK2_16.WAV",
	"AUDIO\\COK2_20.WAV",
	"AUDIO\\COK2_21.WAV",
	"AUDIO\\COK2_2.WAV", // this is probably a typo of COK2_22
	"AUDIO\\COK3_1.WAV",
	"AUDIO\\COK3_2.WAV",
	"AUDIO\\COK3_3.WAV",
	"AUDIO\\COK3_4.WAV",
	"AUDIO\\COK4_1.WAV",
	"AUDIO\\COK4_2.WAV",
	"AUDIO\\COK4_3.WAV",
	"AUDIO\\COK4_4.WAV",
	"AUDIO\\COK4_5.WAV",
	"AUDIO\\COK4_6.WAV",
	"AUDIO\\COK4_7.WAV",
	"AUDIO\\COK4_8.WAV",
	"AUDIO\\COK4_9.WAV",
	"AUDIO\\COK4_9A.WAV",
	"AUDIO\\COK4_10.WAV",
	"AUDIO\\COK4_11.WAV",
	"AUDIO\\COK4_12.WAV",
	"AUDIO\\COK4_13.WAV",
	"AUDIO\\COK4_14.WAV",
	"AUDIO\\COK4_15.WAV",
	"AUDIO\\COK4_16.WAV",
	"AUDIO\\COK4_17.WAV",
	"AUDIO\\COK4_18.WAV",
	"AUDIO\\COK4_19.WAV",
	"AUDIO\\COK4_20.WAV",
	"AUDIO\\COK4_21.WAV",
	"AUDIO\\COK4_22.WAV",
	"AUDIO\\COK4_23.WAV",
	"AUDIO\\COK4_24.WAV",
	"AUDIO\\COK4_25.WAV",
	"AUDIO\\COK4_26.WAV",
	"AUDIO\\COK4_27.WAV",
	"AUDIO\\COL1_1.WAV",
	"AUDIO\\COL1_2.WAV",
	"AUDIO\\COL1_3.WAV",
	"AUDIO\\COL1_4.WAV",
	"AUDIO\\COL1_5.WAV",
	"AUDIO\\COL1_6.WAV",
	"AUDIO\\COL1_7.WAV",
	"AUDIO\\COL1_8.WAV",
	"AUDIO\\COL2_1.WAV",
	"AUDIO\\COL2_2.WAV",
	"AUDIO\\COL2_3.WAV",
	"AUDIO\\COL2_4.WAV",
	"AUDIO\\COL2_5.WAV",
	"AUDIO\\COL2_6A.WAV",
	"AUDIO\\COL2_7.WAV",
	"AUDIO\\COL2_8.WAV",
	"AUDIO\\COL2_9.WAV",
	"AUDIO\\COL2_10.WAV",
	"AUDIO\\COL2_11.WAV",
	"AUDIO\\COL2_12.WAV",
	"AUDIO\\COL2_13.WAV",
	"AUDIO\\COL2_14.WAV",
	"AUDIO\\COL2_15.WAV",
	"AUDIO\\COL2_16.WAV",
	"AUDIO\\COL3_1.WAV",
	"AUDIO\\COL3_2.WAV",
	"AUDIO\\COL3_2A.WAV",
	"AUDIO\\COL3_2B.WAV",
	"AUDIO\\COL3_3.WAV",
	"AUDIO\\COL3_4.WAV",
	"AUDIO\\COL3_5.WAV",
	"AUDIO\\COL3_6.WAV",
	"AUDIO\\COL3_7.WAV",
	"AUDIO\\COL3_8.WAV",
	"AUDIO\\COL3_9.WAV",
	"AUDIO\\COL3_10.WAV",
	"AUDIO\\COL3_11.WAV",
	"AUDIO\\COL3_12.WAV",
	"AUDIO\\COL3_13.WAV",
	"AUDIO\\COL3_14.WAV",
	"AUDIO\\COL3_15.WAV",
	"AUDIO\\COL3_16.WAV",
	"AUDIO\\COL3_17.WAV",
	"AUDIO\\COL3_18.WAV",
	"AUDIO\\COL3_19.WAV",
	"AUDIO\\COL3_20.WAV",
	"AUDIO\\COL3_21.WAV",
	"AUDIO\\COL3_23.WAV",
	"AUDIO\\COL3_24.WAV",
	"AUDIO\\COL3_25.WAV",
	"AUDIO\\COL4_1.WAV",
	"AUDIO\\COL4_2.WAV",
	"AUDIO\\COL4_3.WAV",
	"AUDIO\\COL4_4.WAV",
	"AUDIO\\COL4_5.WAV",
	"AUDIO\\COL4_6.WAV",
	"AUDIO\\COL4_7.WAV",
	"AUDIO\\COL4_8.WAV",
	"AUDIO\\COL4_9.WAV",
	"AUDIO\\COL4_10.WAV",
	"AUDIO\\COL4_11.WAV",
	"AUDIO\\COL4_12.WAV",
	"AUDIO\\COL4_13.WAV",
	"AUDIO\\COL4_14.WAV",
	"AUDIO\\COL4_15.WAV",
	"AUDIO\\COL4_16.WAV",
	"AUDIO\\COL4_17.WAV",
	"AUDIO\\COL4_18.WAV",
	"AUDIO\\COL4_19.WAV",
	"AUDIO\\COL4_20.WAV",
	"AUDIO\\COL4_21.WAV",
	"AUDIO\\COL4_22.WAV",
	"AUDIO\\COL4_23.WAV",
	"AUDIO\\COL4_24.WAV",
	"AUDIO\\COL4_25.WAV",
	"AUDIO\\COL4_26.WAV",
	"AUDIO\\COL5_1.WAV",
	"AUDIO\\COL5_2.WAV",
	"AUDIO\\COL5_3.WAV",
	"AUDIO\\COL5_4.WAV",
	"AUDIO\\COL5_5.WAV",
	"AUDIO\\COL5_6.WAV",
	"AUDIO\\COL5_7.WAV",
	"AUDIO\\COL5_8.WAV",
	"AUDIO\\COL5_9.WAV",
	"AUDIO\\COL5_10.WAV",
	"AUDIO\\COL5_11.WAV",
	"AUDIO\\COL5_12.WAV",
	"AUDIO\\COL5_13.WAV",
	"AUDIO\\COL5_14.WAV",
	"AUDIO\\COL5_15.WAV",
	"AUDIO\\COL5_16.WAV",
	"AUDIO\\COL5_17.WAV",
	"AUDIO\\COL5_18.WAV",
	"AUDIO\\COL5_19.WAV",
	"AUDIO\\COL5_20.WAV",
	"AUDIO\\COL5_21.WAV",
	"AUDIO\\COL5_22.WAV",
	"AUDIO\\CUB1_1.WAV",
	"AUDIO\\CUB1_2.WAV",
	"AUDIO\\CUB1_3.WAV",
	"AUDIO\\CUB1_4.WAV",
	"AUDIO\\CUB1_5.WAV",
	"AUDIO\\CUB1_6.WAV",
	"AUDIO\\CUB1_7.WAV",
	"AUDIO\\CUB1_8.WAV",
	"AUDIO\\CUB1_9.WAV",
	"AUDIO\\CUB1_10.WAV",
	"AUDIO\\CUB2_1.WAV",
	"AUDIO\\CUB2_2.WAV",
	"AUDIO\\CUB2_3A.WAV",
	"AUDIO\\CUB2_3B.WAV",
	"AUDIO\\CUB2_3C.WAV",
	"AUDIO\\CUB2_4A.WAV",
	"AUDIO\\CUB2_5.WAV",
	"AUDIO\\CUB2_6.WAV",
	"AUDIO\\CUB2_7.WAV",
	"AUDIO\\CUB2_8.WAV",
	"AUDIO\\CUB2_9.WAV",
	"AUDIO\\CUB2_10.WAV",
	"AUDIO\\CUB2_11.WAV",
	"AUDIO\\CUB3_1.WAV",
	"AUDIO\\CUB3_2.WAV",
	"AUDIO\\CUB3_3.WAV",
	"AUDIO\\CUB3_4.WAV",
	"AUDIO\\CUB4_1.WAV",
	"AUDIO\\CUB4_2.WAV",
	"AUDIO\\CUB4_3.WAV",
	"AUDIO\\CUB4_4.WAV",
	"AUDIO\\CUB4_5.WAV",
	"AUDIO\\CUB4_5A.WAV",
	"AUDIO\\CUB4_6.WAV",
	"AUDIO\\CUB4_7.WAV",
	"AUDIO\\CUB4_8.WAV",
	"AUDIO\\CUB4_9.WAV",
	"AUDIO\\CUB4_10.WAV",
	"AUDIO\\CUB4_11.WAV",
	"AUDIO\\CUB4_12.WAV",
	"AUDIO\\CUB4_13.WAV",
	"AUDIO\\CUB4_14.WAV",
	"AUDIO\\CUB4_15.WAV",
	"AUDIO\\CUB4_16.WAV",
	"AUDIO\\GOLF_1.WAV",
	"AUDIO\\GOLF_2.WAV",
	"AUDIO\\GOLF_3.WAV",
	"AUDIO\\BAR_1.WAV",
	"AUDIO\\BAR_2.WAV",
	"AUDIO\\BAR_3.WAV",
	"AUDIO\\BAR_4.WAV",
	"AUDIO\\BAR_5.WAV",
	"AUDIO\\BAR_6.WAV",
	"AUDIO\\BAR_7.WAV",
	"AUDIO\\BAR_8.WAV",
	"AUDIO\\STRIP_1.WAV",
	"AUDIO\\STRIP_2.WAV",
	"AUDIO\\STRIP_3.WAV",
	"AUDIO\\STRIP_4.WAV",
	"AUDIO\\STRIP_5.WAV",
	"AUDIO\\STRIP_6.WAV",
	"AUDIO\\STRIP_7.WAV",
	"AUDIO\\STRIP_8.WAV",
	"AUDIO\\STRIP_9.WAV",
	"AUDIO\\STAR_1.WAV",
	"AUDIO\\STAR_2.WAV",
	"AUDIO\\STAR_3.WAV",
	"AUDIO\\STAR_4.WAV",
	"AUDIO\\FIN_1A.WAV",
	"AUDIO\\FIN_1B.WAV",
	"AUDIO\\FIN_1C.WAV",
	"AUDIO\\FIN_2B.WAV",
	"AUDIO\\FIN_2C.WAV",
	"AUDIO\\FIN_3.WAV",
	"AUDIO\\FIN_4.WAV",
	"AUDIO\\FIN_5.WAV",
	"AUDIO\\FIN_6.WAV",
	"AUDIO\\FIN_10.WAV",
	"AUDIO\\FIN_11A.WAV",
	"AUDIO\\FIN_11B.WAV",
	"AUDIO\\FIN_12A.WAV",
	"AUDIO\\FIN_12B.WAV",
	"AUDIO\\FIN_12C.WAV",
	"AUDIO\\FIN_13.WAV",
	"AUDIO\\FINKILL.WAV",
	"AUDIO\\LAW1_1.WAV",
	"AUDIO\\LAW1_2.WAV",
	"AUDIO\\LAW1_3.WAV",
	"AUDIO\\LAW1_4.WAV",
	"AUDIO\\LAW1_5.WAV",
	"AUDIO\\LAW1_6.WAV",
	"AUDIO\\LAW1_7.WAV",
	"AUDIO\\LAW1_8.WAV",
	"AUDIO\\LAW1_9.WAV",
	"AUDIO\\LAW1_10.WAV",
	"AUDIO\\LAW2_1.WAV",
	"AUDIO\\LAW2_2.WAV",
	"AUDIO\\LAW2_3.WAV",
	"AUDIO\\LAW2_4.WAV",
	"AUDIO\\LAW2_5.WAV",
	"AUDIO\\LAW2_6.WAV",
	"AUDIO\\LAW2_7.WAV",
	"AUDIO\\LAW2_8.WAV",
	"AUDIO\\LAW2_9.WAV",
	"AUDIO\\LAW2_10.WAV",
	"AUDIO\\LAW3_1.WAV",
	"AUDIO\\LAW3_2.WAV",
	"AUDIO\\LAW3_3.WAV",
	"AUDIO\\LAW3_4.WAV",
	"AUDIO\\LAW3_5.WAV",
	"AUDIO\\LAW3_6.WAV",
	"AUDIO\\LAW3_10.WAV",
	"AUDIO\\LAW3_11.WAV",
	"AUDIO\\LAW3_12.WAV",
	"AUDIO\\LAW3_13.WAV",
	"AUDIO\\LAW3_14.WAV",
	"AUDIO\\LAW3_16.WAV",
	"AUDIO\\LAW3_17.WAV",
	"AUDIO\\LAW3_18.WAV",
	"AUDIO\\LAW3_19.WAV",
	"AUDIO\\LAW3_20.WAV",
	"AUDIO\\LAW3_21.WAV",
	"AUDIO\\LAW3_22.WAV",
	"AUDIO\\LAW3_23.WAV",
	"AUDIO\\LAW3_24.WAV",
	"AUDIO\\LAW3_25.WAV",
	"AUDIO\\LAW4_1A.WAV",
	"AUDIO\\LAW4_1B.WAV",
	"AUDIO\\LAW4_1C.WAV",
	"AUDIO\\LAW4_1D.WAV",
	"AUDIO\\LAW4_10.WAV",
	"AUDIO\\LAW4_3.WAV",
	"AUDIO\\LAW4_4.WAV",
	"AUDIO\\LAW4_5.WAV",
	"AUDIO\\LAW4_6.WAV",
	"AUDIO\\LAW4_7.WAV",
	"AUDIO\\LAW4_8.WAV",
	"AUDIO\\LAW4_9.WAV",
	"AUDIO\\PHIL1_2.WAV",
	"AUDIO\\PHIL1_3.WAV",
	"AUDIO\\PHIL2_1.WAV",
	"AUDIO\\PHIL2_2.WAV",
	"AUDIO\\PHIL2_3.WAV",
	"AUDIO\\PHIL2_4.WAV",
	"AUDIO\\PHIL2_5.WAV",
	"AUDIO\\PHIL2_6.WAV",
	"AUDIO\\PHIL2_7.WAV",
	"AUDIO\\PHIL2_8.WAV",
	"AUDIO\\PHIL2_9.WAV",
	"AUDIO\\PHIL210.WAV",
	"AUDIO\\PHIL211.WAV",
	"AUDIO\\PORN1_1.WAV",
	"AUDIO\\PORN1_2.WAV",
	"AUDIO\\PORN1_3.WAV",
	"AUDIO\\PRN1_3A.WAV",
	"AUDIO\\PORN1_4.WAV",
	"AUDIO\\PORN1_5.WAV",
	"AUDIO\\PORN1_6.WAV",
	"AUDIO\\PORN1_7.WAV",
	"AUDIO\\PORN1_8.WAV",
	"AUDIO\\PORN1_9.WAV",
	"AUDIO\\PRN1_10.WAV",
	"AUDIO\\PRN1_11.WAV",
	"AUDIO\\PRN1_12.WAV",
	"AUDIO\\PRN1_13.WAV",
	"AUDIO\\PRN1_14.WAV",
	"AUDIO\\PRN1_15.WAV",
	"AUDIO\\PRN1_16.WAV",
	"AUDIO\\PRN1_17.WAV",
	"AUDIO\\PRN1_18.WAV",
	"AUDIO\\PRN1_19.WAV",
	"AUDIO\\PRN1_20.WAV",
	"AUDIO\\PRN1_21.WAV",
	"AUDIO\\PORN3_1.WAV",
	"AUDIO\\PORN3_2.WAV",
	"AUDIO\\PORN3_3.WAV",
	"AUDIO\\PORN3_4.WAV",
	"AUDIO\\PSYCH_1.WAV",
	"AUDIO\\PSYCH_2.WAV",
	"AUDIO\\ROK2_01.WAV",
	"AUDIO\\ROK3_1.WAV",
	"AUDIO\\ROK3_2.WAV",
	"AUDIO\\ROK3_3.WAV",
	"AUDIO\\ROK3_4.WAV",
	"AUDIO\\ROK3_5.WAV",
	"AUDIO\\ROK3_6.WAV",
	"AUDIO\\ROK3_7.WAV",
	"AUDIO\\ROK3_8.WAV",
	"AUDIO\\ROK3_9.WAV",
	"AUDIO\\ROK3_10.WAV",
	"AUDIO\\ROK3_11.WAV",
	"AUDIO\\ROK3_12.WAV",
	"AUDIO\\ROK3_13.WAV",
	"AUDIO\\ROK3_14.WAV",
	"AUDIO\\ROK3_15.WAV",
	"AUDIO\\ROK3_16.WAV",
	"AUDIO\\ROK3_17.WAV",
	"AUDIO\\ROK3_18.WAV",
	"AUDIO\\ROK3_19.WAV",
	"AUDIO\\ROK3_20.WAV",
	"AUDIO\\ROK3_21.WAV",
	"AUDIO\\ROK3_22.WAV",
	"AUDIO\\ROK3_23.WAV",
	"AUDIO\\ROK3_24.WAV",
	"AUDIO\\ROK3_25.WAV",
	"AUDIO\\ROK3_26.WAV",
	"AUDIO\\ROK3_27.WAV",
	"AUDIO\\ROK3_62.WAV",
	"AUDIO\\ROK3_63.WAV",
	"AUDIO\\ROK3_64.WAV",
	"AUDIO\\ROK3_65.WAV",
	"AUDIO\\ROK3_66.WAV",
	"AUDIO\\ROK3_67.WAV",
	"AUDIO\\ROK3_68.WAV",
	"AUDIO\\ROK3_69.WAV",
	"AUDIO\\ROK3_70.WAV",
	"AUDIO\\ROK3_71.WAV",
	"AUDIO\\ROK3_73.WAV",
	"AUDIO\\RESC_1.WAV",
	"AUDIO\\RESC_2.WAV",
	"AUDIO\\RESC_3.WAV",
	"AUDIO\\RESC_4.WAV",
	"AUDIO\\RESC_5.WAV",
	"AUDIO\\RESC_6.WAV",
	"AUDIO\\RESC_7.WAV",
	"AUDIO\\RESC_8.WAV",
	"AUDIO\\RESC_9.WAV",
	"AUDIO\\RESC_10.WAV",
	"AUDIO\\ROK1_1A.WAV",
	"AUDIO\\ROK1_1B.WAV",
	"AUDIO\\ROK1_5.WAV",
	"AUDIO\\ROK1_6.WAV",
	"AUDIO\\ROK1_7.WAV",
	"AUDIO\\ROK1_8.WAV",
	"AUDIO\\ROK1_9.WAV",
	"AUDIO\\TAX1_1.WAV",
	"AUDIO\\TAX1_2.WAV",
	"AUDIO\\TAX1_3.WAV",
	"AUDIO\\TAX1_4.WAV",
	"AUDIO\\TAX1_5.WAV",
	"AUDIO\\TAX2_1.WAV",
	"AUDIO\\TAX2_2.WAV",
	"AUDIO\\TAX2_3.WAV",
	"AUDIO\\TAX2_4.WAV",
	"AUDIO\\TAX2_5.WAV",
	"AUDIO\\TAX2_6.WAV",
	"AUDIO\\TAX2_7.WAV",
	"AUDIO\\TAX3_1.WAV",
	"AUDIO\\TAX3_2.WAV",
	"AUDIO\\TAX3_3.WAV",
	"AUDIO\\TAX3_4.WAV",
	"AUDIO\\TAX3_5.WAV",
	"AUDIO\\TEX1_1.WAV",
	"AUDIO\\TEX1_2.WAV",
	"AUDIO\\TEX1_3.WAV",
	"AUDIO\\TEX1_4.WAV",
	"AUDIO\\TEX1_5.WAV",
	"AUDIO\\TEX1_6.WAV",
	"AUDIO\\TEX2_1.WAV",
	"AUDIO\\TEX3_1.WAV",
	"AUDIO\\TEX3_2.WAV",
	"AUDIO\\TEX3_3.WAV",
	"AUDIO\\TEX3_4.WAV",
	"AUDIO\\TEX3_5.WAV",
	"AUDIO\\TEX3_6.WAV",
	"AUDIO\\TEX3_7.WAV",
	"AUDIO\\TEX3_8.WAV",
	"AUDIO\\HAT_1A.WAV",
	"AUDIO\\INTRO1.WAV",
	"AUDIO\\INTRO2.WAV",
	"AUDIO\\INTRO3.WAV",
	"AUDIO\\INTRO4.WAV",
	"AUDIO\\MOB_01A.WAV",
	"AUDIO\\MOB_01B.WAV",
	"AUDIO\\MOB_01C.WAV",
	"AUDIO\\MOB_02A.WAV",
	"AUDIO\\MOB_02B.WAV",
	"AUDIO\\MOB_02C.WAV",
	"AUDIO\\MOB_03A.WAV",
	"AUDIO\\MOB_03B.WAV",
	"AUDIO\\MOB_03C.WAV",
	"AUDIO\\MOB_03D.WAV",
	"AUDIO\\MOB_03E.WAV",
	"AUDIO\\SHARK_1.WAV",
	"AUDIO\\SHARK_2.WAV",
	"AUDIO\\SHARK_3.WAV",
	"AUDIO\\SHARK_4.WAV",
	"AUDIO\\SHARK_5.WAV",
	"AUDIO\\MOB_04A.WAV",
	"AUDIO\\MOB_04B.WAV",
	"AUDIO\\MOB_04C.WAV",
	"AUDIO\\MOB_04D.WAV",
	"AUDIO\\MOB_05A.WAV",
	"AUDIO\\MOB_05B.WAV",
	"AUDIO\\MOB_05C.WAV",
	"AUDIO\\MOB_05D.WAV",
	"AUDIO\\MOB_06A.WAV",
	"AUDIO\\MOB_06B.WAV",
	"AUDIO\\MOB_06C.WAV",
	"AUDIO\\MOB_07A.WAV",
	"AUDIO\\MOB_07B.WAV",
	"AUDIO\\MOB_08A.WAV",
	"AUDIO\\MOB_08B.WAV",
	"AUDIO\\MOB_08C.WAV",
	"AUDIO\\MOB_08D.WAV",
	"AUDIO\\MOB_08E.WAV",
	"AUDIO\\MOB_08F.WAV",
	"AUDIO\\MOB_08G.WAV",
	"AUDIO\\MOB_09A.WAV",
	"AUDIO\\MOB_09B.WAV",
	"AUDIO\\MOB_09C.WAV",
	"AUDIO\\MOB_09D.WAV",
	"AUDIO\\MOB_09E.WAV",
	"AUDIO\\MOB_09F.WAV",
	"AUDIO\\MOB_10A.WAV",
	"AUDIO\\MOB_10B.WAV",
	"AUDIO\\MOB_10C.WAV",
	"AUDIO\\MOB_10D.WAV",
	"AUDIO\\MOB_10E.WAV",
	"AUDIO\\MOB_11A.WAV",
	"AUDIO\\MOB_11B.WAV",
	"AUDIO\\MOB_11C.WAV",
	"AUDIO\\MOB_11D.WAV",
	"AUDIO\\MOB_11E.WAV",
	"AUDIO\\MOB_11F.WAV",
	"AUDIO\\MOB_14A.WAV",
	"AUDIO\\MOB_14B.WAV",
	"AUDIO\\MOB_14C.WAV",
	"AUDIO\\MOB_14D.WAV",
	"AUDIO\\MOB_14E.WAV",
	"AUDIO\\MOB_14F.WAV",
	"AUDIO\\MOB_14G.WAV",
	"AUDIO\\MOB_14H.WAV",
	"AUDIO\\MOB_16A.WAV",
	"AUDIO\\MOB_16B.WAV",
	"AUDIO\\MOB_16C.WAV",
	"AUDIO\\MOB_16D.WAV",
	"AUDIO\\MOB_16E.WAV",
	"AUDIO\\MOB_16F.WAV",
	"AUDIO\\MOB_16G.WAV",
	"AUDIO\\MOB_17A.WAV",
	"AUDIO\\MOB_17B.WAV",
	"AUDIO\\MOB_17C.WAV",
	"AUDIO\\MOB_17D.WAV",
	"AUDIO\\MOB_17E.WAV",
	"AUDIO\\MOB_17G.WAV",
	"AUDIO\\MOB_17H.WAV",
	"AUDIO\\MOB_17I.WAV",
	"AUDIO\\MOB_17J.WAV",
	"AUDIO\\MOB_17K.WAV",
	"AUDIO\\MOB_17L.WAV",
	"AUDIO\\MOB_18A.WAV",
	"AUDIO\\MOB_18B.WAV",
	"AUDIO\\MOB_18C.WAV",
	"AUDIO\\MOB_18D.WAV",
	"AUDIO\\MOB_18E.WAV",
	"AUDIO\\MOB_18F.WAV",
	"AUDIO\\MOB_18G.WAV",
	"AUDIO\\MOB_20A.WAV",
	"AUDIO\\MOB_20B.WAV",
	"AUDIO\\MOB_20C.WAV",
	"AUDIO\\MOB_20D.WAV",
	"AUDIO\\MOB_20E.WAV",
	"AUDIO\\MOB_24A.WAV",
	"AUDIO\\MOB_24B.WAV",
	"AUDIO\\MOB_24C.WAV",
	"AUDIO\\MOB_24D.WAV",
	"AUDIO\\MOB_24E.WAV",
	"AUDIO\\MOB_24F.WAV",
	"AUDIO\\MOB_24G.WAV",
	"AUDIO\\MOB_24H.WAV",
	"AUDIO\\MOB_25A.WAV",
	"AUDIO\\MOB_25B.WAV",
	"AUDIO\\MOB_25C.WAV",
	"AUDIO\\MOB_25D.WAV",
	"AUDIO\\MOB_26A.WAV",
	"AUDIO\\MOB_26B.WAV",
	"AUDIO\\MOB_26C.WAV",
	"AUDIO\\MOB_26D.WAV",
	"AUDIO\\MOB_26E.WAV",
	"AUDIO\\MOB_29A.WAV",
	"AUDIO\\MOB_29B.WAV",
	"AUDIO\\MOB_29C.WAV",
	"AUDIO\\MOB_29D.WAV",
	"AUDIO\\MOB_29E.WAV",
	"AUDIO\\MOB_29F.WAV",
	"AUDIO\\MOB_29G.WAV",
	"AUDIO\\MOB_30A.WAV",
	"AUDIO\\MOB_30B.WAV",
	"AUDIO\\MOB_30C.WAV",
	"AUDIO\\MOB_30D.WAV",
	"AUDIO\\MOB_30E.WAV",
	"AUDIO\\MOB_30F.WAV",
	"AUDIO\\MOB_33A.WAV",
	"AUDIO\\MOB_33B.WAV",
	"AUDIO\\MOB_33C.WAV",
	"AUDIO\\MOB_33D.WAV",
	"AUDIO\\MOB_34A.WAV",
	"AUDIO\\MOB_34B.WAV",
	"AUDIO\\MOB_34C.WAV",
	"AUDIO\\MOB_34D.WAV",
	"AUDIO\\MOB_35A.WAV",
	"AUDIO\\MOB_35B.WAV",
	"AUDIO\\MOB_35C.WAV",
	"AUDIO\\MOB_35D.WAV",
	"AUDIO\\MOB_36A.WAV",
	"AUDIO\\MOB_36B.WAV",
	"AUDIO\\MOB_36C.WAV",
	"AUDIO\\MOB_40A.WAV",
	"AUDIO\\MOB_40B.WAV",
	"AUDIO\\MOB_40C.WAV",
	"AUDIO\\MOB_40D.WAV",
	"AUDIO\\MOB_40E.WAV",
	"AUDIO\\MOB_40F.WAV",
	"AUDIO\\MOB_40G.WAV",
	"AUDIO\\MOB_40H.WAV",
	"AUDIO\\MOB_40I.WAV",
	"AUDIO\\MOB_41A.WAV",
	"AUDIO\\MOB_41B.WAV",
	"AUDIO\\MOB_41C.WAV",
	"AUDIO\\MOB_41D.WAV",
	"AUDIO\\MOB_41E.WAV",
	"AUDIO\\MOB_41F.WAV",
	"AUDIO\\MOB_41G.WAV",
	"AUDIO\\MOB_41H.WAV",
	"AUDIO\\MOB_42A.WAV",
	"AUDIO\\MOB_42B.WAV",
	"AUDIO\\MOB_42C.WAV",
	"AUDIO\\MOB_42D.WAV",
	"AUDIO\\MOB_42E.WAV",
	"AUDIO\\MOB_43A.WAV",
	"AUDIO\\MOB_43B.WAV",
	"AUDIO\\MOB_43C.WAV",
	"AUDIO\\MOB_43D.WAV",
	"AUDIO\\MOB_43E.WAV",
	"AUDIO\\MOB_43F.WAV",
	"AUDIO\\MOB_43G.WAV",
	"AUDIO\\MOB_43H.WAV",
	"AUDIO\\MOB_45A.WAV",
	"AUDIO\\MOB_45B.WAV",
	"AUDIO\\MOB_45C.WAV",
	"AUDIO\\MOB_45D.WAV",
	"AUDIO\\MOB_45E.WAV",
	"AUDIO\\MOB_45F.WAV",
	"AUDIO\\MOB_45G.WAV",
	"AUDIO\\MOB_45H.WAV",
	"AUDIO\\MOB_45I.WAV",
	"AUDIO\\MOB_45J.WAV",
	"AUDIO\\MOB_45K.WAV",
	"AUDIO\\MOB_45L.WAV",
	"AUDIO\\MOB_45M.WAV",
	"AUDIO\\MOB_45N.WAV",
	"AUDIO\\MOB_46A.WAV",
	"AUDIO\\MOB_46B.WAV",
	"AUDIO\\MOB_46C.WAV",
	"AUDIO\\MOB_46D.WAV",
	"AUDIO\\MOB_46E.WAV",
	"AUDIO\\MOB_46F.WAV",
	"AUDIO\\MOB_46G.WAV",
	"AUDIO\\MOB_46H.WAV",
	"AUDIO\\MOB_47A.WAV",
	"AUDIO\\MOB_52A.WAV",
	"AUDIO\\MOB_52B.WAV",
	"AUDIO\\MOB_52C.WAV",
	"AUDIO\\MOB_52D.WAV",
	"AUDIO\\MOB_52E.WAV",
	"AUDIO\\MOB_52F.WAV",
	"AUDIO\\MOB_52G.WAV",
	"AUDIO\\MOB_52H.WAV",
	"AUDIO\\MOB_54A.WAV",
	"AUDIO\\MOB_54B.WAV",
	"AUDIO\\MOB_54C.WAV",
	"AUDIO\\MOB_54D.WAV",
	"AUDIO\\MOB_54E.WAV",
	"AUDIO\\MOB_55A.WAV",
	"AUDIO\\MOB_55B.WAV",
	"AUDIO\\MOB_55C.WAV",
	"AUDIO\\MOB_55D.WAV",
	"AUDIO\\MOB_55E.WAV",
	"AUDIO\\MOB_55F.WAV",
	"AUDIO\\MOB_56A.WAV",
	"AUDIO\\MOB_56B.WAV",
	"AUDIO\\MOB_56C.WAV",
	"AUDIO\\MOB_56D.WAV",
	"AUDIO\\MOB_56E.WAV",
	"AUDIO\\MOB_56F.WAV",
	"AUDIO\\MOB_57A.WAV",
	"AUDIO\\MOB_57B.WAV",
	"AUDIO\\MOB_57C.WAV",
	"AUDIO\\MOB_57D.WAV",
	"AUDIO\\MOB_57E.WAV",
	"AUDIO\\MOB_58A.WAV",
	"AUDIO\\MOB_58B.WAV",
	"AUDIO\\MOB_58C.WAV",
	"AUDIO\\MOB_58D.WAV",
	"AUDIO\\MOB_58E.WAV",
	"AUDIO\\MOB_58F.WAV",
	"AUDIO\\MOB_58G.WAV",
	"AUDIO\\MOB_61A.WAV",
	"AUDIO\\MOB_61B.WAV",
	"AUDIO\\MOB_62A.WAV",
	"AUDIO\\MOB_62B.WAV",
	"AUDIO\\MOB_62C.WAV",
	"AUDIO\\MOB_62D.WAV",
	"AUDIO\\MOB_63A.WAV",
	"AUDIO\\MOB_63B.WAV",
	"AUDIO\\MOB_63C.WAV",
	"AUDIO\\MOB_63D.WAV",
	"AUDIO\\MOB_63E.WAV",
	"AUDIO\\MOB_63F.WAV",
	"AUDIO\\MOB_63G.WAV",
	"AUDIO\\MOB_63H.WAV",
	"AUDIO\\MOB_63I.WAV",
	"AUDIO\\MOB_63J.WAV",
	"AUDIO\\MOB_66A.WAV",
	"AUDIO\\MOB_66B.WAV",
	"AUDIO\\MOB_68A.WAV",
	"AUDIO\\MOB_68B.WAV",
	"AUDIO\\MOB_68C.WAV",
	"AUDIO\\MOB_68D.WAV",
	"AUDIO\\MOB_70A.WAV",
	"AUDIO\\MOB_70B.WAV",
	"AUDIO\\MOB_71A.WAV",
	"AUDIO\\MOB_71B.WAV",
	"AUDIO\\MOB_71C.WAV",
	"AUDIO\\MOB_71D.WAV",
	"AUDIO\\MOB_71E.WAV",
	"AUDIO\\MOB_71F.WAV",
	"AUDIO\\MOB_71G.WAV",
	"AUDIO\\MOB_71H.WAV",
	"AUDIO\\MOB_71I.WAV",
	"AUDIO\\MOB_71J.WAV",
	"AUDIO\\MOB_71K.WAV",
	"AUDIO\\MOB_71L.WAV",
	"AUDIO\\MOB_71M.WAV",
	"AUDIO\\MOB_71N.WAV",
	"AUDIO\\MOB_72A.WAV",
	"AUDIO\\MOB_72B.WAV",
	"AUDIO\\MOB_72C.WAV",
	"AUDIO\\MOB_72D.WAV",
	"AUDIO\\MOB_72E.WAV",
	"AUDIO\\MOB_72F.WAV",
	"AUDIO\\MOB_72G.WAV",
	"AUDIO\\MOB_73A.WAV",
	"AUDIO\\MOB_73C.WAV",
	"AUDIO\\MOB_73D.WAV",
	"AUDIO\\MOB_73F.WAV",
	"AUDIO\\MOB_73G.WAV",
	"AUDIO\\MOB_73I.WAV",
	"AUDIO\\MOB_95A.WAV",
	"AUDIO\\MOB_96A.WAV",
	"AUDIO\\MOB_98A.WAV",
	"AUDIO\\MOB_99A.WAV",
	"AUDIO\\JOB1_1B.WAV",
	"AUDIO\\JOB1_1C.WAV",
	"AUDIO\\JOB1_1D.WAV",
	"AUDIO\\JOB2_1B.WAV",
	"AUDIO\\JOB2_2.WAV",
	"AUDIO\\JOB2_3.WAV",
	"AUDIO\\JOB2_4.WAV",
	"AUDIO\\JOB2_5.WAV",
	"AUDIO\\JOB2_6.WAV",
	"AUDIO\\JOB2_7.WAV",
	"AUDIO\\JOB2_8.WAV",
	"AUDIO\\JOB2_9.WAV",
	"AUDIO\\JOB3_1.WAV",
	"AUDIO\\JOB3_2.WAV",
	"AUDIO\\JOB3_3.WAV",
	"AUDIO\\JOB4_1.WAV",
	"AUDIO\\JOB4_2.WAV",
	"AUDIO\\JOB4_3.WAV",
	"AUDIO\\JOB5_1.WAV",
	"AUDIO\\JOB5_2.WAV",
	"AUDIO\\JOB5_3.WAV",
	"AUDIO\\BJM1_20.WAV",
	"AUDIO\\BJM1_4.WAV",
	"AUDIO\\BJM1_5.WAV",
	"AUDIO\\MERC_39.WAV",
	"AUDIO\\MONO_1.WAV",
	"AUDIO\\MONO_2.WAV",
	"AUDIO\\MONO_3.WAV",
	"AUDIO\\MONO_4.WAV",
	"AUDIO\\MONO_5.WAV",
	"AUDIO\\MONO_6.WAV",
	"AUDIO\\MONO_7.WAV",
	"AUDIO\\MONO_8.WAV",
	"AUDIO\\MONO_9.WAV",
	"AUDIO\\MONO10.WAV",
	"AUDIO\\MONO11.WAV",
	"AUDIO\\MONO12.WAV",
	"AUDIO\\MONO13.WAV",
	"AUDIO\\MONO14.WAV",
	"AUDIO\\MONO15.WAV",
	"AUDIO\\MONO16.WAV",
	"AUDIO\\FUD_01.WAV",
	"AUDIO\\FUD_02.WAV",
	"AUDIO\\FUD_03.WAV",
	"AUDIO\\FUD_04.WAV",
	"AUDIO\\FUD_05.WAV",
	"AUDIO\\FUD_06.WAV",
	"AUDIO\\FUD_07.WAV",
	"AUDIO\\FUD_08.WAV",
	"AUDIO\\FUD_09.WAV",
	"AUDIO\\FUD_10.WAV",
	"AUDIO\\FUD_11.WAV",
	"AUDIO\\FUD_12.WAV",
	"AUDIO\\FUD_13.WAV",
	"AUDIO\\FUD_14.WAV",
	"AUDIO\\FUD_15.WAV",
	"AUDIO\\FUD_16.WAV",
	"AUDIO\\FUD_17.WAV",
	"AUDIO\\FUD_18.WAV",
	"AUDIO\\FUD_19.WAV",
	"AUDIO\\FUD_20.WAV",
	"AUDIO\\BURG_01.WAV",
	"AUDIO\\BURG_02.WAV",
	"AUDIO\\BURG_03.WAV",
	"AUDIO\\BURG_04.WAV",
	"AUDIO\\BURG_05.WAV",
	"AUDIO\\BURG_06.WAV",
	"AUDIO\\BURG_07.WAV",
	"AUDIO\\BURG_08.WAV",
	"AUDIO\\BURG_09.WAV",
	"AUDIO\\BURG_10.WAV",
	"AUDIO\\BURG_11.WAV",
	"AUDIO\\BURG_12.WAV",
	"AUDIO\\CRUST01.WAV",
	"AUDIO\\CRUST02.WAV",
	"AUDIO\\CRUST03.WAV",
	"AUDIO\\CRUST04.WAV",
	"AUDIO\\CRUST05.WAV",
	"AUDIO\\CRUST06.WAV",
	"AUDIO\\CRUST07.WAV",
	"AUDIO\\CRUST08.WAV",
	"AUDIO\\CRUST09.WAV",
	"AUDIO\\BAND_01.WAV",
	"AUDIO\\BAND_02.WAV",
	"AUDIO\\BAND_03.WAV",
	"AUDIO\\BAND_04.WAV",
	"AUDIO\\BAND_05.WAV",
	"AUDIO\\BAND_06.WAV",
	"AUDIO\\BAND_07.WAV",
	"AUDIO\\BAND_08.WAV",
	"AUDIO\\SHAFT01.WAV",
	"AUDIO\\SHAFT02.WAV",
	"AUDIO\\SHAFT03.WAV",
	"AUDIO\\SHAFT04.WAV",
	"AUDIO\\SHAFT05.WAV",
	"AUDIO\\SHAFT06.WAV",
	"AUDIO\\SHAFT07.WAV",
	"AUDIO\\SHAFT08.WAV",
	"AUDIO\\PISS_01.WAV",
	"AUDIO\\PISS_02.WAV",
	"AUDIO\\PISS_03.WAV",
	"AUDIO\\PISS_04.WAV",
	"AUDIO\\PISS_05.WAV",
	"AUDIO\\PISS_06.WAV",
	"AUDIO\\PISS_07.WAV",
	"AUDIO\\PISS_08.WAV",
	"AUDIO\\PISS_09.WAV",
	"AUDIO\\PISS_10.WAV",
	"AUDIO\\PISS_11.WAV",
	"AUDIO\\PISS_12.WAV",
	"AUDIO\\PISS_13.WAV",
	"AUDIO\\PISS_14.WAV",
	"AUDIO\\PISS_15.WAV",
	"AUDIO\\PISS_16.WAV",
	"AUDIO\\PISS_17.WAV",
	"AUDIO\\PISS_18.WAV",
	"AUDIO\\PISS_19.WAV",
	"AUDIO\\GIMME01.WAV",
	"AUDIO\\GIMME02.WAV",
	"AUDIO\\GIMME03.WAV",
	"AUDIO\\GIMME04.WAV",
	"AUDIO\\GIMME05.WAV",
	"AUDIO\\GIMME06.WAV",
	"AUDIO\\GIMME07.WAV",
	"AUDIO\\GIMME08.WAV",
	"AUDIO\\GIMME09.WAV",
	"AUDIO\\GIMME10.WAV",
	"AUDIO\\GIMME11.WAV",
	"AUDIO\\GIMME12.WAV",
	"AUDIO\\GIMME13.WAV",
	"AUDIO\\GIMME14.WAV",
	"AUDIO\\GIMME15.WAV",
	"AUDIO\\BUST_01.WAV",
	"AUDIO\\BUST_02.WAV",
	"AUDIO\\BUST_03.WAV",
	"AUDIO\\BUST_04.WAV",
	"AUDIO\\BUST_05.WAV",
	"AUDIO\\BUST_06.WAV",
	"AUDIO\\BUST_07.WAV",
	"AUDIO\\BUST_08.WAV",
	"AUDIO\\BUST_09.WAV",
	"AUDIO\\BUST_10.WAV",
	"AUDIO\\BUST_11.WAV",
	"AUDIO\\BUST_12.WAV",
	"AUDIO\\BUST_13.WAV",
	"AUDIO\\BUST_14.WAV",
	"AUDIO\\BUST_15.WAV",
	"AUDIO\\BUST_16.WAV",
	"AUDIO\\BUST_17.WAV",
	"AUDIO\\BUST_18.WAV",
	"AUDIO\\BUST_19.WAV",
	"AUDIO\\BUST_20.WAV",
	"AUDIO\\BUST_21.WAV",
	"AUDIO\\BUST_22.WAV",
	"AUDIO\\BUST_23.WAV",
	"AUDIO\\BUST_24.WAV",
	"AUDIO\\BUST_25.WAV",
	"AUDIO\\BUST_26.WAV",
	"AUDIO\\BUST_27.WAV",
	"AUDIO\\BUST_28.WAV",
};
