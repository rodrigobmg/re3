#ifndef _WIN32
#include "common.h"
#include "crossplatform.h"
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#include "CdStream.h"
#include "rwcore.h"
#include "RwHelper.h"

#define CDDEBUG(f, ...)   debug ("%s: " f "\n", "cdvd_stream", ## __VA_ARGS__)
#define CDTRACE(f, ...)   printf("%s: " f "\n", "cdvd_stream", ## __VA_ARGS__)

// #define ONE_THREAD_PER_CHANNEL // Don't use if you're not on SSD/Flash. (Also you may want to benefit from this via using all channels in Streaming.cpp)

bool flushStream[MAX_CDCHANNELS];

struct CdReadInfo
{
	uint32 nSectorOffset;
	uint32 nSectorsToRead;
	void *pBuffer;
	bool bLocked;
	bool bReading;
	int32 nStatus;
#ifdef ONE_THREAD_PER_CHANNEL
	int8 nThreadStatus; // 0: created 1:initalized 2:abort now
    pthread_t pChannelThread;
    sem_t pStartSemaphore;
#endif
	sem_t pDoneSemaphore; // used for CdStreamSync
	int32 hFile;
};

char gCdImageNames[MAX_CDIMAGES+1][64];
int32 gNumImages;
int32 gNumChannels;

int32 gImgFiles[MAX_CDIMAGES]; // -1: error 0:unused otherwise: fd
char *gImgNames[MAX_CDIMAGES];

#ifndef ONE_THREAD_PER_CHANNEL
pthread_t _gCdStreamThread;
sem_t gCdStreamSema; // released when we have new thing to read(so channel is set)
int8 gCdStreamThreadStatus; // 0: created 1:initalized 2:abort now
Queue gChannelRequestQ;
bool _gbCdStreamOverlapped;
#endif

CdReadInfo *gpReadInfo;

int32 lastPosnRead;

int _gdwCdStreamFlags;

void *CdStreamThread(void* channelId);

void
CdStreamInitThread(void)
{
	int status;

#ifndef ONE_THREAD_PER_CHANNEL
	gChannelRequestQ.items = (int32 *)calloc(gNumChannels + 1, sizeof(int32));
	gChannelRequestQ.head = 0;
	gChannelRequestQ.tail = 0;
	gChannelRequestQ.size = gNumChannels + 1;
	ASSERT(gChannelRequestQ.items != nil );
    status = sem_init(&gCdStreamSema, 0, 0);
#endif


	if (status == -1) {
		CDTRACE("failed to create stream semaphore");
		ASSERT(0);
		return;
	}


	if ( gNumChannels > 0 )
	{
		for ( int32 i = 0; i < gNumChannels; i++ )
		{
		    status = sem_init(&gpReadInfo[i].pDoneSemaphore, 0, 0);

			if (status == -1)
			{
				CDTRACE("failed to create sync semaphore");
				ASSERT(0);
				return;
			}
#ifdef ONE_THREAD_PER_CHANNEL
		    status = sem_init(&gpReadInfo[i].pStartSemaphore, 0, 0);

			if (status == -1)
			{
				CDTRACE("failed to create start semaphore");
				ASSERT(0);
				return;
			}
            gpReadInfo[i].nThreadStatus = 0;
			int *channelI = (int*)malloc(sizeof(int));
			*channelI = i;
            status = pthread_create(&gpReadInfo[i].pChannelThread, NULL, CdStreamThread, (void*)channelI);

			if (status == -1)
			{
				CDTRACE("failed to create sync thread");
				ASSERT(0);
				return;
			}
#endif
		}
	}

#ifndef ONE_THREAD_PER_CHANNEL
    debug("Using one streaming thread for all channels\n");
    status = pthread_create(&_gCdStreamThread, NULL, CdStreamThread, nil);
    gCdStreamThreadStatus = 0;

    if (status == -1)
    {
        CDTRACE("failed to create sync thread");
        ASSERT(0);
        return;
    }
#else
    debug("Using seperate streaming threads for each channel\n");
#endif
}

void
CdStreamInit(int32 numChannels)
{
    struct statvfs fsInfo;

    if((statvfs("models/gta3.img", &fsInfo)) < 0)
    {
        CDTRACE("can't get filesystem info");
        ASSERT(0);
        return;
    }
#ifdef __linux__
	_gdwCdStreamFlags = O_RDONLY | O_NOATIME;
#else
	_gdwCdStreamFlags = O_RDONLY;
#endif
	// People say it's slower
/*
	if ( fsInfo.f_bsize <= CDSTREAM_SECTOR_SIZE )
	{
		_gdwCdStreamFlags |= O_DIRECT;
		debug("Using no buffered loading for streaming\n");
	}
*/
	void *pBuffer = (void *)RwMallocAlign(CDSTREAM_SECTOR_SIZE, fsInfo.f_bsize);
	ASSERT( pBuffer != nil );

	gNumImages = 0;

	gNumChannels = numChannels;

	gpReadInfo = (CdReadInfo *)calloc(numChannels, sizeof(CdReadInfo));
	ASSERT( gpReadInfo != nil );

	CDDEBUG("read info %p", gpReadInfo);

	CdStreamInitThread();

	ASSERT( pBuffer != nil );
	RwFreeAlign(pBuffer);
}

