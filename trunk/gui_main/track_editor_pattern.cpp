//
// C++ Implementation: track_editor_pattern
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor_pattern.h"
#include "gui_common/common_skin.h"


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

int TrackEditorPattern::get_font_height() {
	
	return get_painter()->get_font_height(font(FONT_TRACK_EDITOR));
}
int TrackEditorPattern::get_font_width() {
	
	return get_painter()->get_font_char_width(font(FONT_TRACK_EDITOR),'X'); //monospace luckily	
}

int TrackEditorPattern::get_row_size() {
	
	
	return get_row_height();
}

void TrackEditorPattern::paint_note_event( GUI::Painter& p, int p_row, NoteListElement & p_note, bool p_repeat ) {

	
	GUI::FontID f=font(FONT_TRACK_EDITOR);
	
	GUI::Color font_col=color(COLOR_PATTERN_FONT);
	GUI::Color font_col_repeat=color(COLOR_PATTERN_FONT_REPEAT);
	GUI::Color font_col_vol=color(COLOR_PATTERN_FONT_VOLUME);
	GUI::Color font_col_nofit=color(COLOR_PATTERN_FONT_NOFIT);

	
	int rowsize=get_row_height();
	int fontofs=constant(C_TRACK_EDITOR_ROW_MARGIN);
	int textofs=p_row*rowsize+fontofs+p.get_font_ascent(font(FONT_TRACK_EDITOR));
	int volofs=p_row*rowsize+fontofs+p.get_font_height( font(FONT_TRACK_EDITOR) ) + constant(C_TRACK_EDITOR_VOL_NOTE_BAR_SEPAATION);
	int notewidth=get_font_width()*3;
	int columnwidth=get_font_width()*4;

	int fontxofs=get_font_width()+p_note.pos.column*columnwidth;

	
	Tick row_ticks=Editor::get_singleton()->get_ticks_per_row();
	Tick note_ofs=p_note.pos.tick; //p_row*row_ticksp_note.pos.tick-Editor::get_singleton()->get_row_ticks( Editor::get_singleton()->get_window_offset() ); 
	
	
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

		switch( Editor::get_singleton()->get_track_edit_mode() ) {

		case Editor::EDIT_MODE_FRONT: { // note


			int note=p_note.note.note % 12;
			int octave = p_note.note.note / 12;

			buf[0]=note_str[note][0];
			buf[1]=note_str[note][1];
			buf[2]=(octave>9)?('A'+(char)(octave-10)):('0'+octave);
			p.draw_text(f,GUI::Point(fontxofs,textofs),buf,font_col);
			// volume

			p.draw_fill_rect(GUI::Point(fontxofs,volofs),GUI::Size(notewidth,constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_VOLUME_BG));
			int vol_w=(p_note.note.volume*notewidth)/PatternTrack::Note::MAX_VOLUME;
			p.draw_fill_rect(GUI::Point(fontxofs,volofs),GUI::Size(vol_w,constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_VOLUME_BAR));

		} break;
		case Editor::EDIT_MODE_BACK: { // volume


			buf[0]=' ';
			buf[1]='0'+p_note.note.volume/10;
			buf[2]='0'+p_note.note.volume%10;
			p.draw_text(f,GUI::Point(fontxofs,textofs),buf,font_col_vol);

			p.draw_fill_rect(GUI::Point(fontxofs,volofs),GUI::Size(notewidth,constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_NOTE_BG));
//check this color later
			int note_w=get_note_display_column_pos(notewidth,p_note.note.note);

			p.draw_fill_rect(GUI::Point(fontxofs+note_w,volofs),GUI::Size(2,constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)),color(COLOR_PATTERN_NOTE_BAR));
		} break;


		}
		
		GUI::Color col=color(COLOR_PATTERN_SUBBEAT_LINE);
		p.draw_fill_rect(GUI::Point(fontxofs,volofs),GUI::Size(1,constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)),col);
		p.draw_fill_rect(GUI::Point(size.width-fontxofs,volofs),GUI::Size(1,constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)),col);
		
		
	} else if (p_note.note.is_note_off()) {

		buf[0]='*';
		buf[1]='*';
		buf[2]='*';
		p.draw_text(f,GUI::Point(fontxofs,textofs),buf,font_col);

	}

	// draw volume

	if (p_note.note.is_note()) {
	}



}


