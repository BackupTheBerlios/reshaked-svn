//
// C++ Interface: button_port_edit
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUTTON_PORT_EDIT_H
#define BUTTON_PORT_EDIT_H

#include "widgets/button.h"
#include "base/range.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class ButtonPortEdit : public GUI::Button {

	bool own_it;
	GUI::RangeBase *range;
	bool auto_hide;

	void _check_auto_hide();
	void _range_changed();
	void press_slot();
	bool pressing;

public:
	void set_range( GUI::RangeBase *p_range, bool p_own_it=false );
	void set_auto_hide( bool p_enabled );

	ButtonPortEdit();	
	~ButtonPortEdit();

};

#endif
