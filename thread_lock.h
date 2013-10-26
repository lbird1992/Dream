#ifndef _DREAM_THREADLOCK_
#define _DREAM_THREADLOCK_

#include <Windows.h>

struct ThreadLock
{
	CRITICAL_SECTION lock;
	ThreadLock() { InitializeCriticalSection( &lock);}
	~ThreadLock() { DeleteCriticalSection( &lock);}

	void Lock() { EnterCriticalSection( &lock);}
	void Unlock() { LeaveCriticalSection( &lock);}
};

extern ThreadLock g_thread_lock_time;

#endif