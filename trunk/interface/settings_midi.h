//
// C++ Interface: settings_midi
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSETTINGS_MIDI_H
#define RESHAKEDSETTINGS_MIDI_H

#include "ui_blocks/helpers.h"

#include <Qt/qcombobox.h>
#include <Qt/qcheckbox.h>
#include <Qt/qpushbutton.h>
#include <Qt/qlineedit.h>

namespace ReShaked {

/**
	@author red <red@killy>
 */
class SettingsMidi : public CVBox {
	
	
	Q_OBJECT
	
	QLineEdit *driver_status;			
			
	QComboBox *select_engine;
	QCheckBox *driver_enabled;
	QPushButton *driver_restart;
	
	CVBox *options_holder;
	CVBox *driver_options;	
	
	void check_driver_status();
	void update_driver_options();	
	
public slots:	

	void restart();
	void toggle_active(bool p_active);
	void driver_changed(int p_to_which);

public:
	SettingsMidi(QWidget *p_parent);
	~SettingsMidi();

};

}

#endif
