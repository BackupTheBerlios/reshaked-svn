//
// C++ Implementation: editor_automation
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
#include "engine/track_pattern.h"
#include "keyboard_input.h"
#include "pianokeys.h"


namespace ReShaked {





void Editor::show_automation(int p_property,Track *p_track) {
	
	
	if (p_track==NULL) {
		
		p_track=d->song->get_track(get_current_track());
	}
	
	d->undo_stream.begin("Show Automation");
	d->undo_stream.add_command( Command2(&commands,&EditorCommands::automation_show,p_track,p_property) );
	d->undo_stream.end();
	
	
}

void Editor::hide_automation(int p_property,Track *p_track) {
	
	if (p_track==NULL) {
		
		p_track=d->song->get_track(get_current_track());
	}
	
	d->undo_stream.begin("Hide Automation");
	d->undo_stream.add_command( Command2(&commands,&EditorCommands::automation_hide,p_track,p_property) );
	d->undo_stream.end();
	
}

void Editor::add_automation_point(Automation *p_automation,Tick p_tick, float p_val,float p_lfo_depth) {
	
	d->undo_stream.begin("Add Point");
	
	/* CHECK That we are not overwriting a previous point, if we are, save it! */
	int block_idx=p_automation->get_block_idx_at_pos( p_tick );
	if (block_idx>=0) {
		
		
		Automation::AutomationData *d=p_automation->get_block( block_idx )->get_data();
		int point=d->get_exact_index( p_tick-p_automation->get_block_pos(block_idx) );
		if ( point!=INVALID_STREAM_INDEX) {
			/* POINT WILL BE OVERWRITTEN! SAVE IT! */
			remove_automation_point( p_automation, block_idx, point ); 
		}
	}
			
	d->undo_stream.add_command( Command4(&commands,&EditorCommands::add_automation_point,p_automation,p_tick,p_val,p_lfo_depth) );
	d->undo_stream.end();
	
	
}
void Editor::move_automation_point(Automation *p_automation,int p_block, int p_point, Tick p_to_tick, float p_to_val) {
	
	d->undo_stream.begin("Move Point");
	d->undo_stream.add_command( Command5(&commands,&EditorCommands::move_automation_point,p_automation,p_block,p_point,p_to_tick,p_to_val) );
	d->undo_stream.end();
	
}
void Editor::remove_automation_point(Automation *p_automation,int p_block,int p_point) {
	
	d->undo_stream.begin("Remove Point");
	d->undo_stream.add_command( Command3(&commands,&EditorCommands::remove_automation_point,p_automation,p_block,p_point) );
	d->undo_stream.end();
	
	
}



bool Editor::automation_edit_key_press(int p_key_value) {
	
	Automation *automation=dynamic_cast<Automation*>(get_blocklist( get_current_blocklist() ) );
	ERR_FAIL_COND_V(automation==NULL,false);
	
	if (handle_navigation_key_press( automation, p_key_value))
		return true;
	bool handled=true;
	
	SWITCH(p_key_value)
			
			CASE( KEYBIND("left") ) {

		if (d->global_edit.current_blocklist>0) {
			d->global_edit.current_blocklist--;
			enter_blocklist(EditorData::ENTER_RIGHT);
		}				
			}			
			CASE( KEYBIND("right") ) {
				if (d->global_edit.current_blocklist<(get_blocklist_count()-1)) {
					d->global_edit.current_blocklist++;
					enter_blocklist(EditorData::ENTER_LEFT);
				}				
			}
			DEFAULT
		
					handled=false;
	
		
			END_SWITCH;			
			return handled;	
}






} //end of namespace
