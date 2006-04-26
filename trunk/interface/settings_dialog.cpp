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

SettingsSound *SettingsDialog::get_settings_sound() {
	
	return settings_sound;
}
SettingsKeys *SettingsDialog::get_key_settings() {
	
	return key_settings;
	
}
SettingsMidi *SettingsDialog::get_settings_midi() {
	
	return settings_midi;
}
SettingsInterface *SettingsDialog::get_settings_interface() {
	
	return settings_interface;
}


void SettingsDialog::load(TreeLoader *p_loader) {
	
	p_loader->enter("audio");
	settings_sound->load(p_loader);
	p_loader->exit();
	
	p_loader->enter("midi");
	settings_midi->load(p_loader);
	p_loader->exit();
	
	p_loader->enter("keyboard");
	key_settings->load(p_loader);
	p_loader->exit();
	
	p_loader->enter("interface");
	settings_interface->load(p_loader);
	p_loader->exit();
	
}
void SettingsDialog::save(TreeSaver *p_saver) {
	
	
	p_saver->enter("audio");
	settings_sound->save(p_saver);
	p_saver->exit();
	
	p_saver->enter("midi");
	settings_midi->save(p_saver);
	p_saver->exit();
	
	p_saver->enter("keyboard");
	key_settings->save(p_saver);
	p_saver->exit();
	
	p_saver->enter("interface");
	settings_interface->save(p_saver);
	p_saver->exit();
	
}


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
	
	settings_interface = new SettingsInterface(NULL);
	tabs->addTab(settings_interface,"Interface");
	
	setMinimumSize(500,400);	
	setWindowTitle("Reshaked Configuration");
}


SettingsDialog::~SettingsDialog()
{
}


}
