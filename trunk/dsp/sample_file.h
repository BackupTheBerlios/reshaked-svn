//
// C++ Interface: sample_file
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAMPLE_FILE_H
#define RESHAKEDSAMPLE_FILE_H

#include <vector>

#include "dsp/sample_loader.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SampleFile {
	
	static SampleFile *singleton;
	
	std::vector<SampleLoader*> loaders;
	
public:
	
	
	
	static SampleFile *get_singleton();
		
	SampleLoader::LoadStatus load_sample(String p_filename,Sample *p_sample);
	
	void add_loader(SampleLoader *p_loader);
	
	SampleFile();
	
	~SampleFile();

};

}

#endif
