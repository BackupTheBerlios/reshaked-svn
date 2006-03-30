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
	
	enum {
		BASE_SIZE=2048,	//somewhat standard, OK base size for oscillator.
		MIN_LEVEL_SIZE=4
	};
private:	
	
	std::vector<std::vector<float> > oscil_maps;
	
public:
	
	int get_max_submap_lebel();
	const float *get_osc(int p_submap_level=0);
	void set_frame(int p_frame,float p_value);
	
	void update_submaps();
	
	Oscillator();
	~Oscillator();

};

}

#endif
