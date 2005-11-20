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



}
