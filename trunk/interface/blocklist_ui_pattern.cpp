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
#include "interface/visual_settings.h"
#include <Qt/qevent.h>

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

int BlockListUI_Pattern::get_row_size() {
	
	return VisualSettings::get_singleton()->get_editing_row_height();
}

void BlockListUI_Pattern::paint_note_event( QPainter& p, int p_row, Track_Pattern::NoteListElement & p_note, bool p_repeat ) {

	PixmapFont * repeat_font=VisualSettings::get_singleton()->get_pattern_font_nofit();
	PixmapFont * font=p_repeat?repeat_font:VisualSettings::get_singleton()->get_pattern_font();
	PixmapFont * font_vol=p_repeat?repeat_font:VisualSettings::get_singleton()->get_pattern_vol_font();

	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int fontofs=(rowsize-font->get_height())/2;
	int textofs=p_row*rowsize+fontofs;
	int volofs=textofs+font->get_height()+1;
	int notewidth=font->get_width()*3;
	int columnwidth=font->get_width()*4;

	int fontxofs=font->get_width()+p_note.pos.column*columnwidth;

	Tick row_ticks=editor->get_cursor().get_snap_tick_size();
	Tick note_ofs=p_note.pos.tick-editor->get_cursor().get_snapped_window_tick_pos(p_row);
	
	
	if (note_ofs!=0) {
		int pix_ofs=(int)note_ofs*get_row_size()/(int)row_ticks;
		textofs+=pix_ofs;
		volofs+=pix_ofs;
		font=VisualSettings::get_singleton()->get_pattern_font_nofit();
		font_vol=VisualSettings::get_singleton()->get_pattern_font_nofit();
		
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
			font->render_string(p,fontxofs,textofs,buf);
			// volume

			p.fillRect(fontxofs,volofs,notewidth,2,QColor(0,0,0)); //check this color later
			QColor volcol=VisualSettings::get_singleton()->get_color( COLORLIST_PATTERN_EDIT_VOL );
			int vol_w=(p_note.note.volume*notewidth)/Track_Pattern::Note::MAX_VOLUME;
			p.fillRect(fontxofs,volofs,vol_w,2,volcol);

		} break;
		case EditorData::MODE_VOLUME: {


			buf[0]=' ';
			buf[1]='0'+p_note.note.volume/10;
			buf[2]='0'+p_note.note.volume%10;
			font_vol->render_string(p,fontxofs,textofs,buf);

			p.fillRect(fontxofs,volofs,notewidth,2,QColor(0,0,0)); //check this color later
			int note_w=get_note_display_column_pos(notewidth,p_note.note.note);
			QColor notecol=VisualSettings::get_singleton()->get_color( COLORLIST_PATTERN_EDIT_NOTE );
			p.fillRect(fontxofs+note_w,volofs,2,2,notecol);
		} break;


		}

		p.setPen(GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));
		p.drawLine(fontxofs,volofs,fontxofs,volofs+2);
		p.drawLine(width()-fontxofs,volofs,width()-fontxofs,volofs+2);
		
	} else if (p_note.note.is_note_off()) {

		buf[0]='*';
		buf[1]='*';
		buf[2]='*';
		font->render_string(p,fontxofs,textofs,buf);

	}

	// draw volume

	if (p_note.note.is_note()) {
	}



}


