//
// C++ Interface: midi_device_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_DEVICE_EDITOR_H
#define RESHAKEDMIDI_DEVICE_EDITOR_H

#include <Qt/qdialog.h>
#include <Qt/qlistwidget.h>
#include <Qt/qtablewidget.h>
#include <Qt/qspinbox.h>
#include <Qt/qlineedit.h>
#include <Qt/qtabwidget.h>
#include <Qt/qcombobox.h>

#include "ui_blocks/helpers.h"

#include "engine/midi_device_description.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/

class MidiDeviceEditor : public QDialog {
	
	Q_OBJECT
	
			
	QListWidget *banks;
	QTableWidget *patches;
	QTableWidget *controls;
	
	QSpinBox *bank_select_MSB;
	QSpinBox *bank_select_LSB;
	
	QLineEdit *device_name;
	
	QComboBox *bank_select_method;
	
	MidiDeviceDescription *desc;
	
	int current_bank;
	
	void update_current_bank();
	void update_bank_list();
	
	QTabWidget *tabs;		
	
	void update_device();
	
private slots:	
	
	void add_bank_slot();
	void erase_bank_slot();
	void add_gm_bank();
	void set_gm_controls();
	void patch_name_changed(int p_row,int p_col);
	void control_name_changed(int p_row,int p_col);
	void bank_selected_slot(int p_bank);
	void device_name_changed(const QString& p_new_text);
	void bank_name_changed(const QString&p_new_name);
	void bank_select_MSB_changed(int p_to_val);
	void bank_select_LSB_changed(int p_to_val);
	void bank_select_method_changed(int p_method);
	void check_bank_names_changed_horrible_qt4_hack();
	
	void load_device_slot();
	void save_device_slot();
	
	
public:
    
	MidiDeviceEditor(QWidget *p_parent,MidiDeviceDescription *p_desc);
	~MidiDeviceEditor();

};

}

#endif
