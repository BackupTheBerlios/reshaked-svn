//
// C++ Interface: synth_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSYNTH_UI_H
#define RESHAKEDSYNTH_UI_H

#include "interface/sound_plugin_ui.h"
#include "plugins/sinth.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/oscillator_editor.h"
#include "ui_blocks/oscillator_display.h"
#include "ui_blocks/filter_editor.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class SinthUI : public SoundPluginUI {
	
	Q_OBJECT
	
	Sinth *sinth;
	

	
	OscillatorDisplay *oscview_1;
	OscillatorDisplay *oscview_2;
	
	FilterEditor *filter_editor;
	
	
private slots:	
	
	void filter_mode_selected(int p_mode);
	void oscedit_make_sine_slot(int p_which);
	void oscedit_make_saw_slot(int p_which);
	void oscedit_make_square_slot(int p_which);
	void oscedit_edit_slot(int p_which);
	
public:


	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	SinthUI(QWidget *p_parent,Sinth *p_sinth);
	~SinthUI();

};

}

#endif
