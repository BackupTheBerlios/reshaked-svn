//
// C++ Implementation: helpers
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "helpers.h"
#include <Qt/qboxlayout.h>
#include <Qt/qevent.h>


namespace ReShaked {


bool CHBox::event( QEvent * e ) {

	
	bool res= QWidget::event(e);
		
	if (e->type()==QEvent::ChildAdded) {

		printf("event\n");		
		QChildEvent *ce=(QChildEvent*)e;
		if (ce->child()->isWidgetType())
			l->addWidget((QWidget*)ce->child());
		
	}	

	l->setSpacing(0);
	return res;
}

CHBox::CHBox(QWidget *p_parent,bool p_vertical) : QWidget(p_parent) {
	
	if (p_vertical) 
		l = new QVBoxLayout(this);
	else 
		l = new QHBoxLayout(this);

	l->setMargin(0);	
	l->setSpacing(0);
}


CVBox::CVBox(QWidget *p_parent) : CHBox(p_parent,true) {

}


}
