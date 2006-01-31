//
// C++ Interface: global_properties
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_PROPERTIES_H
#define RESHAKEDGLOBAL_PROPERTIES_H

#include "property.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class GlobalProperties{
			    
	LocalProperty swing;
	LocalProperty volume;
	LocalProperty tempo;

			    
public:
	
	LocalProperty &get_swing();
	LocalProperty &get_volume();
	LocalProperty &get_tempo();
	
	void clear();
	
	GlobalProperties();
	~GlobalProperties();

};

}

#endif
