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
#include <Qt/qpainter.h>


namespace ReShaked {


void CHBox::paintEvent(QPaintEvent *e) {
	
	if (!bg.isNull()) {
		
		
		QPainter p(this);
		p.drawTiledPixmap(0,0,width(),height(),bg);
	}
	
	
}

void CHBox::set_bg(QPixmap p_px) {
	
	bg=p_px;
}
bool CHBox::event( QEvent * e ) {

	
	bool res= QWidget::event(e);
		
	if (e->type()==QEvent::ChildAdded) {


		QChildEvent *ce=(QChildEvent*)e;
		if (ce->child()->isWidgetType())
			l->addWidget((QWidget*)ce->child());
		
	}	


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

QString QStrify(const String& p_str) {
	
	QString q;
	for (int i=0;i<p_str.length();i++) 
		q+=QChar(p_str[i]);

	return q;

}

String DeQStrify(const QString& p_str) {
	
	String s;
	for (int i=0;i<p_str.length();i++) 
		s+=p_str[i].unicode();

	return s;
	
	
}


QWidget *topLevelOf(QWidget* p_widget) {

    while (p_widget->parentWidget()!=NULL)
        p_widget=p_widget->parentWidget();

    return p_widget;

}

void BindIntSignal::emitter_slot() {
	
	activated_signal(val);
}
BindIntSignal::BindIntSignal(QObject *p_parent,int p_int) :QObject(p_parent){
	
	val=p_int;
	
}

QString get_note_string(int p_note) {
	
	if (p_note<0)
		return "";
	
	/* Helpers */
	static const char* note_str[12] = {
		"C-",
		"C#",
		"D-",
		"D#",
		"E-",
		"F-",

		"F#",
		"G-",
		"G#",
		"A-",
		"A#",
		"B-"
	};

	
	QString note=note_str[p_note%12];
	note+=QString::number(p_note/12);
	
	return note;
	
}

}
