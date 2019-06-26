#pragma once

enum {
	STREAM_OFFSET_MODEL = 0,
	STREAM_OFFSET_TXD = STREAM_OFFSET_MODEL+MODELINFOSIZE,
	NUMSTREAMINFO = STREAM_OFFSET_TXD+TXDSTORESIZE
};

enum StreamFlags
{
	STREAMFLAGS_DONT_REMOVE = 0x01,
	STREAMFLAGS_SCRIPTOWNED = 0x02,
	STREAMFLAGS_DEPENDENCY  = 0x04,
	STREAMFLAGS_PRIORITY    = 0x08,
	STREAMFLAGS_NOFADE      = 0x10,

	// TODO: this isn't named well, maybe CANT_REMOVE?
	STREAMFLAGS_NOT_IN_LIST = STREAMFLAGS_DONT_REMOVE|STREAMFLAGS_SCRIPTOWNED,
	STREAMFLAGS_KEEP_IN_MEMORY = STREAMFLAGS_DONT_REMOVE|STREAMFLAGS_SCRIPTOWNED|STREAMFLAGS_DEPENDENCY,
};

enum StreamLoadState
{
	STREAMSTATE_NOTLOADED = 0,
	STREAMSTATE_LOADED    = 1,
	STREAMSTATE_INQUEUE   = 2,
	STREAMSTATE_READING   = 3,	// channel is reading
	STREAMSTATE_STARTED   = 4,	// first part loaded
};

enum ChannelState
{
	CHANNELSTATE_IDLE = 0,
	CHANNELSTATE_READING = 1,
	CHANNELSTATE_STARTED = 2,
	CHANNELSTATE_ERROR = 3,
};

class CStreamingInfo
{
public:
	CStreamingInfo *m_next;
	CStreamingInfo *m_prev;
	uint8 m_loadState;
	uint8 m_flags;

	int16  m_nextID;
	uint32 m_position;
	uint32 m_size;

	bool GetCdPosnAndSize(uint32 &posn, uint32 &size);
	void SetCdPosnAndSize(uint32 posn, uint32 size);
	void AddToList(CStreamingInfo *link);
	void RemoveFromList(void);
	uint32 GetCdSize(void) { return m_size; }
	bool IsPriority(void) { return !!(m_flags & STREAMFLAGS_PRIORITY); }
};

struct CStreamingChannel
{
	int32 streamIds[4];
	int32 offsets[4];
	int32 state;
	int32 field24;
	int32 position;
	int32 size;
	int32 numTries;
	int32 status;	// from CdStream
};

class CDirectory;
enum eLevelName;

class CStreaming
{
public:
	static bool &ms_disableStreaming;
	static bool &ms_bLoadingBigModel;
	static int32 &ms_numModelsRequested;
	static CStreamingInfo *ms_aInfoForModel;	//[NUMSTREAMINFO]
	static CStreamingInfo &ms_startLoadedList;
	static CStreamingInfo &ms_endLoadedList;
	static CStreamingInfo &ms_startRequestedList;
	static CStreamingInfo &ms_endRequestedList;
	static int32 &ms_oldSectorX;
	static int32 &ms_oldSectorY;
	static int32 &ms_streamingBufferSize;
	static int8 **ms_pStreamingBuffer;	//[2]
	static int32 &ms_memoryUsed;
	static CStreamingChannel *ms_channel;	//[2]
	static int32 &ms_channelError;
	static int32 &ms_numVehiclesLoaded;
	static int32 *ms_vehiclesLoaded;	//[MAXVEHICLESLOADED]
	static int32 &ms_lastVehicleDeleted;
	static CDirectory *&ms_pExtraObjectsDir;
	static int32 &ms_numPriorityRequests;
	static bool &ms_hasLoadedLODs;
	static int32 &ms_currentPedGrp;
	static int32 ms_lastCullZone;
	static uint16 &ms_loadedGangs;
	static int32 ms_currentPedLoading;
	static int32 *ms_imageOffsets;	//[NUMCDIMAGES]
	static int32 &ms_lastImageRead;
	static int32 &ms_imageSize;
	static int32 &ms_memoryAvailable;

	static void Init(void);
	static void Shutdown(void);
	static void LoadCdDirectory(void);
	static void LoadCdDirectory(const char *dirname, int32 n);
	static bool ConvertBufferToObject(int8 *buf, int32 streamId);
	static bool FinishLoadingLargeFile(int8 *buf, int32 streamId);
	static void RequestModel(int32 model, int32 flags);
	static void ReRequestModel(int32 model) { RequestModel(model, ms_aInfoForModel[model].m_flags); }
	static void RequestTxd(int32 txd, int32 flags) { RequestModel(txd + STREAM_OFFSET_TXD, flags); }
	static void ReRequestTxd(int32 txd) { ReRequestModel(txd + STREAM_OFFSET_TXD); }
	static void RequestSubway(void);
	static void RequestBigBuildings(eLevelName level);
	static void RequestIslands(eLevelName level);
	static void RequestSpecialModel(int32 modelId, const char *modelName, int32 flags);
	static void RequestSpecialChar(int32 charId, const char *modelName, int32 flags);
	static void DecrementRef(int32 id);
	static void RemoveModel(int32 id);
	static void RemoveTxd(int32 id) { RemoveModel(id + STREAM_OFFSET_TXD); }
	static void RemoveUnusedBuildings(eLevelName level);
	static void RemoveBuildings(eLevelName level);
	static void RemoveUnusedBigBuildings(eLevelName level);
	static void RemoveIslandsNotUsed(eLevelName level);
	static void RemoveBigBuildings(eLevelName level);
	static bool RemoveLoadedVehicle(void);
	static bool RemoveLeastUsedModel(void);
	static void RemoveAllUnusedModels(void);
	static void RemoveUnusedModelsInLoadedList(void);
	static bool RemoveReferencedTxds(int32 mem);
	static int32 GetAvailableVehicleSlot(void);
	static bool IsTxdUsedByRequestedModels(int32 txdId);
	static bool AddToLoadedVehiclesList(int32 modelId);
	static bool IsObjectInCdImage(int32 id);
	static void HaveAllBigBuildingsLoaded(eLevelName level);
	static void SetModelIsDeletable(int32 id);
	static void SetModelTxdIsDeletable(int32 id);
	static void SetMissionDoesntRequireModel(int32 id);

	static int32 GetCdImageOffset(int32 lastPosn);
	static int32 GetNextFileOnCd(int32 position, bool priority);
	static void RequestModelStream(int32 ch);
	static bool ProcessLoadingChannel(int32 ch);
	static void RetryLoadFile(int32 ch);
	static void LoadRequestedModels(void);
	static void LoadAllRequestedModels(bool priority);
	static void FlushChannels(void);
	static void FlushRequestList(void);

	static void MakeSpaceFor(int32 size);
	static void ImGonnaUseStreamingMemory(void);
	static void IHaveUsedStreamingMemory(void);
	static void UpdateMemoryUsed(void);


	static void LoadInitialPeds(void);
	static void LoadInitialVehicles(void);

	static void LoadScene(CVector *pos);
	static void LoadAllRequestedModels(bool);
};
