//
// C++ Implementation: settings_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settings_dialog.h"
#include <Qt/qlayout.h>

namespace ReShaked {

SettingsDialog::SettingsDialog(QWidget *p_parent) :QDialog (p_parent)
{
	setLayout(new QHBoxLayout(this));
	
	layout()->addWidget(( tabs = new QTabWidget(this)));
	
	settings_sound = new SettingsSound(NULL);
	tabs->addTab(settings_sound,"Audio");
	
	settings_midi = new SettingsMidi(NULL);
	tabs->addTab(settings_midi,"MIDI");
	
	key_settings = new SettingsKeys(NULL);
	tabs->addTab(key_settings,"Key Bindings");
	
	setMinimumSize(500,400);	
	setWindowTitle("Reshaked Configuration");
}


SettingsDialog::~SettingsDialog()
{
}


}
