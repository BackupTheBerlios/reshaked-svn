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

namespace ReShaked {


int Oscillator::get_max_submap_lebel() {
	
	return oscil_maps.size()-1;
}

const float *Oscillator::get_osc(int p_submap_level) {
	
	ERR_FAIL_INDEX_V(p_submap_level,oscil_maps.size(),NULL);
	
	return &oscil_maps[p_submap_level][0];
	
}
void Oscillator::set_frame(int p_frame,float p_value) {
	
	ERR_FAIL_INDEX(p_frame,BASE_SIZE);
	oscil_maps[0][p_frame]=p_value;
	
}

void Oscillator::update_submaps() {
	
	/* regenerate submaps */
	
	for (int i=1;i<oscil_maps.size();i++) {
		
		for (int j=0;j<oscil_maps[i].size();j++) {
			
			oscil_maps[i][j]=(oscil_maps[i-1][j*2]+oscil_maps[i-1][j*2+1])/2.0;
		}
	}
	
}


Oscillator::Oscillator() {
	
	
	int level_size=BASE_SIZE;
	
	while(level_size>=MIN_LEVEL_SIZE) {
		
		std::vector<float> v;
		v.resize(level_size);
		oscil_maps.push_back(v);
		level_size/=2;
	}
	
}


Oscillator::~Oscillator() {
	
}


}
