//
// C++ Interface: lfo
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLFO_H
#define RESHAKEDLFO_H

namespace ReShaked {

/**
	@author red <red@killy>
*/
class LFO{
public:	
	enum Mode {
		MODE_SINE,
		MODE_SAW_UP,
		MODE_SAW_DOWN,
		MODE_SQUARE,
	};
	
private:	
	
	unsigned long delay; ///< dependent on implementation, can be msecs, , etc.
	double rate; ///< hz,cycles/beat,etc
	double depth; ///< whoever-uses-this-dependent
	double phase; ///< 0 .. 1
	double random_depth; ///< 0 .. 1	

	Mode mode;
	
	unsigned long rate_unit_size;
	
public:
	
	void set_rate_unit_size(unsigned long p_size); ///<could be hz (mixing rate), size of a beat in ticks, etc
	
	void set_delay(unsigned long p_delay);
	void set_rate(double p_rate);
	void set_depth(double p_depth);
	void set_phase(double p_phase);
	void set_random_depth(double p_amount);
	void set_mode(Mode p_mode);
	
	unsigned long get_delay() const;
	double get_rate() const;
	double get_depth() const;
	double get_phase() const;
	double get_random_depth() const;
	Mode get_mode() const ;
	
	double get_value(unsigned long p_for_time) const; //get value since time happened
	
	LFO();
	~LFO();

};

}

#endif
