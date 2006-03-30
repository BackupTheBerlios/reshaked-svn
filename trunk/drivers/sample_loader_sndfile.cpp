//
// C++ Implementation: sample_loader_sndfile
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_loader_sndfile.h"
#include "version.h"
#ifdef SNDFILE_ENABLED

#include <stdio.h>
#include <sndfile.h>


namespace ReShaked {


SampleLoader::LoadStatus SampleLoader_SNDFile::load_sample(String p_filename, Sample *p_sample) {
	

	/* HEADER */
	SF_INFO sfinfo;
	memset(&sfinfo,0,sizeof(sfinfo));
	SNDFILE * sfile  =  sf_open(p_filename.utf8().get_data(), SFM_READ, &sfinfo);
	if (sfile==NULL) {

		return LOAD_UNRECOGNIZED;
	}

	if ((sfile!=NULL) && ((sfinfo.format&SF_FORMAT_TYPEMASK)==SF_FORMAT_RAW)) {
		
		sf_close(sfile);
		return LOAD_UNRECOGNIZED;
	}
	
	/* FORMAT */

	//create 
	p_sample->create(sfinfo.channels,sfinfo.frames);
	p_sample->set_base_freq(sfinfo.samplerate);
	
	
	/* DATA */

	int index=0;

	while (true) {

		int readcount=AUXBUFF_MAX_SIZE/sfinfo.channels;
		int result=sf_readf_float(sfile, auxbuff,readcount);

		for (int i=0;i<result;i++) {

			FrameData f(sfinfo.channels);
			
			for (int j=0;j<sfinfo.channels;j++)
				f.set(j,auxbuff[ sfinfo.channels*i+j ] );

			p_sample->set_frame(index,f);
			index++;
		}

		if (result!=readcount)
			break;
	}

	sf_close(sfile);

	return LOAD_OK;

}

SampleLoader_SNDFile::SampleLoader_SNDFile() {
	
	
}

SampleLoader_SNDFile::~SampleLoader_SNDFile() {
	
	
}

}

#endif /*libsndfile enabled */