void BlockListUI_Pattern::paint_multiple_nonvisible_events( QPainter& p, int p_row, const Track_Pattern::NoteList& p_list) {

	PixmapFont * font=VisualSettings::get_singleton()->get_pattern_font();
	
	
	
	int font_width=font->get_width();
	int rowsize=get_row_size();
	int fontofs=(rowsize-font->get_height())/2;
	int eventofs=p_row*rowsize+fontofs;
	int notewidth=font->get_width()*3;
	int columnwidth=font->get_width()*4;

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
		QColor col=GET_QCOLOR(COLORLIST_PATTERN_EDIT_NOTE);



		if (note.note.is_note()) {

			switch( editor->get_pattern_note_edit_mode() ) {
				case EditorData::MODE_NOTE: {
					QColor bg=QColor(0,0,0);
					QColor fg=GET_QCOLOR(COLORLIST_PATTERN_EDIT_NOTE);
	
					int note_w=get_note_display_column_pos(notewidth,note.note.note);
	
					p.fillRect(xofs,eventofs+from,notewidth,eheight,bg);
					p.fillRect(xofs+note_w,eventofs+from,2,eheight,fg);
				} break;

				case EditorData::MODE_VOLUME: {

					QColor bg=QColor(0,0,0);
					QColor fg=GET_QCOLOR(COLORLIST_PATTERN_EDIT_VOL );
	
					int vol_w=(note.note.volume*notewidth)/Track_Pattern::Note::MAX_VOLUME;
					p.fillRect(xofs,eventofs+from,notewidth,eheight,bg);
					p.fillRect(xofs,eventofs+from,vol_w,eheight,fg);
				} break;
			}
		} else if (note.note.is_note_off()) {
			QColor col=GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE);
			p.fillRect(xofs,eventofs+from,notewidth,eheight,col);
		} else {

			//printf("WTF!!\n");
		}
	}
}


void BlockListUI_Pattern::paint_multiple_note_events( QPainter& p, int p_row , const Track_Pattern::NoteList& p_list , bool p_repeat) {


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

void BlockListUI_Pattern::paint_cursor(QPainter &p,int p_row) {

	PixmapFont * font=VisualSettings::get_singleton()->get_pattern_font();
	QPixmap cursor=VisualSettings::get_singleton()->get_pattern_cursor_pixmap();

	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int fontofs=(rowsize-font->get_height())/2;
	int textofs=p_row*rowsize+fontofs;
	int columnwidth=font->get_width()*4;

	int column=editor->get_pattern_edit_column();
	int field=editor->get_pattern_edit_field();

	int xofs=column*columnwidth+font->get_width();

	xofs+=VisualSettings::get_singleton()->get_pattern_cursor_offset().x();
	textofs+=VisualSettings::get_singleton()->get_pattern_cursor_offset().y();

	switch( editor->get_pattern_note_edit_mode() ) {

		case EditorData::MODE_NOTE: {

			xofs+=2*field*font->get_width();
		} break;
		case EditorData::MODE_VOLUME: {

			xofs+=font->get_width()+field*font->get_width();
		} break;
	}

	p.drawPixmap(xofs,textofs,cursor);
}

void BlockListUI_Pattern::paint_selection(QPainter&p,int p_clip_from,int p_clip_to) {
	
	if (!editor->is_selection_active())
		return;
	
	int sel_margin=VisualSettings::get_singleton()->get_selection_h_margin();
	int current_bl=editor->find_blocklist(track);
	int font_width=VisualSettings::get_singleton()->get_pattern_font()->get_width();
	
	if ( editor->get_selection_begin_blocklist() <= current_bl && editor->get_selection_end_blocklist() >= current_bl &&
		    editor->get_selection_begin_row() <= (editor->get_cursor().get_window_size()+editor->get_cursor().get_window_offset()+1) && (editor->get_selection_end_row()+1)>=editor->get_cursor().get_window_offset() ) {
		
		/* SELECTION VISIBLE */
		
		int from_y=editor->get_selection_begin_row()-editor->get_cursor().get_window_offset();
		
		int to_y=(editor->get_selection_end_row()+1)-editor->get_cursor().get_window_offset();
		
		from_y*=get_row_size();
		to_y*=get_row_size();
		
		if (from_y<0)
			from_y=0;
		if (to_y>height())
			to_y=height();

		//no point painting?
		if (from_y>p_clip_to || to_y<p_clip_from)
			return;
		
		if (from_y<p_clip_from)
			from_y=p_clip_from;
		
		if (to_y>p_clip_to)
			to_y=p_clip_to;
		
		int from_x=sel_margin;
		int to_x=width()-sel_margin*2;
		
		if (editor->get_selection_begin_blocklist()==current_bl) {
			
			int col_from=editor->get_selection_begin_column();
			from_x=sel_margin+4*font_width*col_from;
		}
		
		if (editor->get_selection_end_blocklist()==current_bl) {
		
			int col_to=editor->get_selection_end_column();
			to_x=(font_width-sel_margin)+4*font_width*(col_to+1);
	
		}
		//printf("drawing from %i to %i\n",p_clip_from,p_clip_to);
		p.fillRect(from_x,from_y,to_x-from_x,to_y-from_y,GET_QCOLOR(COLORLIST_EDITOR_SELECTION_PATTERN));
	}
		    
	
}


void BlockListUI_Pattern::paint_frames(QPainter& p) {

	int begin_pos=-1;
	int old_block_idx=-1;
	int visible_rows=editor->get_cursor().get_window_size();
	SkinBox *sb=VisualSettings::get_singleton()->get_skin_box(  SKINBOX_EDITING_PATTERN );
	SkinBox *sb_shared=VisualSettings::get_singleton()->get_skin_box(  SKINBOX_EDITING_PATTERN_SHARED );
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

			int row_size=VisualSettings::get_singleton()->get_editing_row_height();
			int begin_h=begin_pos*row_size;
			int len_h=(i-begin_pos)*row_size;

			if (last_block_idx!=-1 && track->get_block(last_block_idx)->is_shared())

				sb_shared->paint_into( p, 0,begin_h, width(), len_h );
			else
				sb->paint_into( p, 0,begin_h, width(), len_h );

		}

		if (begin)
			begin_pos=i;

	}

	p.setPen(GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));
	for (int i=0;i<(track->get_visible_columns()-1);i++ ) {
		
		int font_width=VisualSettings::get_singleton()->get_pattern_font()->get_width();
		int ofs=font_width/2+font_width*4*(i+1);	
		
		p.fillRect(ofs,0,1,height(),GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));
	}
	
}

