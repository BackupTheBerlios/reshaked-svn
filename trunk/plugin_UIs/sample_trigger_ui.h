//
// C++ Interface: sample_trigger_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAMPLE_TRIGGER_UI_H
#define RESHAKEDSAMPLE_TRIGGER_UI_H

#include "interface/sound_plugin_ui.h"
#include "ui_blocks/sample_viewer.h"

#include "plugins/sample_trigger.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class SampleTriggerUI : public SoundPluginUI {
	
	Q_OBJECT
	
	SampleViewer *viewer;			

	SampleTrigger *sample_trigger;

	
private slots:			


	void load_sample_slot();
	void edit_sample_slot();
public:
	
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	SampleTriggerUI(QWidget *p_parent,SampleTrigger *p_sample_trigger);
	~SampleTriggerUI();

};

}

#endif
