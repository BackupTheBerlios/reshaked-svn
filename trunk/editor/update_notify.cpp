//
// C++ Implementation: update_notify
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "update_notify.h"

UpdateNotify *UpdateNotify::singleton=0;

UpdateNotify *UpdateNotify::get_singleton() {

	return singleton;
}

UpdateNotify::UpdateNotify()
{
	singleton=this;
}

UpdateNotify::~UpdateNotify()
{
}


