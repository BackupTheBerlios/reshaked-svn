//
// C++ Interface: compressor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "dsp/envelope.h"
#include "dsp/formulas.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/


class CompressorBase {
	
	float mix_rate;
	
	float attack;
	float release;
	float ratio;	
	float threshold;
	
	float c_attack;
	float c_release;
	
	float state;
		
	void recompute_coeffs();
		

protected:
		
	_FORCE_INLINE_ float process(float p_energy) {
	
		p_energy+=UNDENORMAL_OFFSET; 
		
		float db = energy_to_db( p_energy );
		
		float over = db - threshold;
		
		if (over<0)
			over=0;
			
		over+=UNDENORMAL_OFFSET; // this makes the DC offset to never be zero (avoids denormals)
							
		// run the envelope detector 
		if ( over > state ) 
			state=over+c_attack*(state-over);
		else
			state=over+c_release*(state-over);
			
		over = state - UNDENORMAL_OFFSET; // then, this avoids the signal to constantly gain reduct

		return db_to_energy( over*(ratio-1.0) );
	}
	
	CompressorBase();		
public:

	void set_attack(float p_attack);
	float get_attack() const;
	
	void set_release(float p_release);
	float get_release() const;
	
	void set_threshold(float p_threshold);
	float get_threshold() const;
	
	void set_ratio(float p_ratio);
	float get_ratio() const;
	
	void set_mix_rate(float p_mix_rate);
	float get_mix_rate() const;
	
	virtual ~CompressorBase() {}
};


class CompressorPeak : public CompressorBase {

public:

	_FORCE_INLINE_ float process_1(sample_t mono) {
	
		return process( fabsf(mono) );
	}

	_FORCE_INLINE_ float process_2(sample_t left,sample_t right) {
	
		float peak = std::max( fabsf(left), fabsf(right) );
		return process( peak );
	}
	
	_FORCE_INLINE_ float process_4(sample_t front_left,sample_t front_right,sample_t rear_left,sample_t rear_right) {
	
		float peak_front = std::max( fabsf(front_left), fabsf(front_right) );
		float peak_rear = std::max( fabsf(rear_left), fabsf(rear_right) );
		float peak = std::max( peak_front, peak_rear );
		return process( peak );
	}
	
};

typedef CompressorPeak CompressorRMS;

#endif
