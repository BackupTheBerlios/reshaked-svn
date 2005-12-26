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
#include <Qt/qlabel.h>

namespace ReShaked {


void GlobalViewFrame::h_scollbar_changed_slot(int p_new_idx) {
	
		
	global_view->set_pixel_h_offset( p_new_idx );
}

void GlobalViewFrame::v_scollbar_changed_slot(int p_new_idx) {
	
	global_view->set_pixel_v_offset( p_new_idx );
}

void GlobalViewFrame::block_list_changed_slot() {
	
//	if (global_view->width()>=global_view->get_total_pixel_width()) {
		
//		h_scroll->hide();
//	} else {
		
		h_scroll->setRange(0,global_view->get_total_pixel_width()-global_view->width());	
//		h_scroll->show();
		h_scroll->setValue( global_view->get_pixel_h_offset() );
		h_scroll->setPageStep(global_view->width());
//	}
			
//	if (global_view->height()>=global_view->get_total_pixel_width()) {
		
//		h_scroll->hide();
//	} else {
		int v_range=(global_view->get_total_pixel_height()*2)-global_view->height();
		if (v_range<(global_view->height()*2))
			v_range=global_view->height()*2; //hack so there is always space to work with
		
		v_scroll->setRange(0,v_range);	
//		v_scroll->show();
		v_scroll->setValue( global_view->get_pixel_v_offset() );
		v_scroll->setPageStep(global_view->height());
//	}
	
}

void GlobalViewFrame::zoom_changed_slot(int p_to_val) {
	
	float zoom_val=(float)p_to_val/100.0;
	global_view->set_zoom( zoom_val );
	
}

void GlobalViewFrame::update() {
	
	block_list_changed_slot();
	global_view->update();	
}

GlobalView *GlobalViewFrame::get_global_view() {
	
	return global_view;
}

GlobalViewFrame::GlobalViewFrame(QWidget *p_parent,Editor *p_editor) : QFrame (p_parent) {
	
	editor=p_editor;
	
	QGridLayout *l = new QGridLayout(this);
	
	cursor_op= new GlobalViewCursor(this);
	
	l->addWidget(cursor_op,0,0,1,2);
		
	
	QFrame *gb_frame = new QFrame( this );
	gb_frame->setLayout(new QHBoxLayout(gb_frame));
	gb_frame->setLineWidth(2);
	gb_frame->setFrameStyle(Panel+Sunken);
	gb_frame->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	l->addWidget(gb_frame,1,0,1,2);
	beat_bar_column = new GlobalBeatBarColumn(gb_frame,editor);
	gb_frame->layout()->addWidget(beat_bar_column);
	global_view = new GlobalView( gb_frame, p_editor);
	gb_frame->layout()->addWidget(global_view);
	gb_frame->layout()->setMargin(0);
	gb_frame->layout()->setSpacing(0);
	
	
	beat_bar_column->set_global_view( global_view );
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
	zoom->setRange(0,100);
	zoom->setValue(0);
	
	h->addWidget(zoom);
	h->setStretchFactor(h_scroll,4);
	h->setStretchFactor(zoom,1);
	QLabel *z=new QLabel(this);
	z->setPixmap(GET_QPIXMAP(ICON_ZOOM_SMALL));
	l->addWidget(z,2,2);
	
	//some default
	zoom_changed_slot( 35 );
	zoom->setValue(35); 
	
	QObject::connect(global_view,SIGNAL(resized_signal()),this,SLOT(block_list_changed_slot()));
	QObject::connect(global_view,SIGNAL(resized_signal()),this,SIGNAL(global_view_changed_blocks_signal()));
	QObject::connect(h_scroll,SIGNAL(valueChanged(int)),this,SLOT(h_scollbar_changed_slot( int )));
	QObject::connect(v_scroll,SIGNAL(valueChanged(int)),this,SLOT(v_scollbar_changed_slot( int )));
	QObject::connect(zoom,SIGNAL(valueChanged(int)),this,SLOT(zoom_changed_slot( int )));
	QObject::connect(cursor_op,SIGNAL(edit_mode_changed_signal( GlobalView::EditMode )),global_view,SLOT(set_edit_mode( EditMode )));
	QObject::connect(cursor_op,SIGNAL(delete_clicked_signal()),global_view,SLOT(delete_selected_blocks()));
	
	l->setMargin(0);
	l->setSpacing(0);
	h->setMargin(0);
	
	setFrameStyle(Panel+Sunken);
	setLineWidth(1);
	
}


GlobalViewFrame::~GlobalViewFrame()
{
}


}
