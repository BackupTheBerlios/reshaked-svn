//
// C++ Interface: mutex_lock_win32
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMUTEX_LOCK_WIN32_H
#define RESHAKEDMUTEX_LOCK_WIN32_H

#include "version.h"

#ifdef WIN32_ENABLED

#include "mutex_lock.h"
#include <windows.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/

class MutexLock_Win32 : public MutexLock  {

	CRITICAL_SECTION internal_mutex; 

public:

	void grab() {

		EnterCriticalSection(&internal_mutex);

	};
	bool try_grab() {

		return (TryEnterCriticalSection(&internal_mutex)==0);
	};

	void release() {

		LeaveCriticalSection(&internal_mutex);

	};

	static MutexLock* create_mutex_pthreads();

	MutexLock_Win32() {
		
		InitializeCriticalSection(&internal_mutex);

	}
	
	~MutexLock_Win32() {
		
		DeleteCriticalSection(&internal_mutex);

	}
};

}

#endif //WIN32_ENABLED

#endif
