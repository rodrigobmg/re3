#define WITHWINDOWS
#include "common.h"

#include "DMAudio.h"
#include "Record.h"
#include "Timer.h"

uint32 CTimer::m_snTimeInMilliseconds;
uint32 CTimer::m_snTimeInMillisecondsPauseMode = 1;
uint32 CTimer::m_snTimeInMillisecondsNonClipped;
uint32 CTimer::m_snPreviousTimeInMilliseconds;
uint32 CTimer::m_FrameCounter;
float CTimer::ms_fTimeScale;
float CTimer::ms_fTimeStep;
float CTimer::ms_fTimeStepNonClipped;
bool  CTimer::m_UserPause;
bool  CTimer::m_CodePause;

uint32 oldPcTimer;

uint32 suspendPcTimer;

uint32 _nCyclesPerMS = 1;

LARGE_INTEGER _oldPerfCounter;

LARGE_INTEGER perfSuspendCounter;

uint32 suspendDepth;

#ifdef FIX_BUGS
double frameTime;
#endif

void CTimer::Initialise(void)
{
	debug("Initialising CTimer...\n");
	
	ms_fTimeScale = 1.0f;
	ms_fTimeStep = 1.0f;
	suspendDepth = 0;
	m_UserPause = false;
	m_CodePause = false;
	m_snTimeInMillisecondsNonClipped = 0;
	m_snPreviousTimeInMilliseconds = 0;
	m_snTimeInMilliseconds = 1;
	
	LARGE_INTEGER perfFreq;
	if ( QueryPerformanceFrequency(&perfFreq) )
	{
		OutputDebugString("Performance counter available\n");
		_nCyclesPerMS = uint32(perfFreq.QuadPart / 1000);
		QueryPerformanceCounter(&_oldPerfCounter);
	}
	else
	{
		OutputDebugString("Performance counter not available, using millesecond timer\n");
		_nCyclesPerMS = 0;
		oldPcTimer = RsTimer();
	}
	
	m_snTimeInMilliseconds = m_snPreviousTimeInMilliseconds;
	
	m_FrameCounter = 0;
	
	DMAudio.ResetTimers(m_snPreviousTimeInMilliseconds);
	
	debug("CTimer ready\n");
}

void CTimer::Shutdown(void)
{
	;
}

void CTimer::Update(void)
{
	m_snPreviousTimeInMilliseconds = m_snTimeInMilliseconds;
	
	if ( (double)_nCyclesPerMS != 0.0 )
	{
		LARGE_INTEGER pc;
		QueryPerformanceCounter(&pc);
		
		int32 updInCycles = (pc.LowPart - _oldPerfCounter.LowPart); // & 0x7FFFFFFF; pointless
		
		_oldPerfCounter = pc;
		
		float updInCyclesScaled = updInCycles * ms_fTimeScale;
		
		// We need that real frame time to fix transparent menu bug.
#ifndef FIX_BUGS
		double
#endif
		frameTime = updInCyclesScaled / (double)_nCyclesPerMS;

		m_snTimeInMillisecondsPauseMode = m_snTimeInMillisecondsPauseMode + frameTime;
		
		if ( GetIsPaused() )
			ms_fTimeStep = 0.0f;
		else
		{
			m_snTimeInMilliseconds = m_snTimeInMilliseconds + frameTime;
			m_snTimeInMillisecondsNonClipped = m_snTimeInMillisecondsNonClipped + frameTime;
			ms_fTimeStep = frameTime / 1000.0f * 50.0f;
		}
	}
	else
	{
		uint32 timer = RsTimer();
		
		uint32 updInMs = timer - oldPcTimer;
		
		// We need that real frame time to fix transparent menu bug.
#ifndef FIX_BUGS
		double
#endif
		frameTime = (double)updInMs * ms_fTimeScale;
		
		oldPcTimer = timer;
		
		m_snTimeInMillisecondsPauseMode = m_snTimeInMillisecondsPauseMode + frameTime;
															 
		if ( GetIsPaused() )
			ms_fTimeStep = 0.0f;
		else
		{
			m_snTimeInMilliseconds = m_snTimeInMilliseconds + frameTime;
			m_snTimeInMillisecondsNonClipped = m_snTimeInMillisecondsNonClipped + frameTime;
			ms_fTimeStep = frameTime / 1000.0f * 50.0f;
		}
	}
	
	if ( ms_fTimeStep < 0.01f && !GetIsPaused() )
		ms_fTimeStep = 0.01f;

	ms_fTimeStepNonClipped = ms_fTimeStep;
	
	if ( !CRecordDataForGame::IsPlayingBack() )
	{
		ms_fTimeStep = Min(3.0f, ms_fTimeStep);

		if ( (m_snTimeInMilliseconds - m_snPreviousTimeInMilliseconds) > 60 )
			m_snTimeInMilliseconds = m_snPreviousTimeInMilliseconds + 60;
	}
  
	if ( CRecordDataForChase::IsRecording() )
	{
		ms_fTimeStep = 1.0f;
		m_snTimeInMilliseconds = m_snPreviousTimeInMilliseconds + 16;
	}
  
	m_FrameCounter++;
}

void CTimer::Suspend(void)
{
	if ( ++suspendDepth > 1 )
		return;
	
	if ( (double)_nCyclesPerMS != 0.0 )
		QueryPerformanceCounter(&perfSuspendCounter);
	else
		suspendPcTimer = RsTimer();
}

void CTimer::Resume(void)
{
	if ( --suspendDepth != 0 )
		return;

	if ( (double)_nCyclesPerMS != 0.0 )
	{
		LARGE_INTEGER pc;
		QueryPerformanceCounter(&pc);

		_oldPerfCounter.LowPart += pc.LowPart - perfSuspendCounter.LowPart;
	}
	else
		oldPcTimer += RsTimer() - suspendPcTimer;
}

uint32 CTimer::GetCyclesPerMillisecond(void)
{
	if (_nCyclesPerMS != 0)
		return _nCyclesPerMS;
	else 
		return 1;
}

uint32 CTimer::GetCurrentTimeInCycles(void)
{
	if ( _nCyclesPerMS != 0 )
	{
		LARGE_INTEGER pc;
		QueryPerformanceCounter(&pc);
		return (pc.LowPart - _oldPerfCounter.LowPart); // & 0x7FFFFFFF; pointless
	}
	else
		return RsTimer() - oldPcTimer;
}

bool CTimer::GetIsSlowMotionActive(void)
{
	return ms_fTimeScale < 1.0f;
}

void CTimer::Stop(void)
{
	m_snPreviousTimeInMilliseconds = m_snTimeInMilliseconds;
}

void CTimer::StartUserPause(void)
{
	m_UserPause = true;
}

void CTimer::EndUserPause(void)
{
	m_UserPause = false;
}

uint32 CTimer::GetCyclesPerFrame()
{
	return 20;
}

