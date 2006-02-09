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
#include "ui_blocks/knob.h"
#include "property_editors.h"
#include "helpers.h"
#include <math.h>

namespace ReShaked {



void PropertyEditKnob::knob_changed(float p_to_val) {
	if (property==NULL)
		return;
	property->set_coeff_value(p_to_val);
	value->setText( QStrify(property->get_text_value()) ); 
	
}

void PropertyEditKnob::set_property(Property *p_property) {
	
	property=p_property;	
	label->setText( QStrify(property->get_caption()) );
	value->setText( QStrify(property->get_text_value()) ); 
	knob->set_value( property->get_coeff_value() );
	QObject::connect(knob,SIGNAL(value_changed_signal( float )),this,SLOT(knob_changed( float )));
	
}

PropertyEditKnob::PropertyEditKnob(QWidget *p_parent) :CVBox(p_parent) {
	
	label = new QLabel(this);
	knob = new Knob(this);
	value = new QLabel(this);
	label->setAlignment(Qt::AlignHCenter);
	value->setAlignment(Qt::AlignHCenter);
	
	layout()->setSpacing(0);
	property=NULL;
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

PropertyEditKnob::~PropertyEditKnob() {
	
	
}


void PropertyEditLabel::set_postfix_visible(bool p_visible) {
	
	postfix=p_visible;
	changed();
}
void PropertyEditLabel::changed() {
	
	
	set_text(QStrify(get_property()->get_text_value(!postfix)));
	update();
}

void PropertyEditLabel::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton)
		external_edit_signal( get_property() );
	else	
		PixmapLabel::mousePressEvent(e);
}

void PropertyEditLabel::click_override() {
	
	bool ok;
	
	double val=QInputDialog::getDouble ( this, QStrify( get_property()->get_caption()), "Enter Value", get(), get_property()->get_min(), get_property()->get_max(), 4, &ok);
	if (ok)
		set(val);
	
	changed();
	
}

PropertyEditLabel::PropertyEditLabel(QWidget *p_parent,QPixmap p_bg) : PixmapLabel(p_parent,p_bg) {
	

	get_font().setBold(true);
	postfix=true;

}

PropertyEditLabel::~PropertyEditLabel() {
	
	
}

/*********************/



void PropertyEditSlider::changed() {
	
	
	set_value( get_property()->get_coeff_value() );
	update();
}

void PropertyEditSlider::value_changed(float p_new_value) {
	
	set( get_property()->get_value_from_coeff( p_new_value) );
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

void PropertyEditSliderVU::changed() {
	
	
	set_value( get_property()->get_coeff_value() );
	update();
}

void PropertyEditSliderVU::value_changed(float p_new_value) {
	
	set( get_property()->get_value_from_coeff( p_new_value) );
}

PropertyEditSliderVU::PropertyEditSliderVU(QWidget *p_parent,const Skin& p_skin,Type p_type,int p_margin_beg,int p_margin_end) : PixmapSliderVU(p_parent,p_skin,p_type,p_margin_beg,p_margin_end) {
	
		
	
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



}
