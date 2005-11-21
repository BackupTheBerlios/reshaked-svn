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
#include "ui_blocks/visual_settings.h"
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

void BlockListUI_Pattern::paint_note_event( QPainter& p, int p_row, Track_Pattern::NoteListElement & p_note ) {

	PixmapFont * font=VisualSettings::get_singleton()->get_pattern_font();
	PixmapFont * font_vol=VisualSettings::get_singleton()->get_pattern_vol_font();

	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int fontofs=(rowsize-font->get_height())/2;
	int textofs=p_row*rowsize+fontofs;
	int volofs=textofs+font->get_height()+1;
	int notewidth=font->get_width()*3;
	int columnwidth=font->get_width()*4;

	int fontxofs=font->get_width()+p_note.pos.column*columnwidth;



	static char buf[4]={0,0,0,0};
	if (p_note.note.is_note()) {

		switch( editor->get_pattern_note_edit_mode() ) {

		case Editor::MODE_NOTE: {


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
		case Editor::MODE_VOLUME: {


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

/*
void TrackViewPattern::paint_multiple_nonvisible_events( QPainter& p, int p_row, const Track_Pattern::NoteList& p_list) {

	PixmapFont * font=VisualSettings::get_singleton()->get_pattern_font();
	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int fontofs=(rowsize-font->get_height())/2;
	int textofs=p_row*rowsize+fontofs;
	int barofs=textofs+font->get_height()+1;
	int notewidth=font->get_width()*3;
	int columnwidth=font->get_width()*4;

	Track_Pattern::NoteList::const_iterator I=p_list.begin();

	int howmany=p_list.size();
	int maxevents=(rowsize-5)/2;
	if (howmany>maxevents)
		howmany=maxevents;
	int maxsize=(rowsize-5)-(howmany-1);

	for (int i=0;i<howmany;I++,i++) {

		PatternEdit::EdNote note = *I;
		int xofs=MARGIN_SIZE+note.pos.column*columnwidth;

		int from=i*maxsize/howmany;
		int to=((i+1)*maxsize/howmany);
		int eheight = to-from;
		if (eheight<=0) eheight=1;
		from+=i;

		printf("%i - from %i, height %i\n",i,from,eheight);
		QColor col=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT);



		if (note.note.is_note()) {

			switch( pattern_edit->get_note_edit_mode() ) {
			case PatternEdit::MODE_NOTE: {
				QColor bg=QColor(0,0,0);
				QColor fg=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT);

				int note_w=get_note_display_column_pos(notewidth,note.note.note);

				p.fillRect(xofs,eventofs+from,notewidth,eheight,bg);
				p.fillRect(xofs+note_w,eventofs+from,2,eheight,fg);
			} break;

			case PatternEdit::MODE_VOLUME: {

				QColor bg=QColor(0,0,0);
				QColor fg=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_VOLUME);

				int vol_w=(note.note.volume*notewidth)/PatternNote::MAX_VOLUME;
				p.fillRect(xofs,eventofs+from,notewidth,eheight,bg);
				p.fillRect(xofs,eventofs+from,vol_w,eheight,fg);
			} break;
			}
		} else if (note.note.is_note_off()) {
			QColor col=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_NOTEOFF);
			p.fillRect(xofs,eventofs+from,notewidth,eheight,col);
		} else {

			printf("WTF!!\n");
		}
	}
}
*/

void BlockListUI_Pattern::paint_multiple_note_events( QPainter& p, int p_row , const Track_Pattern::NoteList& p_list ) {


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

		if (notes_in_column[i].size()==1)
			paint_note_event(p,p_row,*notes_in_column[i].begin());
		//else
		//	paint_multiple_nonvisible_events(p,p_row,notes_in_column[i]);

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

		case Editor::MODE_NOTE: {

			xofs+=2*field*font->get_width();
		} break;
		case Editor::MODE_VOLUME: {

			xofs+=font->get_width()+field*font->get_width();
		} break;
	}

	p.drawPixmap(xofs,textofs,cursor);
}

void BlockListUI_Pattern::paint_frames(QPainter& p) {

	int begin_pos=-1;
	int old_block_idx=-1;
	int visible_rows=editor->get_cursor().get_window_size();
	SkinBox *sb=VisualSettings::get_singleton()->get_skin_box( hasFocus() ? SKINBOX_EDITING_PATTERN_SELECTED : SKINBOX_EDITING_PATTERN );

	for (int i=0;i<(visible_rows+3);i++) {
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(i);
		int block_idx=track->get_block_idx_at_pos(tick);

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

			sb->paint_into( p, 0,begin_h, width(), len_h );

		}

		if (begin)
			begin_pos=i;

	}

	if(hasFocus()) {

		p.setPen( VisualSettings::get_singleton()->get_color( COLORLIST_PATTERN_EDIT_FOCUS_RECT ) );
		p.drawRect(0,0,width()-1,height()-1);
	}

}

void BlockListUI_Pattern::paint_row_lines(QPainter &p) {

	int visible_rows=editor->get_cursor().get_window_size();
	int row_size=VisualSettings::get_singleton()->get_editing_row_height();

	for (int i=0;i<visible_rows;i++) {
		if (i==0) //dont want the first one!
			continue;
		
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(i);

		if ( (tick % TICKS_PER_BEAT)==0 ) {//beat
			
			int block=track->get_prev_block_from_idx( tick );
			if ( block!=-1) {
				
				if (track->get_block_pos(block)==tick)
					continue;
				
				if (track->get_block_pos(block)+track->get_block(block)->get_length()==tick)
					continue;
				
			}
			p.fillRect(0,i*row_size,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BEAT_LINE));
		
		} else
			p.fillRect(0,i*row_size,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));

		
		//p.drawRect(0,i*row_size,width(),0);


	}

}

void BlockListUI_Pattern::paintEvent(QPaintEvent *e) {

	editor->get_cursor().set_window_size( height() / VisualSettings::get_singleton()->get_editing_row_height() );

	QPainter p(this);

	p.fillRect(0,0,width(),height(),QColor(0,0,0));

	paint_frames(p);

	int visible_rows=editor->get_cursor().get_window_size();

	for (int i=0;i<visible_rows;i++) {

		Tick from=editor->get_cursor().get_snapped_window_tick_pos(i);
		Tick to=editor->get_cursor().get_snapped_window_tick_pos(i+1)-1;

		Track_Pattern::NoteList nl = track->get_notes_in_range( from, to );
		if (!nl.empty()) {

			paint_multiple_note_events( p,i , nl );
		}

		//paint_cursor(p,i);

	}

	paint_row_lines(p);




	if ( hasFocus() && editor->get_track_blocklist( track_idx) == editor->get_current_blocklist() &&
		    editor->get_cursor().get_pos()>=editor->get_cursor().get_window_offset() &&
		    editor->get_cursor().get_pos()<(editor->get_cursor().get_window_offset()+visible_rows)) {
				/* cursor is here */

		paint_cursor( p, editor->get_cursor().get_pos()-editor->get_cursor().get_window_offset() );

	}

}

void BlockListUI_Pattern::focusInEvent ( QFocusEvent * event ) {

	
	editor->set_current_blocklist( editor->get_track_blocklist( track_idx ) );
	QWidget::focusInEvent(event);
	
}

void BlockListUI_Pattern::mousePressEvent ( QMouseEvent * e ) {

	PixmapFont * font=VisualSettings::get_singleton()->get_pattern_font();

	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int columnwidth=font->get_width()*4;

	int click_x=e->x();
	if (editor->get_pattern_note_edit_mode()==Editor::MODE_NOTE)
		click_x+=font->get_width()/2; //makes clicking easier
	click_x-=font->get_width();
	if (click_x<0)
		click_x=0;

	int row=e->y()/VisualSettings::get_singleton()->get_editing_row_height();
	int column= click_x / columnwidth;
	int col_x=click_x % columnwidth;
	int field=col_x*2/columnwidth;
	editor->set_pattern_edit_field( field );
	editor->set_pattern_edit_column( column );
	editor->get_cursor().set_pos( editor->get_cursor().get_window_offset() + row );
	update();

}
void BlockListUI_Pattern::keyPressEvent ( QKeyEvent * e ) {


	int key_value=e->key();

	if (key_value!=Qt::Key_Backtab) { //ruins things otherwise
		
		bool alt=e->modifiers() & Qt::AltModifier;
		bool shift=e->modifiers() & Qt::ShiftModifier;
		bool control=e->modifiers() & Qt::ControlModifier;
		
		key_value|= (alt?Qt::ALT:0);
		key_value|= (shift?Qt::SHIFT:0);
		key_value|= (control?Qt::CTRL:0);
	}
	if (editor->pattern_edit_key_press(key_value))
		update();

}
BlockListUI_Pattern::BlockListUI_Pattern(QWidget *p_parent,Editor *p_editor, int p_track) : BlockListUI_Base(p_parent) {



	editor = p_editor;
	song=editor->get_song();
	track=dynamic_cast<Track_Pattern*>(song->get_track(p_track));
	track_idx=p_track;
	ERR_FAIL_COND(track==NULL);

	setBackgroundRole(QPalette::NoRole);

	int fontwidth=VisualSettings::get_singleton()->get_pattern_font()->get_width();
	int columns=track->get_visible_columns();

	int min_chars_width=2+3*columns+( (columns>1)?(columns-1):0 );
	setFixedWidth( min_chars_width*fontwidth );
	setFocusPolicy(Qt::ClickFocus);


}


BlockListUI_Pattern::~BlockListUI_Pattern()
{
}


}
