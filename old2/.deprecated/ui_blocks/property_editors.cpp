//
// C++ Implementation: property_editors
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <Qt/qlabel.h>
#include <Qt/qinputdialog.h>
#include <Qt/qevent.h>
#include <Qt/qtooltip.h>
#include "ui_blocks/knob.h"
#include "property_editors.h"
#include "helpers.h"
#include <math.h>

namespace ReShaked {

/*********************/



void PropertyEditKnob::changed() {
	
	
	set_value( get_property()->get_coeff_value() );
	update();
	setToolTip( QStrify( get_property()->get_text_value() ) );
	
}

void PropertyEditKnob::value_changed(float p_new_value) {
	
	set( get_property()->get_value_from_coeff( p_new_value) );
	setToolTip( QStrify( get_property()->get_text_value() ) );
	//QToolTip::showText( mapToGlobal( QPoint( 0, height() ) ), QStrify( get_property()->get_text_value() ), this );
}

void PropertyEditKnob::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		Knob::mousePressEvent(e);
}

PropertyEditKnob::PropertyEditKnob(QWidget *p_parent,const Skin& p_skin) : Knob(p_parent,p_skin) {
	
		
	
}
		
PropertyEditKnob::~PropertyEditKnob() {
	
	
	
}

/***********************************/


void PropertyEditLabel::set_suffix_visible(bool p_visible) {
	
	suffix=p_visible;
	changed();
}
void PropertyEditLabel::changed() {
	
	
	set_text(QStrify(get_property()->get_text_value(!suffix)));
	update();
}

void PropertyEditLabel::mousePressEvent(QMouseEvent *e) {
	
	ERR_FAIL_COND( get_property()==NULL );
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		PixmapLabel::mousePressEvent(e);
}

void PropertyEditLabel::wheelEvent( QWheelEvent * e ) {
	
	ERR_FAIL_COND( get_property()==NULL );
	
	if (e->delta()>0) 
		set( get() + get_property()->get_stepping() );
	else if (e->delta()<0)
		set( get() - get_property()->get_stepping() );

	changed();

}

void PropertyEditLabel::click_override() {
	
	ERR_FAIL_COND( get_property()==NULL );
	
	bool ok;
	
	QString range = QStrify(get_property()->get_text_value(  get_property()->get_min() ) + " to " + get_property()->get_text_value(  get_property()->get_max() ) );
	
	
	double val=QInputDialog::getDouble ( this, QStrify( get_property()->get_caption()), QString("Enter Value (") + range + ")", get(), get_property()->get_min(), get_property()->get_max(), 4, &ok);
	if (ok)
		set(val);
	
	changed();
	
}

PropertyEditLabel::PropertyEditLabel(QWidget *p_parent,QPixmap p_bg) : PixmapLabel(p_parent,p_bg) {
	

	get_font().setBold(true);
	suffix=true;

}

PropertyEditLabel::~PropertyEditLabel() {
	
	
}

/*********************/



void PropertyEditSlider::changed() {
	
	
	set_value( get_property()->get_coeff_value() );
	update();
	setToolTip( QStrify( get_property()->get_text_value() ) );
	
}

void PropertyEditSlider::value_changed(float p_new_value) {
	
	set( get_property()->get_value_from_coeff( p_new_value) );
	setToolTip( QStrify( get_property()->get_text_value() ) );
	
}

void PropertyEditSlider::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		PixmapSlider::mousePressEvent(e);
}

PropertyEditSlider::PropertyEditSlider(QWidget *p_parent,const Skin& p_skin,Type p_type,int p_margin_beg,int p_margin_end) : PixmapSlider(p_parent,p_skin,p_type,p_margin_beg,p_margin_end) {
	
		
	
}
		
PropertyEditSlider::~PropertyEditSlider() {
	
	
	
}

/***********************************/

void PropertyEditSliderVU::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		PixmapSliderVU::mousePressEvent(e);
}


void PropertyEditSliderVU::changed() {
	
	
	set_slider_value( get_property()->get_coeff_value() );
	update();
}

void PropertyEditSliderVU::slider_value_changed(float p_new_value) {
	
	set( get_property()->get_value_from_coeff( p_new_value) );
}