void TrackEditorPattern::paint_multiple_nonvisible_events( GUI::Painter& p, int p_row, const NoteList& p_list) {

	
	
	int font_width=get_font_width();
	int rowsize=get_row_size();
	int fontofs=(rowsize-get_font_height())/2;
	int eventofs=p_row*rowsize+fontofs;
	int notewidth=get_font_width()*3;
	int columnwidth=get_font_width()*4;

	NoteList::const_iterator I=p_list.begin();

	int howmany=p_list.size();
	int maxevents=(rowsize-4)/2;
	if (howmany>maxevents)
		howmany=maxevents;
	int maxsize=(rowsize-4)-(howmany-1);

	for (int i=0;i<howmany;I++,i++) {

		NoteListElement note = *I;
		int xofs=font_width+note.pos.column*columnwidth;

		int from=i*maxsize/howmany;
		int to=((i+1)*maxsize/howmany);
		int eheight = to-from;
		if (eheight<=0) eheight=1;
		from+=i;

		//printf("%i - from %i, height %i\n",i,from,eheight);
		//GUI::Color col=color(COLOR_PATTERN_FONT);


		if (note.note.is_note()) {

			switch( Editor::get_singleton()->get_track_edit_mode() ) {
				case Editor::EDIT_MODE_FRONT: {
					GUI::Color bg=GUI::Color(COLOR_PATTERN_NOTE_BG);
					GUI::Color fg=color(COLOR_PATTERN_FONT);
	
					int note_w=get_note_display_column_pos(notewidth,note.note.note);
	
					p.draw_fill_rect(GUI::Point(xofs,eventofs+from),GUI::Size(notewidth,eheight),bg);
					p.draw_fill_rect(GUI::Point(xofs+note_w,eventofs+from),GUI::Size(constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT),eheight),fg);
				} break;

				case Editor::EDIT_MODE_BACK: {

					GUI::Color bg=GUI::Color(COLOR_PATTERN_VOLUME_BG);
					GUI::Color fg=color(COLOR_PATTERN_FONT_VOLUME);
	
					int vol_w=(note.note.volume*notewidth)/PatternTrack::Note::MAX_VOLUME;
					p.draw_fill_rect(GUI::Point(xofs,eventofs+from),GUI::Size(notewidth,eheight),bg);
					p.draw_fill_rect(GUI::Point(xofs,eventofs+from),GUI::Size(vol_w,eheight),fg);
				} break;
			}
		} else if (note.note.is_note_off()) {
			GUI::Color col = color(COLOR_PATTERN_SUBBEAT_LINE); 
			p.draw_fill_rect(GUI::Point(xofs,eventofs+from),GUI::Size(notewidth,eheight),col);
		} else {

			//printf("WTF!!\n");
		}
	}
}


