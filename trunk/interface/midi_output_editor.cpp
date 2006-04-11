//
// C++ Implementation: midi_output_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_output_editor.h"
#include <Qt/qscrollarea.h>
#include <Qt/qlayout.h>

#include "engine/midi_driver_list.h"
#include "pixmaps/icon_device_disabled.xpm"
#include "pixmaps/icon_device_enabled.xpm"
#include "pixmaps/icon_more.xpm"
#include "pixmaps/icon_edit.xpm"
#include <Qt/qaction.h>
#include <Qt/qmenu.h>
#include "interface/midi_device_editor.h"

namespace ReShaked {



void MidiOutputEdit::edit_midi_description() {
	
	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	
	MidiDeviceEditor *midi_device_editor = new MidiDeviceEditor(this,current->get_output_device_description(output));
	
	midi_device_editor->exec();
	
	layout_name->setText( QStrify( current->get_output_device_description(output)->get_name() ) );
	
	delete midi_device_editor;
	
}

void MidiOutputEdit::check_status() {
	
	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	
	bool out_enabled=current->is_output_connected( output );
	
	enabled->setChecked( out_enabled );
	
	if (out_enabled)
		enabled->setIcon( QPixmap((const char**)icon_device_enabled_xpm) );
	else
		enabled->setIcon( QPixmap((const char**)icon_device_disabled_xpm) );
	
	output_device_name->setText( QStrify( current->get_output_name( output ) ) );
}


void MidiOutputEdit::connect_to_output() {

	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();

	current->connect_output( output, port_id );
	check_status();
		
}

void MidiOutputEdit::disconnect_output() {

	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	check_status();
	if (!current->is_output_connected( output) )
		return;
	current->disconnect_output( output );
	
		
}

void MidiOutputEdit::output_status_changed(bool p_on) {
	
	if (p_on)
		connect_to_output();
	else
		disconnect_output();
}

void MidiOutputEdit::midi_port_selected(QAction* p_action) {
	
	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	
	int port=p_action->data().toInt();
	
	port_id=current->get_port_identifier( port );
	
	connect_to_output();
	
}

void MidiOutputEdit::port_menu_select_slot() {
	
	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	
	menu->clear();
	
	for (int i=0;i<current->get_port_count();i++) {
		
		QAction* ac = new QAction( QStrify( current->get_port_name( i ) ) ,this);
		ac->setData(i);
		menu->addAction(ac);
	}
	
	menu->adjustSize();
	
	int ofs=menu->width()-select_output->width();
	menu->popup(select_output->mapToGlobal( QPoint(-ofs,select_output->height()) ) );
	
	
	
}

MidiOutputEdit::MidiOutputEdit(QWidget *p_parent,int p_output) : CHBox(p_parent) {
	
	
	output=p_output;
	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	ERR_FAIL_COND(current==NULL);
	MidiDeviceDescription *devdesc=current->get_output_device_description(p_output);
	ERR_FAIL_COND( devdesc==NULL );
	
	(new QLabel(" " + QString::number(p_output+1) + " ", this))->setFixedWidth(30);
	
	new QLabel("  "+QString("Name:")+QString("  "),this);
	layout_name = new QLineEdit(this);
	layout_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout_name->setText( QStrify( devdesc->get_name() ) );	
	layout_name->setReadOnly(true);
				
	QPushButton *edit_layout = new QPushButton(QPixmap((const char**)icon_edit_xpm),"",this);
	QObject::connect(edit_layout,SIGNAL(clicked()),this,SLOT(edit_midi_description()));
	
		
	new QLabel("  "+QString("Device:")+QString("  "),this);
	
	output_device_name = new QLineEdit(this);
	output_device_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	output_device_name->setReadOnly(true);
	output_device_name->setText( QStrify( current->get_output_name( p_output ) ) );
	
	select_output = new QPushButton(QPixmap((const char**)icon_more_xpm),"",this);
	QObject::connect( select_output, SIGNAL(clicked()), this, SLOT(port_menu_select_slot()) );
	
	port_id=current->get_output_identifier( output );
	
	enabled = new QPushButton(QPixmap((const char**)icon_device_disabled_xpm),"",this);
	enabled->setCheckable(true);
	QObject::connect(enabled,SIGNAL(toggled(bool)),this,SLOT(output_status_changed( bool )));
	
	check_status();
		
	menu = new QMenu(this);
	QObject::connect( menu, SIGNAL(triggered(QAction*)), this, SLOT( midi_port_selected(QAction*)) );
	
}


MidiOutputEditor::MidiOutputEditor(QWidget *p_parent) : QDialog (p_parent) {
	
	
	
	setLayout(new QVBoxLayout(this));
	
	MidiDriver *current = MidiDriverList::get_singleton()->get_driver();
	ERR_FAIL_COND(current==NULL);
	
	setWindowTitle( QStrify( current->get_name() ) + " Outputs");
	
	if (!current->is_active()) {
		
		new QLabel("Driver is disabled.",this);
		
	} else {
	
		QScrollArea *sa = new QScrollArea(this);
		layout()->addWidget(sa);
	
		CVBox *vb = new CVBox(sa);
		sa->setWidget(vb);	
		
		for (int i=0;i<current->get_output_count();i++) {
			
			MidiOutputEdit * moe = new MidiOutputEdit(vb,i);
		}
		sa->setWidgetResizable ( true );
		vb->show();
		
		current->rescan_ports();
		
	}
	QPushButton *acceptb = new QPushButton("Close",this);
	acceptb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	QObject::connect(acceptb,SIGNAL(clicked()),this,SLOT(accept()));
	layout()->addWidget(acceptb);
	layout()->setAlignment(acceptb,Qt::AlignHCenter);
	
	setMinimumSize(QSize(600,500));
	
	layout()->setMargin(10);
	layout()->setSpacing(10);
	
	
}


MidiOutputEditor::~MidiOutputEditor()
{
}


}

