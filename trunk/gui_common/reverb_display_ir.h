//
// C++ Interface: reverb_display_ir
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef REVERB_DISPLAY_IR_H
#define REVERB_DISPLAY_IR_H

#include "base/widget.h"
#include "nodes/hq_reverb_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class ReverbDisplayIR : public GUI::Widget {

	enum {
	
		LEN_SECONDS=5
	};

	
	ControlPort *predelay;   
	ControlPort *predelay_fb;   
	ControlPort *hpf;
	ControlPort *room_size;   
	ControlPort *damping;
	ControlPort *spread;
	
	double last_predelay;   
	double last_predelay_fb;   
	double last_hpf;
	double last_room_size;   
	double last_damping;
	double last_spread;
	
	float *buffer;
	int buffer_size;
	
	bool must_precompute_buffer();
	void precompute_buffer();
	GUI::Size get_minimum_size_internal();
	
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	
	
public:

	void check_for_changes();
	
	void set_properties(ControlPort *p_predelay, ControlPort *p_predelay_fb,ControlPort *p_hpf,ControlPort *p_room_size,ControlPort *p_damping,ControlPort *p_spread);
	
	ReverbDisplayIR();
	~ReverbDisplayIR();

};

#endif