void BlockListUI_Pattern::paint_row_lines(QPainter &p) {

	int visible_rows=editor->get_cursor().get_window_size()+1;
	int row_size=VisualSettings::get_singleton()->get_editing_row_height();

	int font_w=VisualSettings::get_singleton()->get_pattern_font()->get_width();
	
	for (int i=0;i<visible_rows;i++) {
		
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(i);

		int from_x; 
		int width_x; 
			
		if (track->is_pos_editable( editor->get_cursor().get_snapped_window_tick_pos(i))) {
			from_x = font_w;
			width_x = width()-from_x*2;
		} else {
			from_x=0;
			width_x=width();
				
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
				p.fillRect(from_x,i*row_size,width_x,1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BAR));
			} else  if (paint) {
				p.fillRect(from_x,i*row_size,width_x,1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BEAT_LINE));

				
				
			}

		} else
			p.fillRect(from_x,i*row_size,width_x,1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));

		
		//p.drawRect(0,i*row_size,width(),0);


	}

}


bool BlockListUI_Pattern::can_scroll() {
	
	return !hasFocus();
}

void BlockListUI_Pattern::paintEvent(QPaintEvent *e) {

	editor->get_cursor().set_window_size( height() / VisualSettings::get_singleton()->get_editing_row_height() );
		
	QPainter p(this);

	p.setClipping(true);
	p.setClipRect(e->rect());
	
	p.fillRect(0,0,width(),height(),QColor(0,0,0));

	paint_frames(p);

	paint_selection(p,e->rect().y(),e->rect().y()+e->rect().height());
	
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
			
			Tick new_tick_from= (from-block_from)%block_len;
			
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




	if ( hasFocus() && editor->get_track_blocklist( editor->find_track_idx( track) ) == editor->get_current_blocklist() &&
		    editor->get_cursor().get_pos()>=editor->get_cursor().get_window_offset() &&
		    editor->get_cursor().get_pos()<(editor->get_cursor().get_window_offset()+visible_rows)) {
				/* cursor is here */

		paint_cursor( p, editor->get_cursor().get_pos()-editor->get_cursor().get_window_offset() );

	}
	
	p.setClipping(false);
	

}

