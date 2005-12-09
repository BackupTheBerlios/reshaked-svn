//
// C++ Implementation: editor_blocklist
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

/* THESE HELPER MACROS NEED pattern_track DEFINED */
static Track_Pattern::Note _fix_macro(const Track_Pattern::Note &p_note) { return p_note; } //fix template annoyance
#define SET_NOTE(m_pos,m_note) d->undo_stream.add_command( Command3(&commands,&EditorCommands::set_note,pattern_track,m_pos,_fix_macro(m_note)) )
#define SET_NOTE_AT_CURSOR(m_note) SET_NOTE( Track_Pattern::Position(d->cursor.get_tick_pos(),d->pattern_edit.column),_fix_macro(m_note))


BlockList* Editor::get_blocklist(int p_idx) {
	
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		if (blocks==p_idx)
			return t;
		blocks++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			if (blocks==p_idx)
				return t->get_visible_automation(j);
			blocks++;
		}
	}
	
	return NULL;
	
}
int Editor::get_blocklist_count() {
	
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		blocks++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			blocks++;
		}
	}
	
	return blocks;
}



int Editor::get_track_blocklist(int p_track) {
	
	ERR_FAIL_INDEX_V(p_track,d->song->get_track_count(),-1);
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		if (p_track==i)
			return blocks;
		
		blocks++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			blocks++;
		}
	}
	
	return -1;
}

int Editor::get_blocklist_track(int p_blocklist) {
	
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		if (blocks==p_blocklist)
			return i;
		
		blocks++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			if (blocks==p_blocklist)
				return i;
			blocks++;
		}
	}
	
	return -1;
}

int Editor::get_current_track() {
	
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		if (d->global_edit.current_blocklist==blocks)
			return i;
		blocks++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			if (d->global_edit.current_blocklist==blocks)
				return i;
			
			blocks++;
		}
	}
	
	return -1;
	
}
void Editor::set_current_blocklist(int p_which) {
	
	if (p_which==d->global_edit.current_blocklist)
		return; //nothing happens
	ERR_FAIL_INDEX(p_which,get_blocklist_count());
	d->global_edit.current_blocklist=p_which;
	d->ui_update_notify->cursor_changed_blocklist();
	
}

void Editor::get_blocklists_sharing_block(BlockList::Block * p_block,std::list<int> *p_blocklist) {
	
	int bl=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		
		if (t->shares_block_data(p_block))
			p_blocklist->push_back(bl);
		
		bl++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			if (t->get_visible_automation(j)->shares_block_data(p_block))
				p_blocklist->push_back(bl);
		
			bl++;
		}
	}
}

void Editor::update_blocklists_sharing_block(BlockList::Block * p_block) {
	
	if (!p_block)
		return;
	std::list<int> blist;
	get_blocklists_sharing_block( p_block, &blist);
	d->ui_update_notify->update_blocklist_list( blist );
	
}



Tick Editor::get_block_list_max_len(BlockList *p_bl) {
	
	int block_count=p_bl->get_block_count();
	if (block_count==0)
		return 0;
	Tick len=0;
	switch (p_bl->get_block_type()) {
		
		case BlockList::BLOCK_TYPE_FIXED_TO_BEAT: {		
			
			len=p_bl->get_block_pos(block_count-1)+p_bl->get_block(block_count-1)->get_length();
		} break;
		case BlockList::BLOCK_TYPE_FREE_MOVING: {		
			
		} break;
	}
	
	return len;
}




bool Editor::blocklist_insert_block(BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos) {
	
	ERR_FAIL_COND_V( !p_blocklist->block_fits( p_pos, p_block->get_length() ) , true ); 
	d->undo_stream.begin("Insert Block");
	d->undo_stream.add_command( Command3(&commands,&EditorCommands::blocklist_insert_block,p_blocklist,p_block,p_pos) );
	d->undo_stream.end();
	
	return false;
	
}
void Editor::blocklist_remove_block(BlockList *p_blocklist,int p_which) {
	
	ERR_FAIL_INDEX(p_which,p_blocklist->get_block_count());
	d->undo_stream.begin("Remove Block");
	d->undo_stream.add_command( Command2(&commands,&EditorCommands::blocklist_remove_block,p_blocklist,p_which) );
	d->undo_stream.end();
	
}

