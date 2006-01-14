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

PropertyEditLabel::PropertyEditLabel(QWidget *p_parent,QPixmap p_bg) : PixmapLabel(p_parent,p_bg) {
	
	QFont f;
	f.setBold(true);
	int font_pix_size=p_bg.height()*2/3;
	f.setPixelSize(font_pix_size);
	set_font(f);
	set_pos(QPoint(p_bg.width()/2,(p_bg.height()-font_pix_size)/2),true);
	set_color(QColor(255,255,255));
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


void PropertyEditSpinBox::config() {
	
	/* calculate digits */
	int digits=-1;
	double interval=get_property()->get_stepping();
	
	if (interval!=0 || interval!=floorf(interval)) {
		
		digits=0;
		double step_aux=interval;
		do {
			step_aux*=10;
			//step_aux+=1e-10;
			digits++;
			if (step_aux-floor(step_aux) < 1e-6 )
				break;
			
		
		} while (true);
	} 
	
	if (digits<0)
		digits=4;
	
	setSuffix( QStrify(get_property()->get_postfix()) );
	setDecimals(digits);
	setMaximum( get_property()->get_max() );
	setMinimum( get_property()->get_min() );
	setSingleStep(get_property()->get_stepping() );

}

void PropertyEditSpinBox::changed() {
	
	
	setValue( get_property()->get() );
}

PropertyEditSpinBox::PropertyEditSpinBox(QWidget *p_widget) : QDoubleSpinBox(p_widget) {

	

}


}
