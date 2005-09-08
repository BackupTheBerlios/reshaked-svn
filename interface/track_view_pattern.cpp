
#include "track_view_pattern.h"
#include <qpainter.h>
#include "pattern_settings.h"

// *make insert note
// *make toggle volume/note/porta
// *make insert/change vol/note/porta
// make snap menu
// make patterns/highlight
// make sidebar
// make home/end navigation menus
// make remaining editing
// make selection


namespace ReShaked {

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

void TrackViewPattern::paint_note_event( QPainter& p, int p_row, PatternEdit::EdNote & p_note ) {

	QFontMetrics metrics(PatternSettings::get_singleton()->get_font());
	int rowsize=PatternSettings::get_singleton()->get_row_size();
	int fontwidth=metrics.maxWidth();
	int fontofs=metrics.ascent()+3;
	int textofs=BORDER_MARGIN+p_row*rowsize+fontofs;
	int volofs=BORDER_MARGIN+(p_row+1)*rowsize-2;
	int notewidth=fontwidth*3;
	int columnwidth=fontwidth*4;
	int fontxofs=BORDER_MARGIN+p_note.pos.column*columnwidth;



	static char buf[4]={0,0,0,0};
	if (p_note.note.is_note()) {

		switch( pattern_edit->get_note_edit_mode() ) {

			case PatternEdit::MODE_NOTE: {

				//note text
				p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT));

				int note=p_note.note.note % 12;
				int octave = p_note.note.note / 12;

				buf[0]=note_str[note][0];
				buf[1]=note_str[note][1];
				buf[2]=(octave>9)?('A'+(char)(octave-10)):('0'+octave);
				p.drawText(fontxofs,textofs,buf);
				// volume
				p.setPen(QColor(0,0,0));
				p.drawLine(fontxofs,volofs,fontxofs+notewidth,volofs);
				p.setPen(QColor(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_VOLUME)));
				int vol_w=(p_note.note.volume*notewidth)/PatternNote::MAX_VOLUME;
				p.drawLine(fontxofs,volofs,fontxofs+vol_w,volofs);

			} break;
			case PatternEdit::MODE_VOLUME: {

				p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_VOLUME));

				buf[0]=' ';
				buf[1]='0'+p_note.note.volume/10;
				buf[2]='0'+p_note.note.volume%10;
				p.drawText(fontxofs,textofs,buf);
				int note_w=get_note_display_column_pos(notewidth,p_note.note.note);
				p.setPen(QColor(0,0,0));
				p.drawLine(fontxofs,volofs,fontxofs+notewidth,volofs);
				p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT));
				p.drawLine(fontxofs+note_w,volofs,fontxofs+note_w+1,volofs);
			} break;
		}

	} else if (p_note.note.is_note_off()) {

		p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_NOTEOFF));
		buf[0]='=';
		buf[1]='=';
		buf[2]='=';
		p.drawText(fontxofs,textofs,buf);
	}

	// draw volume

	if (p_note.note.is_note()) {
	}





}

void TrackViewPattern::paint_multiple_nonvisible_events( QPainter& p, int p_row, const PatternEdit::NoteList& p_list) {

	QFontMetrics metrics(PatternSettings::get_singleton()->get_font());
	int rowsize=PatternSettings::get_singleton()->get_row_size();
	int fontwidth=metrics.maxWidth();
	int fontofs=metrics.ascent()+3;
	int textofs=BORDER_MARGIN+p_row*rowsize+fontofs;
	int eventofs=BORDER_MARGIN+p_row*rowsize+3;
	int notewidth=fontwidth*3;
	int columnwidth=fontwidth*4;

	PatternEdit::NoteList::const_iterator I=p_list.begin();

	int howmany=p_list.size();
	int maxevents=(rowsize-5)/2;
	if (howmany>maxevents)
		howmany=maxevents;
	int maxsize=(rowsize-5)-(howmany-1);

	for (int i=0;i<howmany;I++,i++) {

		PatternEdit::EdNote note = *I;
		int xofs=BORDER_MARGIN+note.pos.column*columnwidth;

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

void TrackViewPattern::paint_multiple_note_events( QPainter& p, int p_row , const PatternEdit::NoteList& p_list ) {


	/* separate in columns */
	std::vector<PatternEdit::NoteList> notes_in_column( pattern_edit->get_columns() );


	PatternEdit::NoteList::const_iterator I=p_list.begin();
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
		else
			paint_multiple_nonvisible_events(p,p_row,notes_in_column[i]);

	}

}

void TrackViewPattern::paint_cursor(QPainter &p,int p_row) {


	if (
		   (pattern_edit->get_cursor().get_pos()==(pattern_edit->get_cursor().get_window_offset()+p_row)) && //match row
		   (get_track_index()==pattern_edit->get_cursor().get_track() && pattern_edit->get_cursor().get_edit_target()==Cursor::EDIT_TRACK)
	   ){


		QFontMetrics metrics(PatternSettings::get_singleton()->get_font());
		int rowsize=PatternSettings::get_singleton()->get_row_size();
		int fontwidth=metrics.maxWidth();
		int yofs=BORDER_MARGIN+p_row*rowsize;
		int volofs=BORDER_MARGIN+(p_row+1)*rowsize-2;
		int columnwidth=fontwidth*4;
		int xofs=BORDER_MARGIN+pattern_edit->get_cursor_column()*columnwidth;
		int fontheight=metrics.height();

		int cursor_x;
		switch (pattern_edit->get_note_edit_mode()) {

			case PatternEdit::MODE_NOTE: {

				cursor_x=xofs+fontwidth*2*pattern_edit->get_cursor_field();

			} break;
			case PatternEdit::MODE_VOLUME:
			case PatternEdit::MODE_PORTA: {
				cursor_x=xofs+fontwidth+fontwidth*pattern_edit->get_cursor_field();
			} break;
		}

		p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_CURSOR));

		p.drawRect(cursor_x-1,yofs+2,fontwidth+2,fontheight);
	}
}


