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
#include "edit_view_pattern.h"
#include "gui_custom/rsskin.h"


namespace ReShaked {



/* Helpers */
static const char* note_str[12] = {
		"C-",
		"C#",
		"D-",
		"D#",
		"E-",
		"F-",

		"F#",
		"G-",
		"G#",
		"A-",
		"A#",
		"B-"
};



int get_note_display_column_pos(int p_column_w,int p_note) {

	int note_w=p_note-24;
	if (note_w<0)
		note_w=0;

				// draw note
	note_w=note_w*p_column_w/72;
	if  (note_w>=p_column_w)
		note_w=p_column_w;

	return note_w;
}


/* end of helpers */

int EditViewPattern::get_font_height() {
	
	return get_painter()->get_font_height(font(FONT_PATTERN));
}
int EditViewPattern::get_font_width() {
	
	return get_painter()->get_font_char_width(font(FONT_PATTERN),'X'); //monospace luckily	
}

int EditViewPattern::get_row_size() {
	
	
	return get_row_height();
}

void EditViewPattern::paint_note_event( Painter& p, int p_row, Track_Pattern::NoteListElement & p_note, bool p_repeat ) {

	
	FontID f=font(FONT_PATTERN);
	
	Color font_col=color(COLOR_PATTERN_FONT);
	Color font_col_repeat=color(COLOR_PATTERN_FONT_REPEAT);
	Color font_col_vol=color(COLOR_PATTERN_FONT_VOLUME);
	Color font_col_nofit=color(COLOR_PATTERN_FONT_NOFIT);

	
	int rowsize=get_row_height();
	int fontofs=(rowsize-get_font_height())/2;
	int textofs=p_row*rowsize+fontofs+p.get_font_ascent(font(FONT_PATTERN));
	int volofs=textofs+p.get_font_descent(font(FONT_PATTERN))+constant(C_EDITOR_ROW_MARGIN);
	int notewidth=get_font_width()*3;
	int columnwidth=get_font_width()*4;

	int fontxofs=get_font_width()+p_note.pos.column*columnwidth;

	Tick row_ticks=editor->get_cursor().get_snap_tick_size();
	Tick note_ofs=p_note.pos.tick-editor->get_cursor().get_snapped_window_tick_pos(p_row);
	
	
	if (note_ofs!=0) {
		int pix_ofs=(int)note_ofs*get_row_size()/(int)row_ticks;
		textofs+=pix_ofs;
		volofs+=pix_ofs;
		font_col=font_col_nofit;
		font_col_vol=font_col_nofit;
		font_col_repeat=font_col_vol;
		
	} 


	static char buf[4]={0,0,0,0};
	if (p_note.note.is_note()) {

		switch( editor->get_pattern_note_edit_mode() ) {

		case EditorData::MODE_NOTE: {


			int note=p_note.note.note % 12;
			int octave = p_note.note.note / 12;

			buf[0]=note_str[note][0];
			buf[1]=note_str[note][1];
			buf[2]=(octave>9)?('A'+(char)(octave-10)):('0'+octave);
			p.draw_text(f,Point(fontxofs,textofs),buf,font_col);
			// volume

			p.draw_fill_rect(Point(fontxofs,volofs),Size(notewidth,constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_VOLUME_BG));
			int vol_w=(p_note.note.volume*notewidth)/Track_Pattern::Note::MAX_VOLUME;
			p.draw_fill_rect(Point(fontxofs,volofs),Size(vol_w,constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_VOLUME_BAR));

		} break;
		case EditorData::MODE_VOLUME: {


			buf[0]=' ';
			buf[1]='0'+p_note.note.volume/10;
			buf[2]='0'+p_note.note.volume%10;
			p.draw_text(f,Point(fontxofs,textofs),buf,font_col_vol);

			p.draw_fill_rect(Point(fontxofs,volofs),Size(notewidth,constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_NOTE_BG));
//check this color later
			int note_w=get_note_display_column_pos(notewidth,p_note.note.note);

			p.draw_fill_rect(Point(fontxofs+note_w,volofs),Size(2,constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_NOTE_BAR));
		} break;


		}
		
		Color col=color(COLOR_PATTERN_SUBBEAT_LINE);
		p.draw_fill_rect(Point(fontxofs,volofs),Size(1,constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)),col);
		p.draw_fill_rect(Point(size.width-fontxofs,volofs),Size(1,constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)),col);
		
		
	} else if (p_note.note.is_note_off()) {

		buf[0]='*';
		buf[1]='*';
		buf[2]='*';
		p.draw_text(f,Point(fontxofs,textofs),buf,font_col);

	}

	// draw volume

	if (p_note.note.is_note()) {
	}



}


