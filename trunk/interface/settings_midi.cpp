//
// C++ Implementation: settings_midi
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settings_midi.h"
#include "engine/midi_driver_list.h"
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



void SettingsMidi::load(TreeLoader *p_loader) {

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	if (!p_loader->is_var("driver_name")) {
		
		MidiDriverList::get_singleton()->init_driver(0,false);
		select_engine->setCurrentIndex(0);
		check_driver_status();
		update_driver_options();

		return;
	}
	
	String name=p_loader->get_string("driver_name");
	
	int driver_idx=-1;
	for (int i=0;i<MidiDriverList::get_singleton()->get_driver_count();i++) {
		
		if ( MidiDriverList::get_singleton()->get_driver_name(i)==name ) {
			
			driver_idx=i;
			break;
		}
	}
	
	if (driver_idx==-1) {
		
		MidiDriverList::get_singleton()->init_driver(0,false);
		select_engine->setCurrentIndex(0);
		check_driver_status();
		update_driver_options();
		
		return;
	}
	
	MidiDriverList::get_singleton()->init_driver(driver_idx,false); //init but dont activate
	
	MidiDriver *d=MidiDriverList::get_singleton()->get_driver();
	
	for (int i=0;i<d->get_settings_count();i++) {
		
		String var_name="driver_setting__"+d->get_setting(i)->get_name();
		if (!p_loader->is_var(var_name))
			continue;
		
		d->get_setting(i)->set( p_loader->get_float(var_name) );
	}
	
	bool init_active=p_loader->get_int( "driver_active" );
	
	
	if (init_active)
		MidiDriverList::get_singleton()->init_driver(driver_idx,true); //init but dont activate
	
	p_loader->enter("outputs");
	
	for (int i=0;i<d->get_output_count();i++) {
		
		if (!p_loader->is_var("device_"+String::num(i)))
			continue;
		
		p_loader->enter("device_"+String::num(i));
		
		String id=p_loader->get_string( "identifier" );
		
		if (init_active)
			d->connect_output(i,id);
		
		
		p_loader->enter("description");
		
		MidiDeviceDescription *dd=d->get_output_device_description(i);
		
		
		dd->load(p_loader);
		
		p_loader->exit(); //description
		
		p_loader->exit(); //device
	}
	
	
	p_loader->exit();
	
	
	select_engine->setCurrentIndex(driver_idx);
	check_driver_status();
	update_driver_options();
	
}
void SettingsMidi::save(TreeSaver *p_saver) {
	

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	MidiDriver *d=MidiDriverList::get_singleton()->get_driver();
	
	ERR_FAIL_COND( d==NULL );
	
	p_saver->add_string( "driver_name" , MidiDriverList::get_singleton()->get_driver()->get_name() );
	p_saver->add_int( "driver_active" , d->is_active() );
	for (int i=0;i<d->get_settings_count();i++) {
		
		p_saver->add_float("driver_setting__"+d->get_setting(i)->get_name(),d->get_setting(i)->get());
	}
		
	
	p_saver->enter("outputs");
	
	for (int i=0;i<d->get_output_count();i++) {
		
		p_saver->enter("device_"+String::num(i));
		
		p_saver->add_string( "identifier",d->get_output_identifier( i ) );
		
		p_saver->enter("description");
		
		MidiDeviceDescription *dd=d->get_output_device_description(i);
		
		
		dd->save(p_saver);
		
		p_saver->exit(); //description
		
		p_saver->exit(); //device
	}
	
	
	p_saver->exit();
	
}


void SettingsMidi::restart() {

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	MidiDriverList::get_singleton()->init_driver();
	check_driver_status();
	
}
void SettingsMidi::toggle_active(bool p_active) {

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	if (p_active)
		MidiDriverList::get_singleton()->init_driver();
	else
		MidiDriverList::get_singleton()->finish_driver();
		
	check_driver_status();
			
}


void SettingsMidi::check_driver_status() {

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	int current_driver=MidiDriverList::get_singleton()->get_current_driver();
	
	driver_enabled->setChecked( MidiDriverList::get_singleton()->is_current_driver_active() );
	bool error=false;
	
	
	if (!MidiDriverList::get_singleton()->is_current_driver_active()) {
		
		String error=MidiDriverList::get_singleton()->get_driver(current_driver)->get_last_error();
		
		
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

void SettingsMidi::driver_changed(int p_to_which) {

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	MidiDriverList::get_singleton()->init_driver(p_to_which,MidiDriverList::get_singleton()->is_current_driver_active());
	check_driver_status();
	update_driver_options();
}

void SettingsMidi::update_driver_options() {

    ERR_FAIL_COND(MidiDriverList::get_singleton()->get_driver_count()==0);

	if (driver_options!=NULL)
		delete driver_options;
	
	driver_options = new CVBox(options_holder);
	driver_options->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	int current_driver=MidiDriverList::get_singleton()->get_current_driver();
	
	if (MidiDriverList::get_singleton()->get_driver(current_driver)->get_settings_count()==0)
		return;
	/* Optiosn here! */
	
	LABELS_BEGIN
	

	
	for (int i=0;i<MidiDriverList::get_singleton()->get_driver(current_driver)->get_settings_count();i++) {
		
		String prop_name=MidiDriverList::get_singleton()->get_driver(current_driver)->get_setting(i)->get_caption();
		LABEL(QStrify(prop_name),driver_options);		
		create_editor_for_property(MidiDriverList::get_singleton()->get_driver(current_driver)->get_setting(i),hb);
		
	}
	
	
}

SettingsMidi::SettingsMidi(QWidget *p_parent) :CVBox (p_parent)
{
	
	LABELS_BEGIN
	
	LABEL("Midi Driver",this);
	
	hb->layout()->setSpacing(4);
	select_engine = new QComboBox(hb);
	for(int i=0;i<MidiDriverList::get_singleton()->get_driver_count();i++)
		select_engine->addItem( QStrify(MidiDriverList::get_singleton()->get_driver_name(i) ) );
	
	select_engine->setCurrentIndex( MidiDriverList::get_singleton()->get_current_driver() );
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


SettingsMidi::~SettingsMidi()
{
}


}
