#pragma once

#define WATERCANNON_GRAVITY (0.009f)
#define WATERCANNON_LIFETIME (150)

class CWaterCannon
{
public:
	enum
	{
		NUM_SEGMENTPOINTS = 16,
	};
	
	int32 m_nId;
	int16 m_nCur;
	char _pad0[2];
	uint32 m_nTimeCreated;
	CVector m_avecPos[NUM_SEGMENTPOINTS];
	CVector m_avecVelocity[NUM_SEGMENTPOINTS];
	bool m_abUsed[NUM_SEGMENTPOINTS];
	
	void Init(void);
	void Update_OncePerFrame(int16 index);
	void Update_NewInput(CVector *pos, CVector *dir);
	void Render(void);
	void PushPeds(void);
};

static_assert(sizeof(CWaterCannon) == 412, "CWaterCannon: error");

class CWaterCannons
{
public:
	static CWaterCannon aCannons[NUM_WATERCANNONS];
	
	static void Init(void);
	static void UpdateOne(uint32 id, CVector *pos, CVector *dir);
	static void Update();
	static void Render(void);
};