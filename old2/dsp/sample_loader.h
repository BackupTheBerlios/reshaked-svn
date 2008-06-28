//
// C++ Interface: sample_loader
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAMPLE_LOADER_H
#define RESHAKEDSAMPLE_LOADER_H

#include "typedefs.h"
#include "dsp/sample.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SampleLoader {
public:
	
	enum LoadStatus {
		
		LOAD_OK,
		LOAD_UNRECOGNIZED,
		LOAD_INVALID,
	};
	
	virtual LoadStatus load_sample(String p_filename, Sample *p_sample)=0;

	SampleLoader();
	
	virtual ~SampleLoader();

};

}

#endif
