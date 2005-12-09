//
// C++ Implementation: editor_track
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "editor.h"


namespace ReShaked {




void Editor::set_track_name(int p_track,String p_name) {
	
	Track *t=d->song->get_track(p_track);
	ERR_FAIL_COND(t==NULL);
	t->set_name( p_name );
}

void Editor::add_track(TrackType p_type,int p_channels,String p_name) {

	
	Track *track;
	switch (p_type) {
		
		case TRACK_TYPE_PATTERN: {
			
			
			track =  new Track_Pattern(p_channels);
			track->set_name(p_name);

		} break;
	}
	
	d->undo_stream.begin("Add Track");
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::add_track,track,d->song->get_track_count()));
	d->undo_stream.end();
	
	/* select the newly added track */
	for (int i=0;i<d->song->get_track_count();i++) {
		
		if (d->song->get_track(i)!=track)
			continue;
		int blocklist=get_track_blocklist( i );
		ERR_FAIL_COND(blocklist==-1);
		d->global_edit.current_blocklist=blocklist;
		enter_blocklist( EditorData::ENTER_LEFT );
	}
}

void Editor::remove_track(int p_which) {
	
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	
	d->undo_stream.begin("Remove Track");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::remove_track,p_which));
	d->undo_stream.end();
	
}



void Editor::move_track_left(int p_which) {
	
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	if (p_which==0)
		return;
	
	d->undo_stream.begin("Track Move Left");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::track_move_left,p_which));
	d->undo_stream.end();
	
}
void Editor::move_track_right(int p_which) {
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	
	if (p_which==(d->song->get_track_count()-1))
		return;
	
	d->undo_stream.begin("Track Move Right");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::track_move_right,p_which));
	d->undo_stream.end();
	
	
}





} //end of namespace