#define INTERP_COLORVAL(m_from,m_to,m_idx,m_total) \
	(((int)m_from*((m_total-1)-m_idx)/m_total)+((int)m_to*m_idx/m_total))
#define INTERP_COLOR(m_from,m_to,m_idx,m_total) \
	QColor(INTERP_COLORVAL(m_from.red(),m_to.red(),m_idx,m_total),INTERP_COLORVAL(m_from.green(),m_to.green(),m_idx,m_total),INTERP_COLORVAL(m_from.blue(),m_to.blue(),m_idx,m_total))



void TrackViewPattern::paintEvent(QPaintEvent *e) {

	QPainter p(this);
	p.setFont(PatternSettings::get_singleton()->get_font());
	printf("paint\n");

	/* paint left borders */
	int margin=PatternSettings::get_singleton()->get_pattern_margin();
	QColor light=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_LIGHT);
	QColor bg=PatternSettings::get_singleton()->get_color(!hasFocus()?PatternSettings::COLOR_PATTERN_BG:PatternSettings::COLOR_PATTERN_BG_FOCUS);
	QColor shadow=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_SHADOW);

	p.fillRect(0,0,width(),height(),bg);

	for (int i=0;i<margin;i++) {


		QColor actual_light=INTERP_COLOR(light,bg,i,margin);
		QColor actual_dark=INTERP_COLOR(shadow,bg,i,margin);

		/* dark first */
		p.setPen(actual_dark);
		p.drawLine(i,height()-1-i,width()-1-i*2,height()-1-i);
		p.drawLine(width()-1-i,i,width()-1-i,height()-1-i);

		/* light second */
		p.setPen(actual_light);
		p.drawLine(i,i,i,height()-1-i);
		p.drawLine(i,i,width()-i,i);


	}

	for (int i=1;i<pattern_edit->get_columns();i++) {

		int w = width()-BORDER_MARGIN*2;
		int from=BORDER_MARGIN+i*w/pattern_edit->get_columns();

		p.setPen(light);
		p.drawLine(from-1,0,from-1,height());
		p.setPen(shadow);
		p.drawLine(from,0,from,height());
	}

	int rowsize=PatternSettings::get_singleton()->get_row_size();
	int lines_fit=(height()-margin*2)/rowsize;
	//update cursor, small hack to make code simpler
	pattern_edit->get_cursor().set_window_size(lines_fit);

	for (int i=0;i<lines_fit;i++) {

		p.setPen(shadow);
		p.drawLine(width()-BORDER_MARGIN,margin+(i+1)*rowsize-1,width(),margin+(i+1)*rowsize-1);
		p.drawLine(0,margin+(i+1)*rowsize-1,BORDER_MARGIN-1,margin+(i+1)*rowsize-1);
		p.setPen(light);
		p.drawLine(0,margin+(i+1)*rowsize,width()-BORDER_MARGIN,margin+(i+1)*rowsize);
		p.setPen(bg);
		p.drawLine(width()-BORDER_MARGIN,margin+(i+1)*rowsize,width(),margin+(i+1)*rowsize);


		PatternEdit::NoteList nl = pattern_edit->get_notes_in_row(i);
		if (!nl.empty()) {

			paint_multiple_note_events( p,i , nl );
		}
		//printf("gah\n");
		paint_cursor(p,i);

	}
}


void TrackViewPattern::keyPressEvent (QKeyEvent *e) {

	int key_value=e->key();
	bool alt=e->state() & Qt::AltButton;
	bool shift=e->state() & Qt::ShiftButton;
	bool control=e->state() & Qt::ControlButton;

	key_value|= (alt?Qt::ALT:0);
	key_value|= (shift?Qt::SHIFT:0);
	key_value|= (control?Qt::CTRL:0);

	if (pattern_edit->key_press_event(key_value))
		update();
}
void TrackViewPattern::keyReleaseEvent (QKeyEvent *e) {


}


TrackViewPattern::TrackViewPattern(QWidget *p_parent,PatternEdit* p_edit,SongEdit *p_song_edit,int p_track) :TrackView(p_parent,p_edit,p_track) {

	QFontMetrics metrics(PatternSettings::get_singleton()->get_font());
	int cols=p_edit->get_columns();
	if (cols==1)
		setFixedWidth(metrics.maxWidth()*3+BORDER_MARGIN*2);
	else
		setFixedWidth(metrics.maxWidth()*3*cols+metrics.maxWidth()*(cols-1)+BORDER_MARGIN*2);

		song_edit=p_song_edit;
	pattern_edit=p_edit;
	printf("initia\n");
	setFocusPolicy(QWidget::ClickFocus);


}



};