void EditViewPattern::paint_multiple_nonvisible_events( Painter& p, int p_row, const Track_Pattern::NoteList& p_list) {

	
	
	int font_width=get_font_width();
	int rowsize=get_row_size();
	int fontofs=(rowsize-get_font_height())/2;
	int eventofs=p_row*rowsize+fontofs;
	int notewidth=get_font_width()*3;
	int columnwidth=get_font_width()*4;

	Track_Pattern::NoteList::const_iterator I=p_list.begin();

	int howmany=p_list.size();
	int maxevents=(rowsize-4)/2;
	if (howmany>maxevents)
		howmany=maxevents;
	int maxsize=(rowsize-4)-(howmany-1);

	for (int i=0;i<howmany;I++,i++) {

		Track_Pattern::NoteListElement note = *I;
		int xofs=font_width+note.pos.column*columnwidth;

		int from=i*maxsize/howmany;
		int to=((i+1)*maxsize/howmany);
		int eheight = to-from;
		if (eheight<=0) eheight=1;
		from+=i;

		//printf("%i - from %i, height %i\n",i,from,eheight);
		//Color col=color(COLOR_PATTERN_FONT);


		if (note.note.is_note()) {

			switch( editor->get_pattern_note_edit_mode() ) {
				case EditorData::MODE_NOTE: {
					Color bg=Color(COLOR_PATTERN_NOTE_BG);
					Color fg=color(COLOR_PATTERN_FONT);
	
					int note_w=get_note_display_column_pos(notewidth,note.note.note);
	
					p.draw_fill_rect(Point(xofs,eventofs+from),Size(notewidth,eheight),bg);
					p.draw_fill_rect(Point(xofs+note_w,eventofs+from),Size(constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT),eheight),fg);
				} break;

				case EditorData::MODE_VOLUME: {

					Color bg=Color(COLOR_PATTERN_VOLUME_BG);
					Color fg=color(COLOR_PATTERN_FONT_VOLUME);
	
					int vol_w=(note.note.volume*notewidth)/Track_Pattern::Note::MAX_VOLUME;
					p.draw_fill_rect(Point(xofs,eventofs+from),Size(notewidth,eheight),bg);
					p.draw_fill_rect(Point(xofs,eventofs+from),Size(vol_w,eheight),fg);
				} break;
			}
		} else if (note.note.is_note_off()) {
			Color col = color(COLOR_PATTERN_SUBBEAT_LINE); 
			p.draw_fill_rect(Point(xofs,eventofs+from),Size(notewidth,eheight),col);
		} else {

			//printf("WTF!!\n");
		}
	}
}


void EditViewPattern::paint_multiple_note_events( Painter& p, int p_row , const Track_Pattern::NoteList& p_list , bool p_repeat) {


	/* separate in columns */
	std::vector<Track_Pattern::NoteList> notes_in_column( track->get_visible_columns() );


	Track_Pattern::NoteList::const_iterator I=p_list.begin();
	for (;I!=p_list.end();I++) {
		int col=I->pos.column;
		ERR_CONTINUE( col < 0 );
		ERR_CONTINUE( col >= notes_in_column.size() );
		notes_in_column[col].push_back(*I);
	}

	/* now draw columns */

	for (int i=0;i<notes_in_column.size();i++) {

		if (notes_in_column[i].empty())
			continue;

		if (notes_in_column[i].size()==1) {
			

			paint_note_event(p,p_row,*notes_in_column[i].begin(),p_repeat);
			
			
		} else
			paint_multiple_nonvisible_events(p,p_row,notes_in_column[i]);

	}

}

