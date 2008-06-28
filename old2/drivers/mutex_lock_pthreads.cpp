
#include "mutex_lock_pthreads.h"


#ifdef POSIX_ENABLED

MutexLock *MutexLock_Pthreads::create_mutex_pthreads() {


  return new MutexLock_Pthreads;
}

#endif