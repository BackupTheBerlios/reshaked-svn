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
	
	if (!d->global_edit.setnote_dont_update) 
		editor->update_blocklists_sharing_block( p_pattern->get_block_at_pos(p_pos.tick) );
	
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
	editor->disable_selection();
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
	editor->disable_selection();
	
	
	return ret;
}


CommandFunc* EditorCommands::track_move_left(bool p_no_undo,int p_which) {
	
	ERR_FAIL_COND_V(p_which==0,NULL);
	CommandFunc *ret=NULL;
	
	if (!p_no_undo) {
		
		ret=Command1(this,&EditorCommands::track_move_right,p_which-1);
	}
	
	Track *t=d->song->get_track(p_which);
	d->song->remove_track(p_which);
	d->song->add_track(t,p_which-1);
	editor->revalidate_cursor();
	
	d->ui_update_notify->track_list_changed();
	editor->disable_selection();
	
	return ret;
		
}

CommandFunc* EditorCommands::track_move_right(bool p_no_undo,int p_which) {
	
	ERR_FAIL_COND_V(p_which==(d->song->get_track_count()-1),NULL);
	
	CommandFunc *ret=NULL;
	
	if (!p_no_undo) {
		
		ret=Command1(this,&EditorCommands::track_move_left,p_which+1);
	}
	
	Track *t=d->song->get_track(p_which);
	d->song->remove_track(p_which);
	d->song->add_track(t,p_which+1);
	editor->revalidate_cursor();
	
	d->ui_update_notify->track_list_changed();
	editor->disable_selection();
	
	return ret;
	
}


CommandFunc* EditorCommands::automation_show(bool p_no_undo,Track *p_track,String p_which) {
	
	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command2(this,&EditorCommands::automation_hide,p_track,p_which);
	}
	
	p_track->show_automation(p_which);
	if (d->song->get_track(editor->get_current_track())==p_track)
		d->ui_update_notify->current_automation_status_changed();
	
	editor->disable_selection();
	return ret;
	
}
CommandFunc* EditorCommands::automation_hide(bool p_no_undo,Track *p_track,String p_which) {
	
	CommandFunc *ret=NULL;
	
	if (!p_no_undo) {
		
		ret=Command2(this,&EditorCommands::automation_show,p_track,p_which);
	}
	
	p_track->hide_automation(p_which);
	
	if (d->song->get_track(editor->get_current_track())==p_track)
		d->ui_update_notify->current_automation_status_changed();

	editor->disable_selection();
	
	return ret;
	
}

CommandFunc* EditorCommands::add_automation_point(bool p_no_undo,Automation *p_automation,Tick p_tick, float p_val, float p_lfo_depth) {
	
	
	int block_idx=p_automation->get_block_idx_at_pos( p_tick );
	ERR_FAIL_COND_V(block_idx==-1,NULL);
	Automation::AutomationData *ad=p_automation->get_block(block_idx)->get_data();
	Tick block_ofs=p_tick-p_automation->get_block_pos(block_idx);
	
	ERR_FAIL_COND_V(ad->get_exact_index(block_ofs)!=INVALID_STREAM_INDEX,NULL); //a point exists there!
	ad->insert(block_ofs, Automation::AutomationValue(p_val,p_lfo_depth) );
	int point_idx=ad->get_exact_index(block_ofs);
	ERR_FAIL_COND_V(point_idx==INVALID_STREAM_INDEX,NULL); //a point exists there!
	
	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command3(this,&EditorCommands::remove_automation_point,p_automation,block_idx,point_idx);
	}
	
	/* Handle Visual Updating */
	if (!d->global_edit.automation_point_dont_update) 
		editor->update_blocklists_sharing_block( p_automation->get_block(block_idx) );
	
	return ret;
	
	
}
CommandFunc* EditorCommands::move_automation_point(bool p_no_undo,Automation *p_automation,int p_block, int p_point, Tick p_to_tick, float p_to_val) {
	
	ERR_FAIL_COND_V( p_automation->get_block(p_block)==NULL , NULL); //block doesnt exist
	
	Automation::AutomationData *ad=p_automation->get_block(p_block)->get_data();
	
	/* SEVERAL CONSISTENCY CHECKS */
	
	ERR_FAIL_INDEX_V(p_point,ad->get_stream_size(),NULL); //point doesnt exist
	ERR_FAIL_COND_V(p_to_tick<0,NULL); //point tick less than block
	ERR_FAIL_COND_V(p_to_tick>=p_automation->get_block(p_block)->get_length(),NULL); //point tick greater than blocksize
	ERR_FAIL_COND_V( (p_point>0) && (p_to_tick<=ad->get_index_pos(p_point-1)), NULL); // new pos is less than previous point
	ERR_FAIL_COND_V( (p_point<(ad->get_stream_size()-1)) && (p_to_tick>=ad->get_index_pos(p_point+1)), NULL); //new pos suprpasses next point
	ERR_FAIL_COND_V( p_to_val<0 , NULL); //value invalid
	ERR_FAIL_COND_V( p_to_val>1 , NULL); //value invalid
	
	float lfo_depth=ad->get_index_value( p_point ).lfo_depth;
	
	CommandFunc *ret=NULL; //undo data
	if (!p_no_undo) {
		
		Tick prev_pos=ad->get_index_pos(p_point);
		float prev_val=ad->get_index_value(p_point).value;
		ret=Command5(this,&EditorCommands::move_automation_point,p_automation,p_block,p_point,prev_pos, prev_val);
	}
	
	
	ad->erase_index( p_point );
	ad->insert( p_to_tick, Automation::AutomationValue(p_to_val, lfo_depth) );
	
	/* Handle Visual Updating */
	if (!d->global_edit.automation_point_dont_update) 
		editor->update_blocklists_sharing_block( p_automation->get_block(p_block) );
	
	return ret;
	
}