void Editor::blocklist_resize_block(BlockList *p_blocklist,int p_which,Tick p_new_size) {
	
	
	//check for invalid block
	ERR_FAIL_INDEX(p_which,p_blocklist->get_block_count());
	//check that block fits at new size
	ERR_FAIL_COND(!p_blocklist->block_fits(p_blocklist->get_block_pos(p_which),p_new_size,p_which));
	
	
	BlockList::Block *block =p_blocklist->get_block(p_which);
	
	ERR_FAIL_COND(block->is_shared()); //cant resize a shared block!
	
	
	if (p_new_size==block->get_length())
		return; //sooooooooooo pointless
	
	/* First, save the data that is going to be deleted before resizing! */
	
	d->undo_stream.begin("Resize Block");
	
	if (p_new_size<block->get_length()) {
		if (dynamic_cast<Track_Pattern*>(p_blocklist)) {
			
			Track_Pattern *pattern_track=dynamic_cast<Track_Pattern*>(p_blocklist);
			Track_Pattern::PatternBlock *pb=pattern_track->get_block(p_which);
			
			Tick block_pos=pattern_track->get_block_pos(p_which);
			
			/* Prepare to chop (we cant delete while we go thru the array) */
			std::list<Track_Pattern::Position> to_chop;
			
			for (int i=0;i<pb->get_note_count();i++) {
				
				Tick note_pos_rel=pb->get_note_pos( i).tick;
				if (note_pos_rel>=p_new_size) {
					
					to_chop.push_back(Track_Pattern::Position(block_pos+note_pos_rel,pb->get_note_pos( i).column));
					/* Will be chopped */
				}
				
			}
			
			/* chop */
			for (std::list<Track_Pattern::Position>::iterator I=to_chop.begin();I!=to_chop.end();I++) {
			
				SET_NOTE(*I,Track_Pattern::Note());
			}
			
			
		} else if (dynamic_cast<Automation*>(p_blocklist)) {
			
			
			
			
		} else {
			
			d->undo_stream.end();
			
			ERR_PRINT("Block not recognized!\n");
			return;
			
		}
	}	
	
	d->undo_stream.add_command( Command3(&commands,&EditorCommands::blocklist_set_block_length,p_blocklist,p_which,p_new_size) );
	
	d->undo_stream.end();
	
	
}

void Editor::enter_blocklist(EditorData::EnterBLDir p_dir) {
	
	BlockList* bl = get_blocklist(d->global_edit.current_blocklist);
	ERR_FAIL_COND(bl == NULL);
	if (bl->get_type_name()=="pattern") {
		
		Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
		ERR_FAIL_COND(tp==NULL);
		if (p_dir==EditorData::ENTER_LEFT) {
				
			d->pattern_edit.column=0;
			d->pattern_edit.field=0;
			
		} else {
			
			d->pattern_edit.column=tp->get_visible_columns()-1;
			d->pattern_edit.field=1;
		}
	}

	d->ui_update_notify->cursor_changed_blocklist();
}

int Editor::get_current_blocklist() {
	
	return d->global_edit.current_blocklist;
}

int Editor::find_blocklist(BlockList *p_blocklist) {
	
	int bl=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		
		Track * t = d->song->get_track(i);
		
		if (t==p_blocklist)
			return bl;
		
		bl++;
		
		for (int j=0;j<t->get_visible_automation_count();j++) {
			
			if (t->get_visible_automation(j)==p_blocklist)
				return bl;
		
			bl++;
		}
	}
	
	return -1;
}


} //end of namespace
