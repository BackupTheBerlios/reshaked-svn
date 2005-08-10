
#include "track_view_pattern.h"
#include <qpainter.h>
#include "pattern_settings.h"

namespace ReShaked {

#define INTERP_COLORVAL(m_from,m_to,m_idx,m_total) \
	(((int)m_from*((m_total-1)-m_idx)/m_total)+((int)m_to*m_idx/m_total))
#define INTERP_COLOR(m_from,m_to,m_idx,m_total) \
	QColor(INTERP_COLORVAL(m_from.red(),m_to.red(),m_idx,m_total),INTERP_COLORVAL(m_from.green(),m_to.green(),m_idx,m_total),INTERP_COLORVAL(m_from.blue(),m_to.blue(),m_idx,m_total))

void TrackViewPattern::paintEvent(QPaintEvent *e) {

	QPainter p(this);
	p.fillRect(0,0,width(),height(),PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_BG));
	printf("paint\n");

	/* paint left borders */
	int margin=PatternSettings::get_singleton()->get_pattern_margin();
	QColor light=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_LIGHT);
	QColor bg=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_BG);
	QColor shadow=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_SHADOW);

	for (int i=0;i<margin;i++) {


		QColor actual_light=INTERP_COLOR(light,bg,i,margin);
		QColor actual_dark=INTERP_COLOR(shadow,bg,i,margin);

		/* dark first */
		p.setPen(actual_dark);
		p.drawLine(i,height()-1-i,width()-1-i*2,height()-1-i);
		p.drawLine(width()-1-i,i,width()-1-i,height()-1-i);

		/* light second */
		p.setPen(actual_light);
		p.drawLine(i,i,width()-i*2,i);
		p.drawLine(i,i,i,height()-1-i);

	}


	int rowsize=PatternSettings::get_singleton()->get_row_size();
	int lines_fit=(height()-margin*2)/rowsize;
	//update cursor, small hack to make code simpler
	pattern_edit->get_cursor().set_window_size(lines_fit);

	for (int i=0;i<lines_fit;i++) {

		p.setPen(shadow);
		p.drawLine(0,margin+(i+1)*rowsize-1,width(),margin+(i+1)*rowsize-1);
		p.setPen(light);
		p.drawLine(0,margin+(i+1)*rowsize,width(),margin+(i+1)*rowsize);

		PatternEdit::NoteList nl = pattern_edit->get_notes_in_row(i);
		if (nl.empty())
			continue;

		for (PatternEdit::NoteList::iterator I = nl.begin() ; I!=nl.end();I++) {


		}

	}
}



TrackViewPattern::TrackViewPattern(QWidget *p_parent,PatternEdit* p_edit) :TrackView(p_parent,p_edit) {

	QFontMetrics metrics(PatternSettings::get_singleton()->get_font());
	setFixedWidth(metrics.maxWidth()*3+BORDER_MARGIN*2);
	columns_visible=1;

	pattern_edit=p_edit;
	printf("initia\n");

}



};



