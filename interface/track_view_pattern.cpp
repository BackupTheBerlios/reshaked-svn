
#include "track_view_pattern.h"
#include <qpainter.h>
#include "pattern_font.h"

namespace ReShaked {


void TrackViewPattern::paintEvent(QPaintEvent *e) {

	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	printf("paint\n");
}



TrackViewPattern::TrackViewPattern(QWidget *p_parent,PatternEdit* p_edit) :TrackView(p_parent,p_edit) {

	QFontMetrics metrics(PatternFont::get_singleton()->get_font());
	setFixedWidth(metrics.maxWidth()*3+BORDER_MARGIN*2);
	

	
	pattern_edit=p_edit;
	printf("initia\n");

}



};



