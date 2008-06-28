//
// C++ Interface: midi_output_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_OUTPUT_EDITOR_H
#define RESHAKEDMIDI_OUTPUT_EDITOR_H

#include "ui_blocks/helpers.h"
#include <Qt/qlineedit.h>
#include <Qt/qlabel.h>
#include <Qt/qpushbutton.h>
#include <Qt/qdialog.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/

/* hmm..

	[device number][device given name][edit][selected output][select][ENABLED]
*/

class MidiOutputEdit : public CHBox {
	
	Q_OBJECT

				
	QLineEdit *layout_name;
			
	QLineEdit *output_device_name;
	
	QPushButton *select_output;
	
	QPushButton *enabled;
	
	QMenu *menu;
	
	int output;
	
	String port_id;
	
	void connect_to_output();
	void disconnect_output();
	
private slots:	
	
	
	void edit_midi_description();
	
	void midi_port_selected(QAction*);
	
	void port_menu_select_slot();
	
	void check_status();
	
	void output_status_changed(bool p_on);
	
public:	
	
	MidiOutputEdit(QWidget *p_parent,int p_output);
	
};

class MidiOutputEditor : public QDialog {
public:

	MidiOutputEditor(QWidget *p_parent);
	~MidiOutputEditor();

};

}

#endif
