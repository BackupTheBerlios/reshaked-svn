//
// C++ Implementation: automation_input_select
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "automation_input_select.h"
#include <Qt/qlabel.h>
#include <Qt/qpushbutton.h>

#include <Qt/qtimer.h>

#include "midi_input_handler.h"

namespace ReShaked {


bool AutomationInputSelect::sens_midi_default=true;

bool AutomationInputSelect::is_input_enabled() {
	
	return input_enabled->isChecked();
}
int AutomationInputSelect::get_channel() {
	
	return channel_spin->value()-1;
}
int AutomationInputSelect::get_control() {
	
	return control_spin->value();
}


void AutomationInputSelect::timer_callback() {
	
	if (!MidiInputHandler::get_singleton()->is_last_control_available()) 
		return;
		
	int chan;
	int ctrl;
	int val;
	
	MidiInputHandler::get_singleton()->get_last_control( &chan,&ctrl,&val );
	
	if (!sens_check->isChecked())
		return;
	
	channel_spin->setValue(chan+1);
	control_spin->setValue(ctrl);
	
}

AutomationInputSelect::AutomationInputSelect(QWidget *p_parent,int p_current_chan,int p_current_ctrl,bool p_input_enabled) : QDialog(p_parent) {
	
	
	setLayout(new QVBoxLayout(this));
	
	sens_check = new QCheckBox("Sens parameters from\nMIDI Input");
	layout()->addWidget(sens_check);
	sens_check->setChecked(sens_midi_default);
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	new QLabel("Channel: ",hb);
	channel_spin = new QSpinBox(hb);
	channel_spin->setMaximum(16);
	channel_spin->setMinimum(1);
	channel_spin->setValue(p_current_chan+1);
	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	new QLabel("Control: ",hb);
	control_spin = new QSpinBox(hb);
	control_spin->setMaximum(127);
	control_spin->setMinimum(0);
	control_spin->setValue(p_current_ctrl);	
	
	input_enabled = new QCheckBox("Recording Enable");
	layout()->addWidget(input_enabled);
	input_enabled->setChecked(p_input_enabled);
	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	
	(new QWidget(hb))->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	QObject::connect(new QPushButton("Close",hb),SIGNAL(clicked()),this,SLOT(accept()));
	(new QWidget(hb))->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

	QTimer *t= new QTimer(this);
	t->start(500);
	QObject::connect(t,SIGNAL(timeout()),this,SLOT(timer_callback()));
	
	if (MidiInputHandler::get_singleton()->is_last_control_available()) {
		/* if anything exists from before, clear it */
		int crap;
		MidiInputHandler::get_singleton()->get_last_control( &crap,&crap,&crap );
	}
		
	
}


AutomationInputSelect::~AutomationInputSelect() {
	
	sens_midi_default=sens_check->isChecked();
}


}
