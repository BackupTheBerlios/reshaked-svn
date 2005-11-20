//
// C++ Interface: helpers
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDHELPERS_H
#define RESHAKEDHELPERS_H

#include <Qt/qwidget.h>
#include <Qt/qlayout.h>
#include "typedefs.h"

namespace ReShaked {

/**
@author Juan Linietsky
*/
class CHBox : public QWidget {

	Q_OBJECT
		
	QLayout *l;
	bool event ( QEvent * e );
	
	
public:
	
	CHBox(QWidget *p_parent,bool p_vertical=false);

};


class CVBox : public CHBox {
	
	Q_OBJECT
public:	
	
	CVBox(QWidget *p_parent);	
};

QString QStrify(const String& p_str);

}

#endif