CommandFunc* EditorCommands::remove_automation_point(bool p_no_undo,Automation *p_automation,int p_block,int p_point) {
	
	
	ERR_FAIL_COND_V( p_automation->get_block(p_block)==NULL , NULL); //block doesnt exist
	
	Automation::AutomationData *ad=p_automation->get_block(p_block)->get_data();
	
	ERR_FAIL_INDEX_V(p_point,ad->get_stream_size(),NULL); //point doesnt exist
	
	Tick point_ofs=ad->get_index_pos(p_point)+p_automation->get_block_pos(p_block);
	float point_val=ad->get_index_value(p_point).value;
	float point_lfo_depth=ad->get_index_value(p_point).lfo_depth;
	
	ad->erase_index( p_point ); //delete the point
	
	
	CommandFunc *ret=NULL; //undo data
	
	if (!p_no_undo) {
		
		ret=Command4(this,&EditorCommands::add_automation_point,p_automation,point_ofs,point_val,point_lfo_depth);
	}
	
	/* Handle Visual Updating */
	if (!d->global_edit.automation_point_dont_update) 
		editor->update_blocklists_sharing_block( p_automation->get_block(p_block) );
	
	
	return ret;
	
}


CommandFunc* EditorCommands::blocklist_insert_block(bool p_no_undo,BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos) {
	
	
	//failed to insert block?
	ERR_FAIL_COND_V( p_blocklist->insert_block( p_block, p_pos ) , NULL );

	int block_idx=p_blocklist->get_block_index( p_block );
	
	ERR_FAIL_COND_V( block_idx<0 , NULL );
	
	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command2(this,&EditorCommands::blocklist_remove_block,p_blocklist, block_idx);
		ret->add_create_data( p_block );
	}
	

	d->ui_update_notify->block_layout_changed();
	return ret;
	
	
	
}
CommandFunc* EditorCommands::blocklist_remove_block(bool p_no_undo,BlockList *p_blocklist,int p_which) {
	
	//block doesnt exist
	ERR_FAIL_INDEX_V(p_which, p_blocklist->get_block_count(), NULL );
	
	//remember block and pos
	BlockList::Block *block_ptr=p_blocklist->get_block( p_which );
	Tick block_pos=p_blocklist->get_block_pos(p_which);
			
	p_blocklist->remove_block( p_which );

	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command3(this,&EditorCommands::blocklist_insert_block,p_blocklist,block_ptr,block_pos);
		ret->add_delete_data( block_ptr );
	}
	

	d->ui_update_notify->block_layout_changed();
	return ret;
	
		
}

CommandFunc* EditorCommands::blocklist_set_block_length(bool p_no_undo,BlockList *p_blocklist,int p_which,Tick p_length) {
	
	//block doesnt exist
	ERR_FAIL_INDEX_V(p_which, p_blocklist->get_block_count(), NULL );
	ERR_FAIL_COND_V(p_length<=0,NULL);
	
	//block
	BlockList::Block *block=p_blocklist->get_block( p_which );
	
	
	CommandFunc *ret=NULL;
	if (!p_no_undo) {
		
		ret=Command3(this,&EditorCommands::blocklist_set_block_length,p_blocklist,p_which,block->get_length());
	}
	
	block->set_length( p_length );
	d->ui_update_notify->block_layout_changed();
	
	return ret;
	
}

}
