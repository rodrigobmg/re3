#pragma once

#include "ClumpModelInfo.h"
#include "PedType.h"
#include "PedStats.h"

enum PedNode {
	PED_TORSO = 0,	// has no bone!
	PED_MID,
	PED_HEAD,
	PED_UPPERARML,
	PED_UPPERARMR,
	PED_HANDL,
	PED_HANDR,
	PED_UPPERLEGL,
	PED_UPPERLEGR,
	PED_FOOTL,
	PED_FOOTR,
	PED_LOWERLEGR,
	PED_LOWERLEGL,

	PED_FOREARML,
	PED_FOREARMR,
	PED_CLAVICLEL,
	PED_CLAVICLER,
	PED_NECK,

	PED_NODE_MAX
};

class CPedModelInfo : public CClumpModelInfo
{
public:
	uint32 m_animGroup;
	ePedType m_pedType;
	ePedStats m_pedStatType;
	uint32 m_carsCanDrive;
	CColModel *m_hitColModel;
#ifdef PED_SKIN
	RpAtomic *m_head;
	RpAtomic *m_lhand;
	RpAtomic *m_rhand;
#endif

	static RwObjectNameIdAssocation m_pPedIds[PED_NODE_MAX];

	CPedModelInfo(void) : CClumpModelInfo(MITYPE_PED) { }
	void DeleteRwObject(void);
	void SetClump(RpClump *);

	void SetLowDetailClump(RpClump*);
	void CreateHitColModel(void);
	void CreateHitColModelSkinned(RpClump *clump);
	CColModel *GetHitColModel(void) { return m_hitColModel; }
	static CColModel *AnimatePedColModel(CColModel* colmodel, RwFrame* frame);
	CColModel *AnimatePedColModelSkinned(RpClump *clump);

#ifdef PED_SKIN
	static RpAtomic *findLimbsCb(RpAtomic *atomic, void *data);
	RpAtomic *getHead(void) { return m_head; }
	RpAtomic *getLeftHand(void) { return m_lhand; }
	RpAtomic *getRightHand(void) { return m_rhand; }
#endif
};
#ifndef PED_SKIN
static_assert(sizeof(CPedModelInfo) == 0x48, "CPedModelInfo: error");
#endif