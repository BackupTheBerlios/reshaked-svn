/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
                          mutex_lock_pthreads.h  -  description
                             -------------------
    begin                : Sun Mar 18 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MUTEX_LOCK_PTHREADS_H
#define MUTEX_LOCK_PTHREADS_H

#include "version.h"

#ifdef POSIX_ENABLED


#include "mutex_lock.h"
#include <pthread.h>
#include <errno.h>
/**
  *@author Juan Linietsky
  */

/******************************
 mutex_lock_pthread.h
 ------------------------

pthreads version of mutex locking
********************************/

class MutexLock_Pthreads : public MutexLock  {

	pthread_mutex_t internal_mutex;
	pthread_mutexattr_t attribute;
	 
public:

	void grab() {

		pthread_mutex_lock(&internal_mutex);
	};
	bool try_grab() {

		return (pthread_mutex_trylock(&internal_mutex)==EBUSY);
	};

	void release() {

		pthread_mutex_unlock(&internal_mutex);
	};

	static MutexLock* create_mutex_pthreads();

	MutexLock_Pthreads() {
	
               /* Using a recursive mutex.. slower, but safer. */
                pthread_mutexattr_init(&attribute);
                pthread_mutexattr_settype(&attribute,PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(&internal_mutex,&attribute);

	}
};

#endif /*posix enabled*/
#endif
