//
// C++ Implementation: rack_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rack_view_frame.h"
#include "widgets/separator.h"
#include "widgets/button.h"
#include "widgets/icon.h"
#include "gui_custom/rsskin.h"

namespace ReShaked {


void RackViewFrame::track_combo_changed_slot(int p_track) {
	
	if (updating)
		return;
	
	selected_track=p_track;
	update_rack();

}

void RackViewFrame::update_track_combo() {

	updating=true;

	tracks_combo->clear();
	
	Song *s = editor->get_song();
	
	for(int i=0;i<s->get_track_count();i++) {
	
		tracks_combo->add_string( s->get_track(i)->get_name() );
	}

	tracks_combo->select( selected_track );
	
	updating=false;

}


void RackViewFrame::update_rack() {
	
	updating=true;



	updating=false;
}
void RackViewFrame::update_all() {

	update_track_combo();
	update_rack();
}

void RackViewFrame::track_list_changed_slot() {


	int tracks = editor->get_song()->get_track_count();
	
	if (tracks==0) {
	
		selected_track=-1;
	} else { 
	
		if (selected_track<0)
			selected_track=0;
		
		if (selected_track>=tracks)
			selected_track=tracks-1;
	}
	
	update_all();
	
}

void RackViewFrame::choose_plugin() {

	if(selected_track<0)
		return;
	plugin_chooser->choose_plugin(selected_track);
}

void RackViewFrame::set_in_window() {

	action_hb = add( new HBoxContainer );
	
	front_back.add( action_hb->add( new Button("Front") ) );
	front_back.add( action_hb->add( new Button("Back") ) );
	
	action_hb->add( new VSeparator );
	
	action_hb->add( new Label("Track: "));
	tracks_combo = action_hb->add( new ComboBox, 1 );
	tracks_combo->selected_signal.connect(this, &RackViewFrame::track_combo_changed_slot);
	
	action_hb->add( new VSeparator );
	
	action_hb->add( new Icon(bitmap(BITMAP_ICON_ADD)) );
	Button *add_b = action_hb->add( new Button("Add Plugin..") );
	add_b->pressed_signal.connect(this,&RackViewFrame::choose_plugin);

	action_hb->add( new VSeparator );

	preset_name = action_hb->add( new Label );
	
	action_hb->add( new Icon(bitmap(BITMAP_ICON_FILE_DISK))) ;
	Button *file_b = action_hb->add( new Button("Preset..") );
	
	viewport_stack = add( new StackContainer, 1);
	
	plugin_chooser = new PluginChooser(get_window(), editor);
}

RackViewFrame::RackViewFrame(GUI_UpdateNotify *p_notifier,Editor *p_editor) {
	
	editor = p_editor;
	
	selected_track=-1;
	updating=false;
	
	p_notifier->track_list_changed_signal.connect( this, &RackViewFrame::track_list_changed_slot );
	p_notifier->track_names_changed_signal.connect( this, &RackViewFrame::update_track_combo );
}


RackViewFrame::~RackViewFrame()
{
}


}
