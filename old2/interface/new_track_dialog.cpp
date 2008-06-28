//
// C++ Implementation: new_track_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "new_track_dialog.h"
#include "bundles/window_box.h"
#include "bundles/margin_group.h"
#include "containers/center_container.h"
#include "widgets/button.h"

namespace ReShaked {

void NewTrackDialog::show(String p_proposed_name) {
	
	
	name->set_text(p_proposed_name);
	name->get_focus();
	name->select_all();
	Window::show();
}
	
	
int NewTrackDialog::get_channels() {
	
	return combo->get_selected()+1;
}
String NewTrackDialog::get_name() {
	
	return name->get_text();
}
	
	
void NewTrackDialog::ok_pressed_text(String p_text) {
		
	ok_pressed();
}
void NewTrackDialog::ok_pressed() {
		
	hide();
	track_create_signal.call();
}
	
NewTrackDialog::NewTrackDialog(Window *p_parent) : Window(p_parent,MODE_POPUP,SIZE_CENTER) {
	
	WindowBox *wb = new WindowBox("New Track");
	
	
	set_root_frame(wb);
	
	
	MarginGroup *mg = wb->add( new MarginGroup("Track Name:         ") );
	
	name = mg->add(new LineEdit);
	
	name->text_enter_signal.connect(this,&NewTrackDialog::ok_pressed_text);
	
	mg = wb->add( new MarginGroup("Channels:") );
	
	combo = mg->add(new ComboBox);
	
	combo->add_string("Mono");
	combo->add_string("Stereo");
	combo->add_string("3");
	combo->add_string("Quad");
	combo->add_string("5");
	combo->add_string("6");
	combo->add_string("7");
	combo->add_string("8");
	
			
	wb->add( new CenterContainer )->set( new Button("Ok") )->pressed_signal.connect(this,&NewTrackDialog::ok_pressed);
	
	
}


NewTrackDialog::~NewTrackDialog()
{
}


}