void EditViewPattern::paint_cursor(Painter &p,int p_row) {


	int rowsize=get_row_height();
	int fontofs=(rowsize-get_font_height())/2;
	int textofs=p_row*rowsize+fontofs;
	int columnwidth=get_font_width()*4;

	int column=editor->get_pattern_edit_column();
	int field=editor->get_pattern_edit_field();

	int xofs=column*columnwidth+get_font_width();

	//xofs+=VisualSettings::get_singleton()->get_pattern_cursor_offset().x;
	//textofs+=VisualSettings::get_singleton()->get_pattern_cursor_offset().y;

	switch( editor->get_pattern_note_edit_mode() ) {

		case EditorData::MODE_NOTE: {

			xofs+=2*field*get_font_width();
		} break;
		case EditorData::MODE_VOLUME: {

			xofs+=get_font_width()+field*get_font_width();
		} break;
	}

	p.draw_style_box( stylebox( SB_EDIT_VIEW_CURSOR ), Point(xofs,textofs), Size( get_font_width(), get_font_height() ) );
	
}

void EditViewPattern::paint_selection(Painter&p,int p_clip_from,int p_clip_to) {
	
	if (!editor->is_selection_active())
		return;
	
	int sel_margin=constant(C_PATTERN_SELECTION_MARGIN);
	int current_bl=editor->find_blocklist(track);
	int font_width=get_font_width();
	
	if ( editor->get_selection_begin_blocklist() <= current_bl && editor->get_selection_end_blocklist() >= current_bl &&
		    editor->get_selection_begin_row() <= (editor->get_cursor().get_window_size()+editor->get_cursor().get_window_offset()+1) && (editor->get_selection_end_row()+1)>=editor->get_cursor().get_window_offset() ) {
		
		/* SELECTION VISIBLE */
		
		int from_y=editor->get_selection_begin_row()-editor->get_cursor().get_window_offset();
		
		int to_y=(editor->get_selection_end_row()+1)-editor->get_cursor().get_window_offset();
		
		from_y*=get_row_size();
		to_y*=get_row_size();
		
		if (from_y<0)
			from_y=0;
		if (to_y>size.height)
			to_y=size.height;

		//no point painting?
		if (from_y>p_clip_to || to_y<p_clip_from)
			return;
		
		if (from_y<p_clip_from)
			from_y=p_clip_from;
		
		if (to_y>p_clip_to)
			to_y=p_clip_to;
		
		int from_x=sel_margin;
		int to_x=size.width-sel_margin*2;
		
		if (editor->get_selection_begin_blocklist()==current_bl) {
			
			int col_from=editor->get_selection_begin_column();
			from_x=sel_margin+4*font_width*col_from;
		}
		
		if (editor->get_selection_end_blocklist()==current_bl) {
		
			int col_to=editor->get_selection_end_column();
			to_x=(font_width-sel_margin)+4*font_width*(col_to+1);
	
		}
		//printf("drawing from %i to %i\n",p_clip_from,p_clip_to);
		p.draw_fill_rect(Point(from_x,from_y),Size(to_x-from_x,to_y-from_y),color(COLOR_PATTERN_SELECTION));
	}
		    
	
}


