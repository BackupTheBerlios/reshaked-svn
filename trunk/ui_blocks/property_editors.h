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
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"
#include "ui_blocks/pixmap_vu.h"
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

class PropertyEditLabel : public PixmapLabel, public PropertyEditor {

	Q_OBJECT

	virtual void changed();
	bool postfix;
public:
	void set_postfix_visible(bool p_visible);
	PropertyEditLabel(QWidget *p_parent,QPixmap p_bg);
	~PropertyEditLabel();

};

class PropertyEditSlider : public PixmapSlider, public PropertyEditor {

	Q_OBJECT

	virtual void changed();
	virtual void value_changed(float p_new_value);
public:

	PropertyEditSlider(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PropertyEditSlider();

};

class PropertyEditVU : public PixmapVU, public PropertyEditor {

Q_OBJECT

	
	virtual void changed();

public:

	PropertyEditVU(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PropertyEditVU();

};


}

#endif
