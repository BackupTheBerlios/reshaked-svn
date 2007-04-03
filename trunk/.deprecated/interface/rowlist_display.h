//
// C++ Interface: rowlist_display
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDROWLIST_DISPLAY_H
#define RESHAKEDROWLIST_DISPLAY_H

#include <Qt/qwidget.h>
#include "editor/editor.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class RowListDisplay : public QWidget {

	enum {
		CHAR_WIDTH=8
	};

	void paint_marker(QPainter& p, int p_marker_idx,int p_row,bool p_paint_arrow=true,int p_check_next=-1);
	void paintEvent(QPaintEvent *p);
	Cursor *cursor;
	Song *song;
public:

	RowListDisplay(QWidget *p_parent,Editor *p_editor);
	~RowListDisplay();
};

}

#endif
