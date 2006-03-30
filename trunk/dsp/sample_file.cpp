//
// C++ Implementation: sample_file
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_file.h"

namespace ReShaked {


SampleFile *SampleFile::singleton=NULL;

SampleFile *SampleFile::get_singleton() {
	
	return singleton;
}


Sample *SampleFile::load_sample(String p_filename) {
	
	Sample * smp = new Sample;
	
	for (int i=0;i<loaders.size();i++) {
		
		if (loaders[i]->load_sample(p_filename,smp)==SampleLoader::LOAD_OK)
			return smp;
	}
	
	delete smp;
	
	return NULL;
}

void SampleFile::add_loader(SampleLoader *p_loader) {
	
	loaders.push_back(p_loader);
}

SampleFile::SampleFile() {
	
	singleton=this;	
}


SampleFile::~SampleFile()
{
}


}