uint32
GetGTA3ImgSize(void)
{
	ASSERT( gImgFiles[0] > 0 );
    struct stat statbuf;

    char path[PATH_MAX];
    realpath(gImgNames[0], path);
    if (stat(path, &statbuf) == -1) {
		// Try case-insensitivity
		char* real = casepath(gImgNames[0], false);
		if (real)
		{
			realpath(real, path);
			free(real);
    		if (stat(path, &statbuf) != -1)
				goto ok;
		}

        CDTRACE("can't get size of gta3.img");
        ASSERT(0);
        return 0;
    }
ok:
	return statbuf.st_size;
}

void
CdStreamShutdown(void)
{
    // Destroying semaphores and free(gpReadInfo) will be done at threads
#ifndef ONE_THREAD_PER_CHANNEL
    gCdStreamThreadStatus = 2;
    sem_post(&gCdStreamSema);
#endif

#ifdef ONE_THREAD_PER_CHANNEL
    for ( int32 i = 0; i < gNumChannels; i++ ) {
        gpReadInfo[i].nThreadStatus = 2;
        sem_post(&gpReadInfo[i].pStartSemaphore);
    }
#endif
}


int32
CdStreamRead(int32 channel, void *buffer, uint32 offset, uint32 size)
{
	ASSERT( channel < gNumChannels );
	ASSERT( buffer != nil );

	lastPosnRead = size + offset;

	ASSERT( _GET_INDEX(offset) < MAX_CDIMAGES );
	int32 hImage = gImgFiles[_GET_INDEX(offset)];
	ASSERT( hImage > 0 );

	CdReadInfo *pChannel = &gpReadInfo[channel];
	ASSERT( pChannel != nil );

	pChannel->hFile = hImage - 1;

    if ( pChannel->nSectorsToRead != 0 || pChannel->bReading )
        return STREAM_NONE;

    pChannel->nStatus = STREAM_NONE;
    pChannel->nSectorOffset = _GET_OFFSET(offset);
    pChannel->nSectorsToRead = size;
    pChannel->pBuffer = buffer;
    pChannel->bLocked = 0;

#ifndef ONE_THREAD_PER_CHANNEL
    AddToQueue(&gChannelRequestQ, channel);
    if ( sem_post(&gCdStreamSema) != 0 )
        printf("Signal Sema Error\n");
#else
    if ( sem_post(&gpReadInfo[channel].pStartSemaphore) != 0 )
        printf("Signal Sema Error\n");
#endif

    return STREAM_SUCCESS;
}

int32
CdStreamGetStatus(int32 channel)
{
	ASSERT( channel < gNumChannels );
	CdReadInfo *pChannel = &gpReadInfo[channel];
	ASSERT( pChannel != nil );

#ifdef ONE_THREAD_PER_CHANNEL
    if (pChannel->nThreadStatus == 2)
        return STREAM_NONE;
#else
    if (gCdStreamThreadStatus == 2)
        return STREAM_NONE;
#endif

    if ( pChannel->bReading )
        return STREAM_READING;

    if ( pChannel->nSectorsToRead != 0 )
        return STREAM_WAITING;

    if ( pChannel->nStatus != STREAM_NONE )
    {
        int32 status = pChannel->nStatus;

        pChannel->nStatus = STREAM_NONE;

        return status;
    }

    return STREAM_NONE;
}

int32
CdStreamGetLastPosn(void)
{
	return lastPosnRead;
}

// wait for channel to finish reading
int32
CdStreamSync(int32 channel)
{
	ASSERT( channel < gNumChannels );
	CdReadInfo *pChannel = &gpReadInfo[channel];
	ASSERT( pChannel != nil );

	if (flushStream[channel]) {
#ifdef ONE_THREAD_PER_CHANNEL
		pChannel->nSectorsToRead = 0;
		pthread_kill(gpReadInfo[channel].pChannelThread, SIGINT);
#else
		if (pChannel->bReading) {
			pChannel->nSectorsToRead = 0;
			pthread_kill(_gCdStreamThread, SIGINT);
		} else {
			pChannel->nSectorsToRead = 0;
		}
#endif
		pChannel->bReading = false;
		flushStream[channel] = false;
	    return STREAM_NONE;
	}

    if ( pChannel->nSectorsToRead != 0 )
    {
        pChannel->bLocked = true;

        sem_wait(&pChannel->pDoneSemaphore);
    }

    pChannel->bReading = false;

    return pChannel->nStatus;
}

void
AddToQueue(Queue *queue, int32 item)
{
	ASSERT( queue != nil );
	ASSERT( queue->items != nil );
	queue->items[queue->tail] = item;

	queue->tail = (queue->tail + 1) % queue->size;

	if ( queue->head == queue->tail )
		debug("Queue is full\n");
}

