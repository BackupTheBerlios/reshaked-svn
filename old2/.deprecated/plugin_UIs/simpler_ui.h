//
// C++ Interface: simpler_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSIMPLER_UI_H
#define RESHAKEDSIMPLER_UI_H

#include "interface/sound_plugin_ui.h"
#include "ui_blocks/sample_viewer.h"

#include "plugins/simpler.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class SimplerUI : public SoundPluginUI {
	
	Q_OBJECT
	
	SampleViewer *viewer;			
	
	Simpler *simpler;
	
	
	
private slots:			
	
	
	void load_sample_slot();
	void edit_sample_slot();
public:
		
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	SimplerUI(QWidget *p_parent,SoundPlugin *p_plugin);
	~SimplerUI();

};

}

#endif