void BlockListUI_Pattern::get_row_column_and_field(QPoint p_pos,int *p_row,int *p_column,int *p_field) {
	
	PixmapFont * font=VisualSettings::get_singleton()->get_pattern_font();

	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int columnwidth=font->get_width()*4;

	int click_x=p_pos.x();
	if (editor->get_pattern_note_edit_mode()==EditorData::MODE_NOTE)
		click_x+=font->get_width()/2; //makes clicking easier
	click_x-=font->get_width();
	if (click_x<0)
		click_x=0;

	*p_row=p_pos.y()/VisualSettings::get_singleton()->get_editing_row_height();
	*p_column= click_x / columnwidth;
	int col_x=click_x % columnwidth;
	*p_field=col_x*2/columnwidth;
	
	if (*p_column>=track->get_visible_columns()) {
		
		*p_column=track->get_visible_columns()-1;
		*p_field=1;
	}
	
}

void BlockListUI_Pattern::get_pos_at_pointer(QPoint p_pointer, int *p_blocklist,int *p_column, int *p_row) {
	
	
	int field;
	get_row_column_and_field( p_pointer, p_row, p_column, &field );
	*p_row+=editor->get_cursor().get_window_offset();
	*p_blocklist=editor->find_blocklist( track );
	
}

void BlockListUI_Pattern::focusInEvent ( QFocusEvent * event ) {

	
	editor->set_current_blocklist( editor->get_track_blocklist( editor->find_track_idx( track) ) );
	QWidget::focusInEvent(event);
	
}

void BlockListUI_Pattern::mousePressEvent ( QMouseEvent * e ) {

	int row;
	int column;
	int field;
	
	get_row_column_and_field(e->pos(),&row,&column,&field);

	editor->set_pattern_edit_field( field );
	editor->set_pattern_edit_column( column );
	editor->get_cursor().set_pos( editor->get_cursor().get_window_offset() + row );
	update();
	
	mouse_selection_begin( e->pos() );

}

void BlockListUI_Pattern::mouseMoveEvent ( QMouseEvent * e ) {
	

	mouse_selection_update_check();
}
void BlockListUI_Pattern::mouseReleaseEvent ( QMouseEvent * e ) {
	
	mouse_selection_end();
	
}

bool BlockListUI_Pattern::event ( QEvent * ev )  {
	
	if (ev->type()==QEvent::KeyPress) {
		
		QKeyEvent * e=(QKeyEvent*)ev;
		int key_value=e->key();

		if (key_value!=Qt::Key_Backtab) { //ruins things otherwise
		
			bool alt=e->modifiers() & Qt::AltModifier;
			bool shift=e->modifiers() & Qt::ShiftModifier;
			bool control=e->modifiers() & Qt::ControlModifier;
		
			key_value|= (alt?Qt::ALT:0);
			key_value|= (shift?Qt::SHIFT:0);
			key_value|= (control?Qt::CTRL:0);
		}
		
		if (editor->pattern_edit_key_press(key_value)) {
			
			update();
			e->accept();
			return true;
		}
		
	}
	
	return QWidget::event(ev);
}
BlockListUI_Pattern::BlockListUI_Pattern(QWidget *p_parent,Editor *p_editor, int p_track) : BlockListUI_Base(p_parent,p_editor) {



	editor = p_editor;
	song=editor->get_song();
	track=dynamic_cast<Track_Pattern*>(song->get_track(p_track));

	ERR_FAIL_COND(track==NULL);

	setBackgroundRole(QPalette::NoRole);

	int fontwidth=VisualSettings::get_singleton()->get_pattern_font()->get_width();
	int columns=track->get_visible_columns();

	int min_chars_width=2+3*columns+( (columns>1)?(columns-1):0 );
	setFixedWidth( min_chars_width*fontwidth );
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);
	

}


BlockListUI_Pattern::~BlockListUI_Pattern()
{
}


}
