#ifndef ROW_LIST_H
#define ROW_LIST_H

#include <qwidget.h>
#include "editing/cursor.h"

namespace ReShaked {

class RowList : public QWidget {


	Cursor *cursor;
	void paintEvent(QPaintEvent *e);
public:

	RowList(QWidget *p_parent,Cursor *p_cursor);

};


};

#endif