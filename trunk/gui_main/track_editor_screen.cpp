//
// C++ Implementation: track_editor_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor_screen.h"
#include "containers/grid_container.h"
#include "track_editor_pattern.h"
#include "widgets/label.h"

void TrackEditorScreen::rebuild() {

	if (hb)
		delete hb;
		
	hb = vb_holder->add( new GUI::HBoxContainer,1 );
	hb->set_separation(0);
	
	for(int i=0;i<editor->get_song()->get_track_count();i++) {
	
		Track *t = editor->get_song()->get_track( i );
		
		PatternTrack *pt = dynamic_cast<PatternTrack*>(t);
		
		if (pt) {
		
			hb->add( new TrackEditorPattern(pt) );
		}

	}
	
}


TrackEditorScreen::TrackEditorScreen(Editor *p_editor,GUI_UpdateNotify *p_update_notify) {

	editor=p_editor;
	
	top = add( new TrackEditorTop );
	
	scroll = add( new GUI::ScrollContainer, true );
	
	
	vb_holder = scroll->set( new GUI::VBoxContainer );
	scroll->set_expand_v(true);
	scroll->set_expand_h(true);
	scroll->set_scroll_v(false);
	scroll->set_scroll_h(true);
	
	hb=NULL;
	
	p_update_notify->track_list_changed_signal.connect( this, &TrackEditorScreen::rebuild );
	
}


TrackEditorScreen::~TrackEditorScreen() {
}


