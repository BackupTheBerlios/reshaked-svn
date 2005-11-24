//
// C++ Implementation: global_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_view_frame.h"
#include <Qt/qlayout.h>

namespace ReShaked {


void GlobalViewFrame::h_scollbar_changed_slot(int p_new_idx) {
	
		
	global_view->set_pixel_h_offset( p_new_idx );
}


void GlobalViewFrame::block_list_changed_slot() {
	
	if (global_view->width()>=global_view->get_total_pixel_width()) {
		
		h_scroll->hide();
	} else {
		
		h_scroll->setRange(0,global_view->get_total_pixel_width()-global_view->width());	
		h_scroll->show();
		h_scroll->setValue( global_view->get_pixel_h_offset() );
		h_scroll->setPageStep(global_view->width());
	}
			
}

void GlobalViewFrame::update() {
	
	block_list_changed_slot();
	global_view->update();	
}

GlobalView *GlobalViewFrame::get_global_view() {
	
	return global_view;
}

GlobalViewFrame::GlobalViewFrame(QWidget *p_parent,Editor *p_editor) : QWidget (p_parent) {
	
	editor=p_editor;
	
	QGridLayout *l = new QGridLayout(this);
	
	global_view = new GlobalView( this, p_editor);
	l->addWidget(global_view,1,1);
	v_scroll = new QScrollBar(Qt::Vertical,this);
	l->addWidget(v_scroll,1,2);
	
	QWidget *hw = new QWidget(this);
	QHBoxLayout *h = new QHBoxLayout(hw);
	hw->setLayout(h);
	
	l->addWidget(hw,2,1);
	h_scroll = new QScrollBar(Qt::Horizontal,hw);
	h_scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	h->addWidget(h_scroll);
	zoom = new QSlider(Qt::Horizontal,hw);
	h->addWidget(zoom);
	h->setStretchFactor(h_scroll,4);
	h->setStretchFactor(zoom,1);
	
	QObject::connect(global_view,SIGNAL(resized_signal()),this,SLOT(block_list_changed_slot()));
	QObject::connect(h_scroll,SIGNAL(valueChanged(int)),this,SLOT(h_scollbar_changed_slot( int )));
	
	l->setMargin(0);
	h->setMargin(0);
	
}


GlobalViewFrame::~GlobalViewFrame()
{
}


}
