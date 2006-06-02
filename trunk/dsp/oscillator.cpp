//
// C++ Implementation: oscillator
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "oscillator.h"
#include "error_macros.h"
#include <math.h>
namespace ReShaked {


int Oscillator::get_max_submap_lebel() {
	
	return oscil_maps.size()-1;
}

const float *Oscillator::get_osc(int p_submap_level) {
	
	ERR_FAIL_COND_V( p_submap_level<0 , NULL );
	
	
	if (p_submap_level>=oscil_maps.size())
		p_submap_level=oscil_maps.size()-1;
	
	return &oscil_maps[p_submap_level][0];
	
}


int Oscillator::get_osc_bits(int p_submap_level) {
	
	int bits=BASE_SIZE_BITS-p_submap_level;
	if (bits<MIN_LEVEL_BITS)
		bits=MIN_LEVEL_BITS;
	
	return bits;

}

void Oscillator::set_frame(int p_frame,float p_value) {
	
	ERR_FAIL_INDEX(p_frame,BASE_SIZE);
	oscil_maps[0][p_frame]=p_value;
	
}


float Oscillator::predownsample(int p_level,int p_index) {
	
	ERR_FAIL_INDEX_V(p_level,oscil_maps.size(),0);
	
	std::vector<float> &arr=oscil_maps[p_level];
	
	unsigned int pre=((unsigned int)p_index - 1)%arr.size();
	unsigned int idx=((unsigned int)p_index)%arr.size();
	unsigned int post=((unsigned int)p_index + 1)%arr.size();
	
	return arr[pre]*0.25+arr[idx]*0.50+arr[post]*0.25;
}

void Oscillator::update_submaps() {
	
	/* regenerate submaps */
	
	for (unsigned int i=1;i<oscil_maps.size();i++) {
		
		for (unsigned int j=0;j<oscil_maps[i].size();j++) {
			
			

			oscil_maps[i][j]=( predownsample(i-1,j*2) + predownsample(i-1,j*2+1) )/2.0;
		}
	}
	
}


Oscillator::Oscillator() {
	
	
	int level_size=BASE_SIZE;
	
	while(level_size>=(1<<MIN_LEVEL_BITS)) {
		
		std::vector<float> v;
		v.resize(level_size);
		oscil_maps.push_back(v);
		level_size/=2.0;
	}
	
	/* default to a sinewave */
	for (int i=0;i<BASE_SIZE;i++) {
		
		set_frame(i,sinf( ((float)i/(float)BASE_SIZE) * M_PI*2.0 ) );
	}
	
	update_submaps();
	
}


Oscillator::~Oscillator() {
	
}


}
