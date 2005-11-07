//
// C++ Implementation: blocklist_ui_pattern
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "blocklist_ui_pattern.h"
#include "visual_settings.h"

namespace ReShaked {


void BlockListUI_Pattern::paint_frames(QPainter& p) {
	
	int begin_pos=-1;
	int old_block_idx=-1;
	int visible_rows=song->get_cursor().get_window_size();

			
	for (int i=0;i<(visible_rows+1);i++) {
		Tick tick=song->get_cursor().get_snapped_window_tick_pos(i);
		int block_idx=track->get_block_idx_at_pos(tick);

		bool begin=(block_idx!=-1 && track->get_block_pos(block_idx)==tick);
				
		if (block_idx!=-1 && i==0 && !begin) {
			
			begin_pos=-1;		
		}
		
		if (i==0)
			old_block_idx=block_idx;
		
		bool end=((old_block_idx!=block_idx || i==visible_rows) && old_block_idx!=-1);
		old_block_idx=block_idx;
		
		if (end) {
			
			int row_size=VisualSettings::get_singleton()->get_editing_row_height();
			int begin_h=begin_pos*row_size;
			int len_h=(i-begin_pos)*row_size;
			SkinBox *sb=VisualSettings::get_singleton()->get_skin_box( SKINBOX_EDITING_PATTERN );
			sb->paint_into( p, 0,begin_h, width(), len_h ); 
			
		}
		
		if (begin)
			begin_pos=i;
		
	}
	
	
}

void BlockListUI_Pattern::paintEvent(QPaintEvent *e) {
	
	song->get_cursor().set_window_size( height() / VisualSettings::get_singleton()->get_editing_row_height() );
	
	QPainter p(this);
	
	p.fillRect(0,0,width(),height(),QColor(0,0,0));

	
	paint_frames(p);
	
	int visible_rows=song->get_cursor().get_window_size();
	

	
}

BlockListUI_Pattern::BlockListUI_Pattern(QWidget *p_parent,Song *p_song, int p_track) : BlockListUI_Base(p_parent) {

		
	
	song=p_song;
	track=dynamic_cast<Track_Pattern*>(p_song->get_track(p_track));
	ERR_FAIL_COND(track==NULL);
	
	setBackgroundRole(QPalette::NoRole);
	
	int fontwidth=VisualSettings::get_singleton()->get_pattern_font()->get_width();
	int columns=track->get_visible_columns();
	
	int min_chars_width=2+3*columns+( (columns>1)?(columns-1):0 );
	setFixedWidth( min_chars_width*fontwidth );

	
}


BlockListUI_Pattern::~BlockListUI_Pattern()
{
}


}
