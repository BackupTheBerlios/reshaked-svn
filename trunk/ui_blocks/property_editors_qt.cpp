//
// C++ Implementation: property_editors_qt
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "property_editors_qt.h"
#include <math.h>
namespace ReShaked {



void PropertyEditComboBox::config() {
	
	clear();
	printf("OPTIONS: %i\n",(int)get_property()->get_max());
	for (int i=0;i<=(int)get_property()->get_max();i++) {
		
		QString option=QStrify(get_property()->get_text_value((double)i));
		printf("Option %s\n",option.toAscii().data());
		addItem(option);
	}
	
	setCurrentIndex((int)get());
}

void PropertyEditComboBox::changed() {
	
	
	setCurrentIndex((int)get());
}

	
void PropertyEditComboBox::item_changed(int p_which) {
	
	
	set((double)p_which);
}

PropertyEditComboBox::PropertyEditComboBox(QWidget *p_widget) :QComboBox(p_widget) {
	
	QObject::connect(this,SIGNAL(activated(int)),this,SLOT(item_changed( int )));	
	
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


void PropertyEditSpinBox::value_changed_slot(double p_val) {
	
//	set( p_val );

}

void PropertyEditSpinBox::changed() {
	
	
	setValue( get_property()->get() );
}

PropertyEditSpinBox::PropertyEditSpinBox(QWidget *p_widget) : QDoubleSpinBox(p_widget) {

	QObject::connect(this,SIGNAL(valueChanged(double)),this,SLOT(value_changed_slot( double )));

}


QWidget * create_editor_for_property(Property *p_prop,QWidget *p_parent) {
	
	
	switch (p_prop->get_display_mode()) {
		
		case Property::DISPLAY_OPTIONS: {
			
			PropertyEditComboBox * prop = new PropertyEditComboBox(p_parent);
			prop->set_property( p_prop );
			return prop;
		} break;
		case Property::DISPLAY_SPIN: {
			PropertyEditSpinBox * prop = new PropertyEditSpinBox(p_parent);
			prop->set_property( p_prop );
			return prop;
			
			
		} break;
		default: {
			
			PropertyEditSpinBox * prop = new PropertyEditSpinBox(p_parent);
			prop->set_property( p_prop );
			return prop;
			
		} break;
		
	}
	
	//dead zone
	return NULL;
}

}
