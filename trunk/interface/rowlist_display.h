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
#include "engine/cursor.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class RowListDisplay : public QWidget {

	enum {
		CHAR_WIDTH=4
	};

	void paintEvent(QPaintEvent *p);
	Cursor *cursor;
public:

	RowListDisplay(QWidget *p_parent,Cursor *p_cursor);
	~RowListDisplay();
};

}

#endif
