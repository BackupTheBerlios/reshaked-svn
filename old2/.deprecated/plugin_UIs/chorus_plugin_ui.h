//
// C++ Interface: chorus_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHORUS_PLUGIN_UI_H
#define RESHAKEDCHORUS_PLUGIN_UI_H

#include "interface/sound_plugin_ui.h"
#include "ui_blocks/pixmap_button.h"
#include "plugins/chorus_plugin.h"
#include "ui_blocks/property_editors.h"
namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class ChorusPluginUI : public SoundPluginUI {
	
	
	Q_OBJECT
	
	class SelectedVoice : public SoundPlugin_MetaData {
		public:	
		int which;
		
		SelectedVoice() { which=0; }
	};
	
	PropertyEditor* delay;
	PropertyEditor* rate;
	PropertyEditor* depth;
	PropertyEditor* level;
	PropertyEditor* width;
	PropertyEditor* cutoff;
	PropertyEditor* pan;
	PropertyEditor* pan_depth;
			
	ChorusPlugin *chorus;
	PixmapButtonGroup * current_voice;
	
	SelectedVoice * selected_voice;
	
protected slots:	
	
	void voice_selected_slot(int p_which);
	
public:
	
	
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	ChorusPluginUI(QWidget *p_parent,ChorusPlugin *p_chorus);
	~ChorusPluginUI();

};

}

#endif
