//
// C++ Interface: automation_input_select
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUTOMATION_INPUT_SELECT_H
#define RESHAKEDAUTOMATION_INPUT_SELECT_H

#include <Qt/qdialog.h>
#include <Qt/qspinbox.h>
#include <Qt/qcheckbox.h>
#include "ui_blocks/helpers.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AutomationInputSelect : public QDialog {
	
	Q_OBJECT
	
	QCheckBox *sens_check;	
	QSpinBox *channel_spin;
	QSpinBox *control_spin;
	QCheckBox *input_enabled;	
	
	static bool sens_midi_default;
private slots:	
	
	void timer_callback();
	
public:
	bool is_input_enabled();
	int get_channel();
	int get_control();
	
	AutomationInputSelect(QWidget *p_parent,int p_current_chan=0,int p_current_ctrl=0,bool p_input_enabled=true);
	~AutomationInputSelect();

};

}

#endif
