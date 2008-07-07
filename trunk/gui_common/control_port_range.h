//
// C++ Interface: control_port_range
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONTROL_PORT_RANGE_H
#define CONTROL_PORT_RANGE_H

#include "base/range.h"
#include "engine/audio_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class ControlPortRange : public GUI::RangeBase {

	ControlPort *control_port;
public:

	virtual void set_min(double p_min);
	virtual double get_min();

	virtual void set_max(double p_max);
	virtual double get_max();

	virtual void set(double p_val);
	virtual double get();
	
	virtual void set_step(double p_step);
	virtual double get_step();
	
	virtual void set_page(double p_step);
	virtual double get_page();
	
	virtual double get_unit_value(); 
	virtual void set_unit_value(double p_v); 
	
	virtual String get_as_text();

	ControlPortRange(ControlPort *p_control_port);	
	~ControlPortRange();

};

#endif