int32
GetFirstInQueue(Queue *queue)
{
	ASSERT( queue != nil );
	if ( queue->head == queue->tail )
		return -1;

	ASSERT( queue->items != nil );
	return queue->items[queue->head];
}

void
RemoveFirstInQueue(Queue *queue)
{
	ASSERT( queue != nil );
	if ( queue->head == queue->tail )
	{
		debug("Queue is empty\n");
		return;
	}

	queue->head = (queue->head + 1) % queue->size;
}

void *CdStreamThread(void *param)
{
	debug("Created cdstream thread\n");

#ifndef ONE_THREAD_PER_CHANNEL
    while (gCdStreamThreadStatus != 2) {
		sem_wait(&gCdStreamSema);
        int32 channel = GetFirstInQueue(&gChannelRequestQ);
#else
    int channel = *((int*)param);
    while (gpReadInfo[channel].nThreadStatus != 2){
		sem_wait(&gpReadInfo[channel].pStartSemaphore);
#endif
		ASSERT( channel < gNumChannels );

		CdReadInfo *pChannel = &gpReadInfo[channel];
		ASSERT( pChannel != nil );

#ifdef ONE_THREAD_PER_CHANNEL
        if (gpReadInfo[channel].nThreadStatus == 0){
            gpReadInfo[channel].nThreadStatus = 1;
#else
        if (gCdStreamThreadStatus == 0){
            gCdStreamThreadStatus = 1;
#endif

#ifdef __linux__
            pid_t tid = syscall(SYS_gettid);
            int ret = setpriority(PRIO_PROCESS, tid, getpriority(PRIO_PROCESS, getpid()) + 1);
#endif
	}

		// spurious wakeup or we sent interrupt signal for flushing
		if(pChannel->nSectorsToRead == 0)
            continue;

		pChannel->bReading = true;

		if ( pChannel->nStatus == STREAM_NONE )
		{
            ASSERT(pChannel->hFile >= 0);
            ASSERT(pChannel->pBuffer != nil );

			lseek(pChannel->hFile, pChannel->nSectorOffset * CDSTREAM_SECTOR_SIZE, SEEK_SET);
		    if (read(pChannel->hFile, pChannel->pBuffer, pChannel->nSectorsToRead * CDSTREAM_SECTOR_SIZE) == -1) {
				// pChannel->nSectorsToRead == 0 at this point means we wanted to flush channel
                pChannel->nStatus = pChannel->nSectorsToRead == 0 ? STREAM_NONE : STREAM_ERROR;
            } else {
                pChannel->nStatus = STREAM_NONE;
            }
		}
	
#ifndef ONE_THREAD_PER_CHANNEL
		RemoveFirstInQueue(&gChannelRequestQ);
#endif

		pChannel->nSectorsToRead = 0;

		if ( pChannel->bLocked )
		{
			sem_post(&pChannel->pDoneSemaphore);
		}
		pChannel->bReading = false;
	}
#ifndef ONE_THREAD_PER_CHANNEL
    for ( int32 i = 0; i < gNumChannels; i++ )
    {
        sem_destroy(&gpReadInfo[i].pDoneSemaphore);
    }
    sem_destroy(&gCdStreamSema);
	free(gChannelRequestQ.items);
#else
    sem_destroy(&gpReadInfo[channel].pStartSemaphore);
    sem_destroy(&gpReadInfo[channel].pDoneSemaphore);
#endif
    free(gpReadInfo);
	pthread_exit(nil);
}

bool
CdStreamAddImage(char const *path)
{
	ASSERT(path != nil);
	ASSERT(gNumImages < MAX_CDIMAGES);

	gImgFiles[gNumImages] = open(path, _gdwCdStreamFlags);

	// Fix case sensitivity and backslashes.
	if (gImgFiles[gNumImages] == -1) {
		char* real = casepath(path, false);
		if (real)
		{
		    gImgFiles[gNumImages] = open(real, _gdwCdStreamFlags);
			free(real);
		}
	}

	if ( gImgFiles[gNumImages] == -1 ) {
		assert(false);
		return false;
	}

	gImgNames[gNumImages] = strdup(path);
	gImgFiles[gNumImages]++; // because -1: error 0: not used

	strcpy(gCdImageNames[gNumImages], path);

	gNumImages++;

	return true;
}

char *
CdStreamGetImageName(int32 cd)
{
	ASSERT(cd < MAX_CDIMAGES);
	if ( gImgFiles[cd] > 0)
		return gCdImageNames[cd];

	return nil;
}

void
CdStreamRemoveImages(void)
{
	for ( int32 i = 0; i < gNumChannels; i++ )
		CdStreamSync(i);

	for ( int32 i = 0; i < gNumImages; i++ )
	{
		close(gImgFiles[i] - 1);
		free(gImgNames[i]);
		gImgFiles[i] = 0;
	}

	gNumImages = 0;
}

int32
CdStreamGetNumImages(void)
{
	return gNumImages;
}
#endif
