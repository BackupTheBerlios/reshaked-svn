#ifndef TRACK_VIEW_PATTERN_H
#define TRACK_VIEW_PATTERN_H


#include "interface/track_view.h"
#include "editing/pattern_edit.h"
#include  <qfont.h>


namespace ReShaked {


class TrackViewPattern : public TrackView {

	enum {

		BORDER_MARGIN=5
	};

	PatternEdit* pattern_edit;


	void paintEvent(QPaintEvent *e);


	
	public:


		TrackViewPattern(QWidget *p_parent,PatternEdit* p_edit);
};



};


#endif
