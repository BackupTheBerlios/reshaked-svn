//
// C++ Interface: adsr
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDADSR_H
#define RESHAKEDADSR_H

namespace ReShaked {

/**
	@author red <red@killy>
*/
class ADSR {
	
	double attack;
	double decay;
	double sustain;
	double release;
	
	double pos;
	
	bool sustaining;
	
	double val;
	bool finished;
public:
	
	void set_attack(double p_attack);
	void set_decay(double p_decay);
	void set_sustain(double p_sustain);
	void set_release(double p_release);
	
	void start();
	
	void process(double p_amount);
	
	void release_begin(); //
	bool is_finished();
	
	double get_val();
	
	ADSR();
	
	~ADSR();

};

}

#endif
