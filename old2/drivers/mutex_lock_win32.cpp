//
// C++ Implementation: mutex_lock_win32
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mutex_lock_win32.h"

#ifdef WIN32_ENABLED

namespace ReShaked {


MutexLock *MutexLock_Win32::create_mutex_pthreads() {

	return new MutexLock_Win32;
}


}


#endif //WIN32_ENABLED