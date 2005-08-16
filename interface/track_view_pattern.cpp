
#include "track_view_pattern.h"
#include <qpainter.h>
#include "pattern_settings.h"

// make insert note
// make toggle volume/note/porta
// make insert/change vol/note/porta
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

	p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT));

	int note=p_note.note.note % 12;
	int octave = p_note.note.note / 12;

	static char buf[4]={0,0,0,0};
	buf[0]=note_str[note][0];	
	buf[1]=note_str[note][1];
	buf[2]=(octave>9)?('A'+(char)(octave-10)):('0'+octave);

	p.drawText(fontxofs,textofs,buf);
	p.setPen(QColor(0,0,0));
	p.drawLine(fontxofs,volofs,fontxofs+notewidth,volofs);
	p.setPen(QColor(255,255,255));
	int vol_w=(p_note.note.volume*notewidth)/PatternNote::MAX_VOLUME;
	p.drawLine(fontxofs,volofs,fontxofs+vol_w,volofs);
        
	
	
	

}

void TrackViewPattern::paint_multiple_note_events( QPainter& p, int p_row , const PatternEdit::NoteList& p_list ) {

	
	
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
				cursor_x=xofs+fontwidth*pattern_edit->get_cursor_field();
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

	
			if (nl.size()==1) {
	
				printf("one note event\n");
				paint_note_event( p, i, *nl.begin() );
			} else {
	
				paint_multiple_note_events( p,i , nl );
			}
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



