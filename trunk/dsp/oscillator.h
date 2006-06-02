//
// C++ Interface: oscillator
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDOSCILLATOR_H
#define RESHAKEDOSCILLATOR_H

#include <vector>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class Oscillator {
public:	
	enum {
		BASE_SIZE_BITS=11,
		BASE_SIZE=(1<<BASE_SIZE_BITS),	//somewhat standard, OK base size for oscillator.
		MIN_LEVEL_BITS=2
	};
private:	
	
	std::vector<std::vector<float> > oscil_maps;
	
	float predownsample(int p_level,int p_index);
public:
	
	int get_max_submap_lebel();
	const float *get_osc(int p_submap_level=0);
	int get_osc_bits(int p_submap_level);
	void set_frame(int p_frame,float p_value);
	
	void update_submaps();
	
	Oscillator();
	~Oscillator();

};

}

#endif
