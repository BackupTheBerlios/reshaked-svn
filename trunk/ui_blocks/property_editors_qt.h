//
// C++ Interface: property_editors_qt
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPROPERTY_EDITORS_QT_H
#define RESHAKEDPROPERTY_EDITORS_QT_H


#include "ui_blocks/helpers.h"
#include "property.h"
#include <Qt/qspinbox.h>
#include <Qt/qcombobox.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/


class PropertyEditComboBox : public QComboBox, public PropertyEditor {
	
	Q_OBJECT
	
	virtual void config();
	virtual void changed();
	
private slots:
		
	void item_changed(int p_which);
public:	

	PropertyEditComboBox(QWidget *p_widget);
	
};


class PropertyEditSpinBox : public QDoubleSpinBox, public PropertyEditor {
	
	Q_OBJECT	
	virtual void config();
	virtual void changed();
	bool updating;
	QString textFromValue ( double v );	
private slots:
			
	void value_changed_slot(double p_val);
public:	
	
	PropertyEditSpinBox(QWidget *p_widget);
	
};



QWidget * create_editor_for_property(Property *p_prop,QWidget *p_parent);

}

#endif
