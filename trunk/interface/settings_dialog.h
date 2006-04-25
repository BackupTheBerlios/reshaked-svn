//
// C++ Interface: settings_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSETTINGS_DIALOG_H
#define RESHAKEDSETTINGS_DIALOG_H


#include <Qt/qdialog.h>
#include <Qt/qtabwidget.h>
#include "interface/settings_sound.h"
#include "interface/settings_keys.h"
#include "interface/settings_midi.h"
#include "interface/settings_interface.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SettingsDialog : public QDialog {
	
	QTabWidget *tabs;
	SettingsSound *settings_sound;
	SettingsKeys *key_settings;
	SettingsMidi *settings_midi;
	SettingsInterface *settings_interface;
	
public:
	
	SettingsSound *get_settings_sound();
	SettingsKeys *get_key_settings();
	SettingsMidi *get_settings_midi();
	SettingsInterface *get_settings_interface();
	
	
	SettingsDialog(QWidget *p_parent);
	~SettingsDialog();

};

}

#endif
