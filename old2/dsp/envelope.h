//
// C++ Interface: envelope
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ENVELOPE_H
#define ENVELOPE_H

/**
	@author red <red@killy>
*/

#include "typedefs.h"
#include <vector>

namespace ReShaked {

class EnvelopeProcess;




struct Envelope {

	enum {
	
		INFINITE_NODES=-1
	};
	
	enum InterpolationType {
		INTERP_NONE,
		INTERP_LINEAR,
		INTERP_CUBIC
	};
	
private:
friend class EnvelopeProcess;
	InterpolationType interp_type;
	
	struct Node {

		int offset;
		float value;
	};

	std::vector <Node> node;

	bool on;

	bool loop_on;
	
	bool cubic;

	int loop_begin_node;
	int loop_end_node;

	bool sustain_loop_on;
	int sustain_loop_begin_node;
	int sustain_loop_end_node;

	bool envelope_signed;

	
	int release_time; //extra release time, 0 for none

	float max_value;
	float min_value;

	int max_nodes; //-1 for infinite
	int min_nodes;

	String prefix;	
	int digits;
	
	
public:


	void set_cubic(bool p_cubic) { cubic=p_cubic; }
	bool is_cubic() { return cubic; }

//max and min height for nodes
	void set_max_value(float p_max) { max_value=p_max; }
	void set_min_value(float p_min) { min_value=p_min; }

	float get_max_value() const { return max_value; }
	float get_min_value() const{ return min_value; }
	float get_max_nodes() const{ return max_nodes; }
	float get_min_nodes() const{ return min_nodes; }

//node management
	int get_node_count() const;
	void set_node(int p_node,int p_ofs,float p_value);
	int add_node_at_offset(int p_offset,float p_value);
	void delete_node(int p_node);
	float get_node_value(int p_node) const;
	int get_node_offset(int p_node) const;

//get envelope val at pos
	float get_value_at_pos(float p_pos) const; //interpolated 


//loop point management
	void set_loop_begin(int p_node);
	void set_loop_end(int p_node);

	void set_sustain_loop_begin(int p_node);
	void set_sustain_loop_end(int p_node);

	int get_loop_begin() const;
	int get_sustain_loop_begin() const;

	int get_loop_end() const;
	int get_sustain_loop_end() const;

	void set_loop_enabled (bool p_enabled);
	bool is_loop_enabled() const;
	void set_sustain_loop_enabled (bool p_enabled);
	bool is_sustain_loop_enabled() const;
	void set_enabled (bool p_enabled);
	bool is_enabled() const;

	void set_max_nodes(int p_max); // -1 infinite nodes
	void set_min_nodes(int p_min);

	void set_release_time(int p_time);
	int get_release_time() const;
	
	void set_prefix(String p_prefix,int p_digits=0);
	String get_prefix() const;
	int get_digits() const;
	
	void reset(bool p_add_default_nodes=true);

	Envelope();

};


class EnvelopeProcess {
	
	Envelope *envelope;
	
	double pos;
	bool sustain;
	
	bool finished;		
public:	
	
	void set_envelope(Envelope *p_envelope) { envelope=p_envelope; }

	inline void process(double p_time) {
	
		pos+=p_time;
	
		if (!envelope->on)
			return;
		
		if (sustain && envelope->sustain_loop_on) {
		
			
			ERR_FAIL_INDEX(envelope->sustain_loop_end_node,envelope->node.size());
			ERR_FAIL_INDEX(envelope->sustain_loop_begin_node,envelope->node.size());
		
		
			double end_pos=(double)envelope->node[envelope->sustain_loop_end_node].offset;
			if (pos>end_pos) {
		
				pos=envelope->node[envelope->sustain_loop_begin_node].offset+(pos-end_pos);
			}
		
		} else if (envelope->loop_on) {
		
			ERR_FAIL_INDEX(envelope->loop_end_node,envelope->node.size());
			ERR_FAIL_INDEX(envelope->loop_begin_node,envelope->node.size());
		
		
			double end_pos=(double)envelope->node[envelope->loop_end_node].offset;
			if (pos>end_pos) {
		
				pos=envelope->node[envelope->loop_begin_node].offset+(pos-end_pos);
			}
		
		
		} else if (envelope->node.size() && pos>(float)envelope->node[envelope->node.size()-1].offset) {
			
			finished=true;
			pos=(float)envelope->node[envelope->node.size()-1].offset;
			
		}
	
	}

	inline float get() {
	
		return envelope->get_value_at_pos(pos);

	}

	inline bool is_active() { return envelope->on; }
	void set_sustain(bool p_sustain) { sustain=p_sustain; }
	bool has_finished() { return finished; }

	void reset() { pos=0; sustain=false; finished=false; }
	EnvelopeProcess() { envelope=NULL; pos=0; sustain=false; finished=false; }
};

}
#endif
