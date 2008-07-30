//
// C++ Implementation: button_port_edit
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "button_port_edit.h"



void ButtonPortEdit::_check_auto_hide() {

	if (!range)
		return;

	if (auto_hide) {
		if ( ((range->get_max()-range->get_min())-range->get_page()) <=0 )
			hide();
		else {
			show();
		}
	}

}

void ButtonPortEdit::_range_changed() {
		
	_check_auto_hide();
		
	if (pressing)
		return;
		
	set_pressed( range->get()==0 );

}

void ButtonPortEdit::press_slot(){


	pressing=true;
	
	range->set( is_pressed()?1:0 );
	
	pressing=false;

}


void ButtonPortEdit::set_range( GUI::RangeBase *p_range, bool p_own_it ){

	if (range && own_it)
		delete range;
	
	range = p_range;
	own_it=p_own_it;
	if (range)
	range->range_changed_signal.connect( this, &ButtonPortEdit::_range_changed );
	pressed_signal.connect( this, &ButtonPortEdit::press_slot );
}

void ButtonPortEdit::set_auto_hide( bool p_enabled ) {

    auto_hide=p_enabled;
}

ButtonPortEdit::ButtonPortEdit()
{
	range=NULL;
	auto_hide=false;
	pressing=false;
	own_it=false;

}


ButtonPortEdit::~ButtonPortEdit() {

	if (range && own_it)
		delete range;

}


