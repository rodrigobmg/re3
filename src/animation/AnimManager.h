#pragma once

#include "AnimBlendHierarchy.h"
#include "AnimationId.h"

enum AssocGroupId
{
	ASSOCGRP_STD,
	ASSOCGRP_PLAYER,
	ASSOCGRP_PLAYERROCKET,
	ASSOCGRP_PLAYER1ARMED,
	ASSOCGRP_PLAYER2ARMED,
	ASSOCGRP_PLAYERBBBAT,
	ASSOCGRP_SHUFFLE,
	ASSOCGRP_OLD,
	ASSOCGRP_GANG1,
	ASSOCGRP_GANG2,
	ASSOCGRP_FAT,
	ASSOCGRP_OLDFAT,
	ASSOCGRP_WOMAN,
	ASSOCGRP_WOMANSHOP,
	ASSOCGRP_BUSYWOMAN,
	ASSOCGRP_SEXYWOMAN,
	ASSOCGRP_OLDWOMAN,
	ASSOCGRP_FARWOMAN,
	ASSOCGRP_PANICCHUNKY,
	ASSOCGRP_PLAYERBACK,
	ASSOCGRP_PLAYERLEFT,
	ASSOCGRP_PLAYERRIGHT,
	ASSOCGRP_ROCKETBACK,
	ASSOCGRP_ROCKETLEFT,
	ASSOCGRP_ROCKETRIGHT,

	NUM_ANIM_ASSOC_GROUPS
};

class CAnimBlendAssociation;
class CAnimBlendAssocGroup;

// A block of hierarchies
struct CAnimBlock
{
	char name[24];
	int32 firstIndex;
	int32 numAnims;
};

struct AnimAssocDesc
{
	int32 animId;
	int32 flags;
};

struct AnimAssocDefinition
{
	char const *name;
	char const *blockName;
	int32 modelIndex;
	int32 numAnims;
	char const **animNames;
	AnimAssocDesc *animDescs;
};

class CAnimManager
{
	static const AnimAssocDefinition ms_aAnimAssocDefinitions[NUM_ANIM_ASSOC_GROUPS];
	static CAnimBlock ms_aAnimBlocks[2];
	static CAnimBlendHierarchy ms_aAnimations[250];
	static int32 ms_numAnimBlocks;
	static int32 ms_numAnimations;
	static CAnimBlendAssocGroup *ms_aAnimAssocGroups;
	static CLinkList<CAnimBlendHierarchy*> ms_animCache;
public:

	static void Initialise(void);
	static void Shutdown(void);
	static void UncompressAnimation(CAnimBlendHierarchy *anim);
	static CAnimBlock *GetAnimationBlock(const char *name);
	static CAnimBlendHierarchy *GetAnimation(const char *name, CAnimBlock *animBlock);
	static CAnimBlendHierarchy *GetAnimation(int32 n) { return &ms_aAnimations[n]; }
	static const char *GetAnimGroupName(AssocGroupId groupId);
	static CAnimBlendAssociation *CreateAnimAssociation(AssocGroupId groupId, AnimationId animId);
	static CAnimBlendAssociation *GetAnimAssociation(AssocGroupId groupId, AnimationId animId);
	static CAnimBlendAssociation *GetAnimAssociation(AssocGroupId groupId, const char *name);
	static CAnimBlendAssociation *AddAnimation(RpClump *clump, AssocGroupId groupId, AnimationId animId);
	static CAnimBlendAssociation *AddAnimationAndSync(RpClump *clump, CAnimBlendAssociation *syncanim, AssocGroupId groupId, AnimationId animId);
	static CAnimBlendAssociation *BlendAnimation(RpClump *clump, AssocGroupId groupId, AnimationId animId, float delta);
	static void LoadAnimFiles(void);
	static void LoadAnimFile(const char *filename);
	static void LoadAnimFile(int fd, bool compress);
	static void RemoveLastAnimFile(void);
};