void EditViewPattern::paint_frames(Painter& p) {

	int begin_pos=-1;
	int old_block_idx=-1;
	int visible_rows=editor->get_cursor().get_window_size();
	StyleBox sb=stylebox(SB_PATTERN);
	StyleBox sb_shared=stylebox(SB_PATTERN_SHARED);
	
	int last_block_idx=-1;
	for (int i=0;i<(visible_rows+3);i++) {
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(i);
		int block_idx=track->get_block_idx_at_pos(tick);
		if (block_idx!=last_block_idx && block_idx!=-1)
			last_block_idx=block_idx;
			

		bool begin=(block_idx!=-1 && track->get_block_pos(block_idx)==tick);

		if (block_idx!=-1 && i==0 && !begin) {

			begin_pos=-1;
		}

		if (i==0)
			old_block_idx=block_idx;

		bool end=((old_block_idx!=block_idx || i==(visible_rows+2)) && old_block_idx!=-1);
		
		old_block_idx=block_idx;

		if (end) {

			int row_size=get_row_height();
			int begin_h=begin_pos*row_size;
			int len_h=(i-begin_pos)*row_size;

			
			p.draw_style_box(sb, Point(0,begin_h),Size(size.width,len_h));
			
			if (last_block_idx!=-1 && track->get_block(last_block_idx)->is_shared()) {

				p.draw_style_box(sb_shared, Point(0,begin_h),Size(size.width,len_h));
			}
		

		}

		if (begin)
			begin_pos=i;

	}

	
	for (int i=0;i<(track->get_visible_columns()-1);i++ ) {
		
		int font_width=get_font_width();
		int ofs=font_width/2+font_width*4*(i+1);	
		
		p.draw_fill_rect(Point(ofs,0),Size(1,size.height),color(COLOR_PATTERN_SUBBEAT_LINE));
	}
	
}

void EditViewPattern::paint_row_lines(Painter &p) {

	int visible_rows=editor->get_cursor().get_window_size()+1;
	int row_size=get_row_height();

	int font_w=get_font_width();
	
	for (int i=0;i<visible_rows;i++) {
		
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(i);

		int from_x; 
		int width_x; 
			
		if (track->is_pos_editable( editor->get_cursor().get_snapped_window_tick_pos(i))) {
			from_x = font_w;
			width_x = size.width-from_x*2;
		} else {
			from_x=0;
			width_x=size.width;
				
		}		
		if ( (tick % TICKS_PER_BEAT)==0 ) {//beat
			
			int block=track->get_prev_block_from_idx( tick );
			bool paint=true;
			if ( block!=-1) {
				
				if (track->get_block_pos(block)==tick)
					paint=false;
				
				if (track->get_block_pos(block)+track->get_block(block)->get_length()==tick)
					paint=false;
				
			}
			
			
		
			if (song->get_bar_map().get_bar_beat(tick/TICKS_PER_BEAT)==0)  {
				p.draw_fill_rect(Point(from_x,i*row_size),Size(width_x,1),color(COLOR_PATTERN_BAR_LINE));
			} else  if (paint) {
				p.draw_fill_rect(Point(from_x,i*row_size),Size(width_x,1),color(COLOR_PATTERN_BEAT_LINE));
				
			}

		} else
			p.draw_fill_rect(Point(from_x,i*row_size),Size(width_x,1),color(COLOR_PATTERN_SUBBEAT_LINE));

		
		//p.drawRect(0,i*row_size,size.width,0);


	}

}


bool EditViewPattern::can_scroll() {
	
	return !has_focus();
}

void EditViewPattern::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	editor->get_cursor().set_window_size( size.height / get_row_height() );
		
	Painter &p=*get_painter();

	p.set_clip_rect(false,p_exposed);
	//p.set_clip_rect(true,p_exposed);
	
	p.draw_fill_rect(Point(0,0),size,color(COLOR_PATTERN_BG));

	paint_frames(p);

	paint_selection(p,p_exposed.pos.y,p_exposed.pos.y+p_exposed.size.height);
	
	int visible_rows=editor->get_cursor().get_window_size();

	for (int i=0;i<visible_rows;i++) {

		Tick from=editor->get_cursor().get_snapped_window_tick_pos(i);
		Tick to=editor->get_cursor().get_snapped_window_tick_pos(i+1)-1;
		Tick adjust=0;

		/* Calculate Repeat, if exists */
		
		bool repeat=false;
		
		int blk_idx = track->get_block_idx_at_pos( from );
		int prev=track->get_prev_block_from_idx( from );
		
		if (blk_idx<0 && prev>=0 && track->get_block( prev )->is_repeat_active())
			repeat=true;
		//printf("row %i, blkidx %i , prev %i, repeat %i\n",i,blk_idx,prev,repeat);

		/* If repeat, adjust from and to */
		
		if (repeat) {
			
			Tick block_from=track->get_block_pos( prev );
			Tick block_len=track->get_block( prev )->get_length();
			
			Tick new_tick_from=block_from+(from-block_from)%block_len;
			
			adjust=from-new_tick_from;
			
			
			to=new_tick_from+(to-from);
			from=new_tick_from;
			

		}
		
		Track_Pattern::NoteList nl = track->get_notes_in_range( from, to );

		/* Then if repeat, adjust positions */
		if (repeat) {
			
			foreach( I, nl ) {
				
				I->pos.tick+=adjust;

			}
		}
		
		if (!nl.empty()) {

			paint_multiple_note_events( p,i , nl, repeat );
		}

		//paint_cursor(p,i);

	}

	paint_row_lines(p);




	if ( has_focus() && editor->get_track_blocklist( editor->find_track_idx( track) ) == editor->get_current_blocklist() &&
		    editor->get_cursor().get_pos()>=editor->get_cursor().get_window_offset() &&
		    editor->get_cursor().get_pos()<(editor->get_cursor().get_window_offset()+visible_rows)) {
				/* cursor is here */

		paint_cursor( p, editor->get_cursor().get_pos()-editor->get_cursor().get_window_offset() );

	}
	

	p.set_clip_rect(false);	
	

}

