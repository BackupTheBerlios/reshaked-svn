//
// C++ Implementation: settings_sound
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settings_sound.h"
#include "engine/sound_driver_list.h"
#include "pixmaps/icon_restart.xpm"
#include "ui_blocks/property_editors_qt.h"
#include <Qt/qmessagebox.h>
#include <Qt/qlabel.h>
#include <Qt/qfont.h>
namespace ReShaked {

#define LABELS_BEGIN\
	CHBox *hb;\
	QLabel *label;\
	QFont font;\
	
	
#define LABEL(m_title,m_parent) \
	label = new QLabel(m_title,m_parent); \
	font=label->font(); \
	font.setBold(true); \
	label->setFont(font); \
	hb = new CHBox(m_parent); \
	(new QFrame(hb))->setFixedWidth(20); \



void SettingsSound::load(TreeLoader *p_loader) {
	
	
	
}
void SettingsSound::save(TreeSaver *p_saver) {
	
	
	
}


void SettingsSound::restart() {
	
	SoundDriverList::get_singleton()->init_driver();
	check_driver_status();
	
}
void SettingsSound::toggle_active(bool p_active) {
	
	if (p_active)
		SoundDriverList::get_singleton()->init_driver();
	else
		SoundDriverList::get_singleton()->finish_driver();
		
	check_driver_status();
			
}


void SettingsSound::check_driver_status() {
	int current_driver=SoundDriverList::get_singleton()->get_current_driver();
	
	driver_enabled->setChecked( SoundDriverList::get_singleton()->is_current_driver_active() );
	bool error=false;
	
	
	if (!SoundDriverList::get_singleton()->is_current_driver_active()) {
		
		String error=SoundDriverList::get_singleton()->get_driver(current_driver)->get_last_error();
		
		
		if (error!="") {
		
			QFont font;
			font.setBold(true);
			QPalette p=driver_status->palette();
			p.setColor(QPalette::Text,QColor(255,150,150));
			driver_status->setPalette(p);
			
			driver_status->setFont(font);
			driver_status->setText(QStrify(error));
		} else {
			
			QFont font;
			font.setBold(true);
			driver_status->setFont(font);
			driver_status->setText("Driver Disabled");
			QPalette p=driver_status->palette();
			p.setColor(QPalette::Text,QColor(255,255,255));
			driver_status->setPalette(p);
			
			
		}
		
	} else {
	
		QFont font;
		font.setBold(true);
		QPalette p=driver_status->palette();
		p.setColor(QPalette::Text,QColor(150,255,150));
		driver_status->setPalette(p);
		driver_status->setFont(font);
		driver_status->setText("Driver Active");
	
	}
}

void SettingsSound::driver_changed(int p_to_which) {
	
	SoundDriverList::get_singleton()->init_driver(p_to_which,SoundDriverList::get_singleton()->is_current_driver_active());
	check_driver_status();
	update_driver_options();
}

void SettingsSound::update_driver_options() {
	
	if (driver_options!=NULL)
		delete driver_options;
	
	driver_options = new CVBox(options_holder);
	driver_options->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	int current_driver=SoundDriverList::get_singleton()->get_current_driver();
	
	if (SoundDriverList::get_singleton()->get_driver(current_driver)->get_settings_count()==0)
		return;
	/* Optiosn here! */
	
	LABELS_BEGIN
	

	
	for (int i=0;i<SoundDriverList::get_singleton()->get_driver(current_driver)->get_settings_count();i++) {
		
		String prop_name=SoundDriverList::get_singleton()->get_driver(current_driver)->get_setting(i)->get_caption();
		LABEL(QStrify(prop_name),driver_options);		
		create_editor_for_property(SoundDriverList::get_singleton()->get_driver(current_driver)->get_setting(i),hb);
		
	}
	
	
}

SettingsSound::SettingsSound(QWidget *p_parent) :CVBox (p_parent)
{
	
	LABELS_BEGIN
	
	LABEL("Sound Driver",this);
	
	hb->layout()->setSpacing(4);
	select_engine = new QComboBox(hb);
	for(int i=0;i<SoundDriverList::get_singleton()->get_driver_count();i++)
		select_engine->addItem( QStrify(SoundDriverList::get_singleton()->get_driver_name(i) ) );
	
	select_engine->setCurrentIndex( SoundDriverList::get_singleton()->get_current_driver() );
	QObject::connect(select_engine,SIGNAL(activated(int)),this,SLOT(driver_changed( int )));

	
	select_engine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	driver_enabled = new QCheckBox("Active",hb);
	
	driver_restart = new QPushButton("Restart",hb);
	driver_restart->setIcon(QPixmap( (const char **)icon_restart_xpm ) );
	
	options_holder = new CVBox(this);
	driver_options=NULL;
	/* Spacer */
	(new QFrame(this))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	LABEL("Status",this);
	
	driver_status=new QLineEdit(hb);
	driver_status->setReadOnly(true);
	QPalette p=driver_status->palette();
	p.setColor(QPalette::Base,QColor(0,0,0));
	driver_status->setPalette(p);
	
	update_driver_options();	
	check_driver_status();
	

	layout()->setMargin(10);
	layout()->setSpacing(4);
	
	QObject::connect(driver_restart,SIGNAL(clicked()),this,SLOT(restart()));
	QObject::connect(driver_enabled,SIGNAL(toggled(bool)),this,SLOT(toggle_active( bool )));
}


SettingsSound::~SettingsSound()
{
}


}
