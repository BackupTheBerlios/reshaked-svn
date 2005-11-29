//
// C++ Interface: global_beatbar_column
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_BEATBAR_COLUMN_H
#define RESHAKEDGLOBAL_BEATBAR_COLUMN_H

#include <Qt/qwidget.h>

namespace Reshaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GlobalBeatBarColumn : public QWidget {
				       
	void paintEvent(QPaintEvent *p);
public:
	GlobalBeatBarColumn(QWidget *p_parent);
	~GlobalBeatBarColumn();

};

}

#endif
