//
// C++ Implementation: control_port_range
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "control_port_range.h"

void ControlPortRange::set_min(double p_min)  {

	
}

double ControlPortRange::get_min()  {

	return control_port->get_min();
}

void ControlPortRange::set_max(double p_max)  {


}
double ControlPortRange::get_max()  {

	return control_port->get_max();
}

void ControlPortRange::set(double p_val)  {

	control_port->set( p_val, true ); // shoudl go throgh editcommands....
	value_changed_signal.call( control_port->get() );
}
double ControlPortRange::get()  {

	return control_port->get();
}

void ControlPortRange::set_step(double p_step) {


}

double ControlPortRange::get_step()  {

	return control_port->get_step();
}

void ControlPortRange::set_page(double p_step)  {


}

double ControlPortRange::get_page()  {

	return 0;
}

double ControlPortRange::get_unit_value()  {

	return control_port->get_normalized();
}
void ControlPortRange::set_unit_value(double p_v)  {

	control_port->set_normalized(p_v); // go through editcommands...
	value_changed_signal.call( control_port->get() );	
}

String ControlPortRange::get_as_text() {

	return control_port->get_value_as_text( control_port->get() );
}


ControlPortRange::ControlPortRange(ControlPort *p_control_port) {

	control_port=p_control_port;
}


ControlPortRange::~ControlPortRange()
{
}


