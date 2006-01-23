//
// C++ Implementation: global_view_blocks
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_view_blocks.h"

#include "engine/automation.h"
#include "engine/track_pattern.h"
#include "visual_settings.h"

namespace ReShaked {

struct NoteTmp {
	
	int note;
	Tick len;
	Tick pos;
};


void GlobalViewBlocks::paint_block_contents(QPainter &p, int p_x,int p_y,int p_w, int p_h, BlockList::Block *p_block,bool p_selected,int p_clip_to_len) {
	
	
	if (p_block && p_block->is_shared())
		GET_SKIN(SKINBOX_LINKED_BLOCK)->paint_into(p,p_x,p_y,p_w,p_h);
	
	if (dynamic_cast<Automation::AutomationBlock*>(p_block)) {
		
		Automation::AutomationBlock *b=dynamic_cast<Automation::AutomationBlock*>(p_block);
		
		p.setPen( QColor(0,0,0) );
		
		int prev_x=-1;
		
		for (int i=0;i<p_h;i++) {
			
			if (p_clip_to_len>0 && i>p_clip_to_len)
				break;
			Tick t=i*b->get_length()/p_h;
			float v=b->get_data()->get_tick_val( t );
			if (v<0) {
				prev_x=-1;
				continue;
			}
			int v_pixel=2+(int)(v*(p_w-4));
			int prev_y=i-1;
			
			if (prev_x==-1) {
				prev_x=v_pixel;
				prev_y=i;
			}
			
			p.drawLine(p_x+prev_x,p_y+prev_y,p_x+v_pixel,p_y+prev_y);
			prev_x=v_pixel;
			
		}
	} else if (dynamic_cast<Track_Pattern::PatternBlock*>(p_block)) {
		
		Track_Pattern::PatternBlock *b=dynamic_cast<Track_Pattern::PatternBlock*>(p_block);
		
		std::vector<NoteTmp> note_list;	
		std::vector<NoteTmp> note_stats;
				
		/* calculate amount of columns used */
		
		for (int i=0;i<b->get_note_count();i++)
			if (b->get_note_pos( i).column>=note_stats.size())
				note_stats.resize(b->get_note_pos( i).column+1);
		
		/* clean up */
		
		for (int i=0;i<note_stats.size();i++)
			note_stats[i].note=-1;
		
		
		for (int i=0;i<b->get_note_count();i++) {
			
			
			int col=b->get_note_pos(i).column;
			Tick pos=b->get_note_pos(i).tick;
			Track_Pattern::Note note=b->get_note(i);
			
			if (note.note<Track_Pattern::Note::MAX_NOTES) { //note n
				
				if (note_stats[col].note!=-1) {
					
					NoteTmp new_n;
					new_n.note=note_stats[col].note;
					new_n.pos=note_stats[col].pos; 
					new_n.len=pos-note_stats[col].pos; 
					note_list.push_back(new_n);
				}
				
				note_stats[col].note=note.note;
				note_stats[col].pos=pos; 
				
			} else if (note.note==Track_Pattern::Note::NOTE_OFF) {
				
				if (note_stats[col].note!=-1) {
					
					NoteTmp new_n;
					new_n.note=note_stats[col].note;
					new_n.pos=note_stats[col].pos; 
					new_n.len=pos-note_stats[col].pos; 
					note_list.push_back(new_n);
				}
				
				note_stats[col].note=-1;
				
			}
		}			
		/* add the ones that end at the end of the block */
		
		for (int i=0;i<note_stats.size();i++) {
			
			if (note_stats[i].note!=-1) {
				
				NoteTmp new_n;
				new_n.note=note_stats[i].note;
				new_n.pos=note_stats[i].pos; 
				new_n.len=b->get_length()-note_stats[i].pos; //len is used as pos
				note_list.push_back(new_n);
				
			}
			
		}
		
		
		/* calculate max and min */
		
		int max_note=0;
		int min_note=Track_Pattern::Note::MAX_NOTES;
		
		for (int i=0;i<note_list.size();i++) {
			
			if (note_list[i].note>max_note)
				max_note=note_list[i].note;
			if (note_list[i].note<min_note)
				min_note=note_list[i].note;
		}
		
		if (max_note==min_note)
			max_note++; //avoid divide by zero;
		/* paint!! */
		
		p.setPen(GET_QCOLOR(COLORLIST_PATTERN_EDIT_NOTE));
		for (int i=0;i<note_list.size();i++) {
			
			float note_xv=(float)(note_list[i].note-min_note)/(float)(max_note-min_note);
			int note_x=p_x+(int)(note_xv*(float)(p_w/2))+p_w/4;
			int pos=note_list[i].pos*p_h/(b->get_length());
			int len=note_list[i].len*p_h/(b->get_length());
			
			if (pos>=p_clip_to_len)
				continue;
			if ( (pos+len)>p_clip_to_len)
				len=p_clip_to_len-pos;
			
			p.drawRect(note_x,p_y+pos,1,len);
		}
			
	
	}
	
}


}