void TrackEditorPattern::paint_multiple_note_events( GUI::Painter& p, int p_row , const NoteList& p_list , bool p_repeat) {


	/* separate in columns */
	std::vector<NoteList> notes_in_column( track->get_visible_columns() );


	NoteList::const_iterator I=p_list.begin();
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

void TrackEditorPattern::paint_cursor(GUI::Painter &p,int p_row) {


	int rowsize=get_row_height();
	int fontofs=(rowsize-get_font_height())/2;
	int textofs=p_row*rowsize+fontofs;
	int columnwidth=get_font_width()*4;

	int column=Editor::get_singleton()->get_cursor_col();
	int field=Editor::get_singleton()->get_cursor_field();

	int xofs=column*columnwidth+get_font_width();

	//xofs+=VisualSettings::get_singleton()->get_pattern_cursor_offset().x;
	//textofs+=VisualSettings::get_singleton()->get_pattern_cursor_offset().y;

	switch( Editor::get_singleton()->get_track_edit_mode() ) {

		case Editor::EDIT_MODE_FRONT: {

			xofs+=2*field*get_font_width();
		} break;
		case Editor::EDIT_MODE_BACK: {

			xofs+=get_font_width()+field*get_font_width();
		} break;
	}

	p.draw_stylebox( stylebox( SB_TRACK_EDITOR_CURSOR ), GUI::Point(xofs,textofs), GUI::Size( get_font_width(), get_font_height() ) );
	
}

void TrackEditorPattern::paint_selection(GUI::Painter&p,int p_clip_from,int p_clip_to) {
	
	if (!Editor::get_singleton()->is_selection_active())
		return;
	
	int sel_margin=constant(C_TRACK_EDITOR_SELECTION_MARGIN);
	int current_track=Editor::get_singleton()->get_song()->find_track_pos(track);
	int font_width=get_font_width();
	
	if ( Editor::get_singleton()->get_selection_begin_track() <= current_track && Editor::get_singleton()->get_selection_end_track() >= current_track &&
		    Editor::get_singleton()->get_selection_begin_row() <= (Editor::get_singleton()->get_window_rows()+Editor::get_singleton()->get_window_offset()+1) && (Editor::get_singleton()->get_selection_end_row()+1)>=Editor::get_singleton()->get_window_offset() ) {
		
		/* SELECTION VISIBLE */
		
		int from_y=Editor::get_singleton()->get_selection_begin_row()-Editor::get_singleton()->get_window_offset();
		
		int to_y=(Editor::get_singleton()->get_selection_end_row()+1)-Editor::get_singleton()->get_window_offset();
		
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
		
		if (Editor::get_singleton()->get_selection_begin_track()==current_track) {
			
			int col_from=Editor::get_singleton()->get_selection_begin_column();
			from_x=sel_margin+4*font_width*col_from;
		}
		
		if (Editor::get_singleton()->get_selection_end_track()==current_track) {
		
			int col_to=Editor::get_singleton()->get_selection_end_column();
			to_x=(font_width-sel_margin)+4*font_width*(col_to+1);
	
		}
		//printf("drawing from %i to %i\n",p_clip_from,p_clip_to);
		p.draw_fill_rect(GUI::Point(from_x,from_y),GUI::Size(to_x-from_x,to_y-from_y),color(COLOR_PATTERN_SELECTION));
	}
		    
	
}


void TrackEditorPattern::paint_frames(GUI::Painter& p) {

	int begin_pos=-1;
	int old_block_idx=-1;
	int visible_rows=Editor::get_singleton()->get_window_rows();
	GUI::StyleBox sb=stylebox(SB_PATTERN_BLOCK);
	GUI::StyleBox sb_shared=stylebox(SB_PATTERN_BLOCK_SHARED);
	
	int last_block_idx=-1;
	for (int i=0;i<(visible_rows+3);i++) {
		Tick tick=Editor::get_singleton()->get_row_ticks( Editor::get_singleton()->get_window_offset() + i );
		int block_idx=track->get_block_at_pos(tick);
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

			
			p.draw_stylebox(sb, GUI::Point(0,begin_h),GUI::Size(size.width,len_h));
			
			if (last_block_idx!=-1 && track->get_block(last_block_idx)->get_refcount()>1) {

				p.draw_stylebox(sb_shared, GUI::Point(0,begin_h),GUI::Size(size.width,len_h));
			}
		

		}

		if (begin)
			begin_pos=i;

	}

	
	for (int i=0;i<(track->get_visible_columns()-1);i++ ) {
		
		int font_width=get_font_width();
		int ofs=font_width/2+font_width*4*(i+1);	
		
		p.draw_fill_rect(GUI::Point(ofs,0),GUI::Size(1,size.height),color(COLOR_PATTERN_SUBBEAT_LINE));
	}
	
}

