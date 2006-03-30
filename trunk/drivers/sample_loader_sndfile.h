//
// C++ Interface: sample_loader_sndfile
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAMPLE_LOADER_SNDFILE_H
#define RESHAKEDSAMPLE_LOADER_SNDFILE_H

#include "version.h"

#ifdef SNDFILE_ENABLED

#include "dsp/sample_loader.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SampleLoader_SNDFile : public SampleLoader {
	
	enum {
	
		AUXBUFF_MAX_SIZE=512
		
	};
	
	
	float auxbuff[AUXBUFF_MAX_SIZE];
	
public:
	
	LoadStatus load_sample(String p_filename, Sample *p_sample);
	
	SampleLoader_SNDFile();
	
	~SampleLoader_SNDFile();

};

}

#endif

#endif