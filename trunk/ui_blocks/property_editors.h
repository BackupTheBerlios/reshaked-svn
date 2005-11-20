//
// C++ Interface: property_editors
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPROPERTY_EDITORS_H
#define RESHAKEDPROPERTY_EDITORS_H

#include "ui_blocks/helpers.h"
#include "property.h"

class Knob;
class QLabel;

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PropertyEditKnob : public CVBox {

	Q_OBJECT

	Property *property;
	QLabel *label;
	Knob *knob;
	QLabel *value;
protected slots:
			
	void knob_changed(float p_to_val);
public:
	
	void set_property(Property *p_property);
	PropertyEditKnob(QWidget *p_parent);
	~PropertyEditKnob();

};

}

#endif
