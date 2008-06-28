//
// C++ Interface: new_track_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDNEW_TRACK_DIALOG_H
#define RESHAKEDNEW_TRACK_DIALOG_H

#include "base/window.h"
#include "bundles/combo_box.h"
#include "widgets/line_edit.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class NewTrackDialog : public Window {
	
	LineEdit *name;
	ComboBox *combo;
	
	void ok_pressed_text(String p_text);
	void ok_pressed();
public:
    
	Signal<> track_create_signal;
	
	int get_channels();
	String get_name();
	
	void show(String p_proposed_name);
	NewTrackDialog(Window *p_parent);
	~NewTrackDialog();
	
	

};

}

#endif
