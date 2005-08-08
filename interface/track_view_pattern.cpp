
#include "track_view_pattern.h"
#include <qpainter.h>

namespace ReShaked {


void TrackViewPattern::paintEvent(QPaintEvent *e) {

	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	printf("paint\n");
}



TrackViewPattern::TrackViewPattern(QWidget *p_parent,PatternEdit* p_edit) :TrackView(p_parent,p_edit) {

	setFixedWidth(200);

	
	pattern_edit=p_edit;
	printf("initia\n");

}



};



