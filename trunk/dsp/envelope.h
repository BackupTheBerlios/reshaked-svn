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

#include <vector>

namespace ReShaked {

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

	InterpolationType interp_type;
	
	struct Node {

		int offset;
		float value;
	};

	std::vector <Node> node;

	bool on;

	bool loop_on;

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

	///////// METHODS /////////////
	
public:



//max and min height for nodes
	void set_max_value(float p_max) { max_value=p_max; }
	void set_min_value(float p_min) { min_value=p_min; }

	float get_max_value() { return max_value; }
	float get_min_value() { return min_value; }
	float get_max_nodes() { return max_nodes; }
	float get_min_nodes() { return min_nodes; }

//node management
	int get_node_count();
	void set_node(int p_node,int p_ofs,float p_value);
	int add_node_at_offset(int p_offset,float p_value);
	void delete_node(int p_node);
	float get_node_value(int p_node);
	int get_node_offset(int p_node);

//get envelope val at pos
	float get_value_at_pos(float p_pos); //interpolated 


//loop point management
	void set_loop_begin(int p_node);
	void set_loop_end(int p_node);

	void set_sustain_loop_begin(int p_node);
	void set_sustain_loop_end(int p_node);

	int get_loop_begin();
	int get_sustain_loop_begin();

	int get_loop_end();
	int get_sustain_loop_end();

	void set_loop_enabled (bool p_enabled);
	bool is_loop_enabled();
	void set_sustain_loop_enabled (bool p_enabled);
	bool is_sustain_loop_enabled();
	void set_enabled (bool p_enabled);
	bool is_enabled();

	void set_max_nodes(int p_max); // -1 infinite nodes
	void set_min_nodes(int p_min);

	void set_release_time(int p_time);
	int get_release_time();
	
	void reset();

	Envelope();

};

}
#endif