void TrackEditorPattern::paint_row_lines(GUI::Painter &p) {

	int visible_rows=Editor::get_singleton()->get_window_rows()+1;
	int row_size=get_row_height();

	int font_w=get_font_width();
	
	
	for (int i=0;i<visible_rows;i++) {
		
		Tick tick=Editor::get_singleton()->get_row_ticks( Editor::get_singleton()->get_window_offset() + i );

		int from_x; 
		int width_x; 
			
		if (track->get_block_at_pos(Editor::get_singleton()->get_row_ticks( Editor::get_singleton()->get_window_offset()+i))>=0) {
			from_x = font_w;
			width_x = size.width-from_x*2;
		} else {
			from_x=0;
			width_x=size.width;
				
		}		
		if ( (tick % TICKS_PER_BEAT)==0 ) {//beat
			
			int block=track->find_block_at_pos( tick );
			bool paint=true;
			if ( block!=-1) {
				
				if (track->get_block_pos(block)==tick)
					paint=false;
				
				if (track->get_block_pos(block)+track->get_block(block)->get_length()==tick)
					paint=false;
				
			}
			
			
		
			if (song->get_bar_map().get_bar_beat(tick/TICKS_PER_BEAT)==0)  {
				p.draw_fill_rect(GUI::Point(from_x,i*row_size),GUI::Size(width_x,1),color(COLOR_PATTERN_BAR_LINE));
			} else  if (paint) {
				p.draw_fill_rect(GUI::Point(from_x,i*row_size),GUI::Size(width_x,1),color(COLOR_PATTERN_BEAT_LINE));
				
			}

		} else
			p.draw_fill_rect(GUI::Point(from_x,i*row_size),GUI::Size(width_x,1),color(COLOR_PATTERN_SUBBEAT_LINE));

		
		//p.drawGUI::Rect(0,i*row_size,size.width,0);


	}

}


bool TrackEditorPattern::can_scroll() {
	
	return !has_focus();
}

void TrackEditorPattern::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	Editor::get_singleton()->set_window_rows( p_size.height / get_row_height() );
		
	GUI::Painter &p=*get_painter();

//	p.set_clip_rect(false,p_exposed);
	//p.set_clip_rect(true,p_exposed);
	
	p.draw_fill_rect(GUI::Point(0,0),size,color(COLOR_PATTERN_BG));

	paint_frames(p);

	paint_selection(p,p_exposed.pos.y,p_exposed.pos.y+p_exposed.size.height);
	
	int visible_rows=Editor::get_singleton()->get_window_rows();

	for (int i=0;i<visible_rows;i++) {

		Tick from=Editor::get_singleton()->get_row_ticks( Editor::get_singleton()->get_window_offset() + i );
		Tick to=from+Editor::get_singleton()->get_ticks_per_row()-1;
		Tick adjust=0;

		/* Calculate Repeat, if exists */
		
		bool repeat=false;
		
		int blk_idx = track->get_block_at_pos( from );
		int prev=track->find_block_at_pos( from );
		
		if (blk_idx<0 && prev>=0 && track->get_block( prev )->is_repeat_enabled())
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
		
		
		NoteList nl;
		 
		{ 
			int from_block=track->find_block_at_pos(from);
			int to_block=track->find_block_at_pos(to);
			
			for (int b=from_block;b<=to_block;b++) {
			
				if (b<0)
					continue;
					
				PatternTrack::PatternBlock *block = static_cast<PatternTrack::PatternBlock*>(track->get_block(b));
				Tick block_pos = track->get_block_pos(b);
				Tick block_len = block->get_length();
				
				if (block_pos+block_len < from )
					continue;
					
				int note_from,note_to;
				//printf("tick %i-%i, notes %i-%i\n",from,to,note_from,note_to);
				
				if (block->get_notes_in_local_range(from-block_pos,to-block_pos,&note_from,&note_to)) {
				
					for (int n=note_from;n<=note_to;n++) {
					
						NoteListElement nle;
						nle.pos=block->get_note_pos(n);
						nle.pos.tick=nle.pos.tick+block_pos-from;
						nle.note=block->get_note(n);
						nl.push_back(nle);	
					}
				}
				
			}
		}

		/* Then if repeat, adjust positions */
		
		NoteList::iterator I=nl.begin();
		
		if (repeat) {
		
			for(;I!=nl.end();I++) {
				I->pos.tick+=adjust;
			}
		}
		
		if (!nl.empty()) {

			paint_multiple_note_events( p,i , nl, repeat );
		}

		//paint_cursor(p,i);

	}

	paint_row_lines(p);