void EditViewPattern::get_row_column_and_field(Point p_pos,int *p_row,int *p_column,int *p_field) {
	
	int rowsize=get_row_height();
	int columnwidth=get_font_width()*4;

	int click_x=p_pos.x;
	if (editor->get_pattern_note_edit_mode()==EditorData::MODE_NOTE)
		click_x+=get_font_width()/2; //makes clicking easier
	click_x-=get_font_width();
	if (click_x<0)
		click_x=0;

	*p_row=p_pos.y/get_row_height();
	*p_column= click_x / columnwidth;
	int col_x=click_x % columnwidth;
	*p_field=col_x*2/columnwidth;
	
	if (*p_column>=track->get_visible_columns()) {
		
		*p_column=track->get_visible_columns()-1;
		*p_field=1;
	}
	
}

void EditViewPattern::get_pos_at_pointer(Point p_pointer, int *p_blocklist,int *p_column, int *p_row) {
	
	
	int field;
	get_row_column_and_field( p_pointer, p_row, p_column, &field );
	*p_row+=editor->get_cursor().get_window_offset();
	*p_blocklist=editor->find_blocklist( track );
	
}

void EditViewPattern::focus_enter() {

	
	editor->set_current_blocklist( editor->get_track_blocklist( editor->find_track_idx( track) ) );

}

void EditViewPattern::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	pre_parse_mouse_button_event(p_pos,p_button,p_press,p_modifier_mask);
	
	if (p_press) {
		int row;
		int column;
		int field;
		
		get_row_column_and_field(p_pos,&row,&column,&field);
	
		editor->set_pattern_edit_field( field );
		editor->set_pattern_edit_column( column );
		editor->get_cursor().set_pos( editor->get_cursor().get_window_offset() + row );
		update();
		
		mouse_selection_begin( p_pos );
	} else {
		
		mouse_selection_end();
	}
}

void EditViewPattern::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {

	mouse_selection_update_check();
}

bool EditViewPattern::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	

	int key_value=p_scan_code;

	if (key_value!=KEY_TAB) { //ruins things otherwise
	
		key_value|=p_modifier_mask;
	}
	
	if (p_press && editor->pattern_edit_key_press(key_value)) {
		
		update();		
		return true;
	}
	
		
	return false;
}


Size EditViewPattern::get_minimum_size_internal() {
	
	int cols=track->get_visible_columns();
	
	return Size( (2+3*cols+(cols-1))*get_font_width(), 0 );
	
}

EditViewPattern::EditViewPattern(Editor *p_editor, int p_track) : EditViewBlockList(p_editor) {

	editor = p_editor;
	song=editor->get_song();
	track=dynamic_cast<Track_Pattern*>(song->get_track(p_track));

	ERR_FAIL_COND(track==NULL);

	set_bg_on_updates(false);

	set_focus_mode(FOCUS_CLICK);
	
}


EditViewPattern::~EditViewPattern()
{
}


}
