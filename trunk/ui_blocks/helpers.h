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
String DeQStrify(const QString& p_str);

class BindIntSignal : public QObject {

	Q_OBJECT
	int val;
	
public slots:	
	void emitter_slot();
signals:
	
	void activated_signal(int);
public:
	
					
	BindIntSignal(QObject *p_parent,int p_int);	
};

#define connect_bind_int(m_from_obj,m_from_sig,m_to,m_to_sigslot,m_int) 		\
	{							   			\
		BindIntSignal *_bis = new BindIntSignal(m_from_obj,m_int);		\
		QObject::connect(m_from_obj,m_from_sig,_bis,SLOT(emitter_slot()));	\
		QObject::connect(_bis,SIGNAL(activated_signal(int)),m_to,m_to_sigslot);	\
	}
	
	
}
#endif