//	printf("focus %i, track %i, row %i (%i-%i)\n",(int)has_focus(),(int)Editor::get_singleton()->get_cursor_track(),(int)Editor::get_singleton()->get_cursor_row(),(int)Editor::get_singleton()->get_window_offset(),(int)(Editor::get_singleton()->get_window_offset()+visible_rows));
	if ( has_focus() && Editor::get_singleton()->get_cursor_track()==song->find_track_pos(track) &&
		    Editor::get_singleton()->get_cursor_row()>=Editor::get_singleton()->get_window_offset() &&
		    Editor::get_singleton()->get_cursor_row()<(Editor::get_singleton()->get_window_offset()+visible_rows)) {
				/* cursor is here */

		
		paint_cursor( p, Editor::get_singleton()->get_cursor_row()-Editor::get_singleton()->get_window_offset() );

	}
	



}

void TrackEditorPattern::get_row_column_and_field(GUI::Point p_pos,int *p_row,int *p_column,int *p_field) {
	
	int rowsize=get_row_height();
	int columnwidth=get_font_width()*4;

	int click_x=p_pos.x;
	if (Editor::get_singleton()->get_track_edit_mode()==Editor::EDIT_MODE_FRONT)
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

void TrackEditorPattern::get_column_and_row_at_pos(const GUI::Point& p_pos, int *p_column, int *p_row) {	
	
	int field;
	get_row_column_and_field( p_pos, p_row, p_column, &field );
	*p_row+=Editor::get_singleton()->get_window_offset();
	if (*p_row<0)
		*p_row=0;
	
}

void TrackEditorPattern::focus_enter() {

	
	Editor::get_singleton()->set_cursor_track( song->find_track_pos( track ) );

}

void TrackEditorPattern::mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	pre_parse_mouse_button_event(p_pos,p_button,p_press,p_modifier_mask);
	
	if (p_press) {
		int row;
		int column;
		int field;
		
		get_row_column_and_field(p_pos,&row,&column,&field);
	
		
		Editor::get_singleton()->set_cursor_track( song->find_track_pos( track ) );
		Editor::get_singleton()->set_cursor_field( field );
		Editor::get_singleton()->set_cursor_col( column );
		Editor::get_singleton()->set_cursor_row( Editor::get_singleton()->get_window_offset() + row );
		
		update();
		
		mouse_selection_begin( p_pos );
	} else {
		
		mouse_selection_end();
	}
}

void TrackEditorPattern::mouse_motion(const GUI::Point& p_pos, const GUI::Point& p_rel, int p_button_mask) {

	mouse_selection_update_check(p_pos);
}

bool TrackEditorPattern::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	

	int key_value=p_scan_code;


	key_value|=p_modifier_mask; // bind them to be recognized

	
	if (p_press && Editor::get_singleton()->track_editor_keypress(key_value)) {
		
		update();		
		return true;
	}
	
		
	return false;
}


GUI::Size TrackEditorPattern::get_minimum_size_internal() {
	
	int cols=track->get_visible_columns();
	
	return GUI::Size( (2+3*cols+(cols-1))*get_font_width(), 0 );
	
}

TrackEditorPattern::TrackEditorPattern(PatternTrack *p_track) : TrackEditor(p_track) {

	song = Editor::get_singleton()->get_song();
	track = p_track;

	set_bg_on_updates(false);

	set_focus_mode(GUI::FOCUS_CLICK);
	
}


TrackEditorPattern::~TrackEditorPattern()
{
}