PropertyEditSliderVU::PropertyEditSliderVU(QWidget *p_parent,QPixmap p_skin) : PixmapSliderVU(p_parent,p_skin) {
	
		
	
}
		
PropertyEditSliderVU::~PropertyEditSliderVU() {
	
	
	
}

/***********************************/


void PropertyEditVU::changed() {
	
	set_value( get_property()->get_coeff_value() );
	update();
}

PropertyEditVU::PropertyEditVU(QWidget *p_parent,const Skin& p_skin,Type p_type,int p_margin_beg,int p_margin_end) : PixmapVU(p_parent,p_skin,p_type,p_margin_beg,p_margin_end) {
	
		
	
}
		
PropertyEditVU::~PropertyEditVU() {
	
	
	
}

/*******************/

void PropertyEditUpDown::changed() {
	
	/* updowns have no fear */
}

void PropertyEditUpDown::max_pressed_slot() {
	if (!get_property())
		return;
	
	set( get_property()->get_max() );
}
void PropertyEditUpDown::min_pressed_slot() {
	if (!get_property())
		return;
	
	set( get_property()->get_min() );
}
void PropertyEditUpDown::up_pressed_slot() {
	if (!get_property())
		return;
	
	set( get() + step );
}
void PropertyEditUpDown::down_pressed_slot() {
	if (!get_property())
		return;
	
	set( get() - step );
}

void PropertyEditUpDown::set_step(double p_step) {
	
	step=p_step;
}

void PropertyEditUpDown::config() {

	step=get_property()->get_stepping();
}
PropertyEditUpDown::PropertyEditUpDown(QWidget *p_parent,const Skin& p_skin) : PixmapUpDown(p_parent,p_skin) {
	
	QObject::connect(this,SIGNAL(down_pressed_signal()),this,SLOT(down_pressed_slot()));
	QObject::connect(this,SIGNAL(up_pressed_signal()),this,SLOT(up_pressed_slot()));
	QObject::connect(this,SIGNAL(min_pressed_signal()),this,SLOT(min_pressed_slot()));
	QObject::connect(this,SIGNAL(max_pressed_signal()),this,SLOT(max_pressed_slot()));
	step=0;
	
}
		
PropertyEditUpDown::~PropertyEditUpDown() {
	
	
	
}

/********* COMBO ********************/

void PropertyEditCombo::changed() {
	
	if (!get_property())
		return;
	
	clear();
	int opcount=lrint( (get_property()->get_max()-get_property()->get_min() )/get_property()->get_stepping() )+1;
	
	ERR_FAIL_COND( opcount>100 ); //wtf?
	
	for (int i=0;i<opcount;i++) {
		
		int op = lrint( get_property()->get_min() + (double)i*get_property()->get_stepping() );
		
		String optxt = get_property()->get_text_value( (double) op );
		
		add_item( QStrify( optxt ) );

	}
	
	double current = (get_property()->get()-get_property()->get_min())/get_property()->get_stepping();
	
	select_item( lrint ( current ) );
}
void PropertyEditCombo::item_selected(int p_to_item) {
	
	if (!get_property())
		return;
	
	set( (double) p_to_item );
	
}

void PropertyEditCombo::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		PixmapCombo::mousePressEvent(e);
}

PropertyEditCombo::PropertyEditCombo(QWidget *p_parent,QPixmap p_bg) : PixmapCombo(p_parent,p_bg) { }
PropertyEditCombo::~PropertyEditCombo() {}

/* BUTTON */




void PropertyEditButton::changed() {
	
	if ((get_property()->get()-get_property()->get_min()) < ( (get_property()->get_max()-get_property()->get_min())/2.0) ) 
		set_pressed( false );
	else
		set_pressed( true );
}

void PropertyEditButton::mouse_toggled(bool p_new_status) {
	
	set( p_new_status? get_property()->get_max() : get_property()->get_min() );
	
}

void PropertyEditButton::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		PixmapButton::mousePressEvent(e);
}

PropertyEditButton::PropertyEditButton(QWidget *p_parent,const Skin& p_skin) : PixmapButton(p_parent,p_skin,PixmapButton::TYPE_TOGGLE) {
	
	
}
PropertyEditButton::~PropertyEditButton() {
	
	
	
}


}
