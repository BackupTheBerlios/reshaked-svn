//
// C++ Implementation: editor_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "editor_commands.h"
#include "editor.h"

namespace ReShaked {


CommandFunc* EditorCommands::set_note(bool p_no_undo,Track_Pattern *p_pattern,Track_Pattern::Position p_pos,Track_Pattern::Note p_note) {
	
	CommandFunc *ret=NULL;
	
	if (!p_no_undo) { //must undo, retrieve previous note
		ret=Command3(this,&EditorCommands::set_note,p_pattern,p_pos,p_pattern->get_note( p_pos ) );
	}
	
	p_pattern->set_note(p_pos,p_note);
	
	/* Handle Visual Updating */
	if (!d->global_edit.setnote_dont_update) {
		BlockList::Block *b=p_pattern->get_block_at_pos(p_pos.tick);
		if (b) { //may not be any block, though i think this will be checked before
			std::list<int> blist;
			editor->get_blocklists_sharing_block( b, &blist);
			d->ui_update_notify->update_blocklist_list( blist );
		}
	}
	
	return ret;
}

CommandFunc* EditorCommands::add_track(bool p_no_undo,Track *p_track,int p_at_pos) {

	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command1(this,&EditorCommands::remove_track,p_at_pos);
		ret->add_create_data( p_track );
	}
	
	d->song->add_track(p_track,p_at_pos);
	editor->revalidate_cursor();
	printf("trying to add track!\n");		
	d->ui_update_notify->track_list_changed();
	return ret;
}
CommandFunc* EditorCommands::remove_track(bool p_no_undo,int p_pos) {
	
	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command2(this,&EditorCommands::add_track,d->song->get_track(p_pos),p_pos);
		ret->add_delete_data( d->song->get_track(p_pos) );
	}
	
	d->song->remove_track(p_pos);
	editor->revalidate_cursor();
	
	d->ui_update_notify->track_list_changed();
	
	
	return ret;
}

}
