//
// C++ Implementation: settings_interface
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settings_interface.h"
#include <Qt/qlabel.h>

#include "editor/undo_stream.h"


namespace ReShaked {


void SettingsInterface::change_max_undo(int p_steps) {
	
	UndoStream::set_max_undo_steps( p_steps );
}

void SettingsInterface::change_theme_request() {
	
	
}

void SettingsInterface::save_template_request() {
	
	
}


void SettingsInterface::load(TreeLoader *p_loader) {
	
	ui_update_interval->setValue( p_loader->get_int("ui_update_interval") );
	ui_update_interval_changed( ui_update_interval->value() );	
	 
	ui_max_undo->setValue(p_loader->get_int("ui_max_undo"));
	UndoStream::set_max_undo_steps( ui_max_undo->value() );
	
	debug_theme->setChecked( p_loader->get_int("ui_debug_theme") );
	
}

void SettingsInterface::save(TreeSaver *p_saver) {
	
	p_saver->add_int("ui_update_interval",ui_update_interval->value());
	p_saver->add_int("ui_max_undo",ui_max_undo->value());
	p_saver->add_int("ui_debug_theme",debug_theme->isChecked());
	
	
}


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



SettingsInterface::SettingsInterface(QWidget *p_parent) : CVBox(p_parent){
	
	LABELS_BEGIN
			
	LABEL("Maximum UNDO Steps",this);
	
	ui_max_undo = new QSpinBox(hb);
	ui_max_undo->setMaximum(100);
	ui_max_undo->setMinimum(5);
	ui_max_undo->setValue(20);
	ui_max_undo->setSuffix(" steps");
	new QLabel("      "+QString("(WARNING: More steps = more system memory)"),this);
	
	QObject::connect(ui_max_undo,SIGNAL(valueChanged(int)),this,SLOT(change_max_undo( int )));
	
	LABEL("UI Update Interval",this);
	
	ui_update_interval = new QSpinBox(hb);
	ui_update_interval->setMaximum(500);
	ui_update_interval->setMinimum(25);
	ui_update_interval->setValue(50);
	ui_update_interval->setSuffix(" msec");
	
	QObject::connect(ui_update_interval,SIGNAL(valueChanged(int)),this,SIGNAL(ui_update_interval_changed( int )));
	
	LABEL("Theme",this);
	
	theme_name = new QLineEdit(hb);
	theme_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	theme_name->setReadOnly(true);
	theme_name->setText("Default");
	
	theme_file = new QPushButton("Change",hb);
	QObject::connect(theme_file,SIGNAL(clicked()),this,SLOT(change_theme_request()));
	
	new QLabel("      "+QString("(restart to apply)"),this);
	
	LABEL("Theme Development (please read help)",this);
	
	debug_theme = new QCheckBox("Debug Pixmaps (restart)",hb);
	debug_theme->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	save_template = new QPushButton("Save Template",hb);
	QObject::connect(save_template,SIGNAL(clicked()),this,SLOT(save_template_request()));
	
	//space up
	(new QFrame(this))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	layout()->setSpacing(10);
	layout()->setMargin(5);

	UndoStream::set_max_undo_steps( 20 );
	
}


SettingsInterface::~SettingsInterface()
{
}


}
