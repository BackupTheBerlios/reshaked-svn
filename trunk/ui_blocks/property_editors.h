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
#include "ui_blocks/pixmap_slider_vu.h"
#include "ui_blocks/pixmap_vu.h"
#include "ui_blocks/pixmap_updown.h"
#include "property.h"
#include <Qt/qspinbox.h>


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
	void click_override();
	void mousePressEvent(QMouseEvent *e);		
signals:	

	void external_edit_signal(Property *p_property);
	
public:
	void set_postfix_visible(bool p_visible);
	PropertyEditLabel(QWidget *p_parent,QPixmap p_bg);
	~PropertyEditLabel();

};

class PropertyEditSlider : public PixmapSlider, public PropertyEditor {

	Q_OBJECT

	virtual void changed();
	virtual void value_changed(float p_new_value);
	
	void mousePressEvent(QMouseEvent *e);
signals:	

	void external_edit_signal(Property *p_property);
	
public:

	PropertyEditSlider(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PropertyEditSlider();

};

class PropertyEditSliderVU : public PixmapSliderVU, public PropertyEditor {

	Q_OBJECT

	virtual void changed();
	virtual void value_changed(float p_new_value);
public:

	PropertyEditSliderVU(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PropertyEditSliderVU();

};


class PropertyEditVU : public PixmapVU, public PropertyEditor {

Q_OBJECT

	
	virtual void changed();
public:

	PropertyEditVU(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PropertyEditVU();

};

class PropertyEditUpDown : public PixmapUpDown, public PropertyEditor {

Q_OBJECT

	double step;
	virtual void changed();
	virtual void config();
	
public slots:
	
	void max_pressed_slot();
	void min_pressed_slot();
	void up_pressed_slot();
	void down_pressed_slot();	
	
public:
	void set_step(double p_step);
	PropertyEditUpDown(QWidget *p_parent,const Skin& p_skin);
	~PropertyEditUpDown();

};




}

#endif
