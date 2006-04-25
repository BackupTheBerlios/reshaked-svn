//
// C++ Interface: settings_interface
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSETTINGS_INTERFACE_H
#define RESHAKEDSETTINGS_INTERFACE_H

#include "ui_blocks/helpers.h"
#include <Qt/qspinbox.h>
#include <Qt/qlineedit.h>
#include <Qt/qcheckbox.h>
#include <Qt/qpushbutton.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SettingsInterface : public CVBox{
	
	Q_OBJECT
	
	QSpinBox *ui_update_interval;
	QSpinBox *ui_max_undo;
	
	QLineEdit *theme_name;
	QPushButton *theme_file;
	
	QCheckBox *debug_theme;
	QPushButton *save_template;

private slots:	
	
	void change_max_undo(int p_steps);
	
	void change_theme_request();
	
	void save_template_request();
signals:	
	
	void ui_update_interval_changed(int p_interval);
public:
	SettingsInterface(QWidget *p_parent);
	
	~SettingsInterface();

};

}

#endif
