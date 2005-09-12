//
// C++ Interface: cursor_qt
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCURSOR_QT_H
#define RESHAKEDCURSOR_QT_H

#include <qobject.h>
#include "editing/cursor.h"

namespace ReShaked {

/**
@author Juan Linietsky
*/
class CursorQt: public QObject, public Cursor {

	Q_OBJECT
	
signals:	
	void cursor_moved();;
	void cursor_changed_track();
	void window_moved();
public:	
    CursorQt(QObject *p_parent);

    ~CursorQt();

};

}

#endif
