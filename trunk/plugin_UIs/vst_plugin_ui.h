

#ifndef VST_PLUGIN_UI_H
#define VST_PLUGIN_UI_H

#include "version.h"
#ifdef VST_ENABLED
#include "ui_blocks/helpers.h"
#include "drivers/vst_plugin.h"
#include "interface/sound_plugin_ui.h"
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/midi_params_editor.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
 


class VST_PersistentData : public QObject,public SoundPlugin_MetaData {
	
	Q_OBJECT
			
	static void plugin_ui_changed_param(void *instance,int p_param);			
signals:
	
	void property_changed_signal(int p_index);
private:
friend class VST_PluginUI;
	bool ui_visible;
	bool params_visible;
	MidiParamsEditor *midi_params_editor;
	VST_PersistentData() { ui_visible=false; params_visible=false; midi_params_editor=NULL; }
};

class VST_PluginUI : public SoundPluginUI {
	
	Q_OBJECT
			
			
	
		
	CHBox *hb_params;
	VST_Plugin *vst;
	PixmapButton *gui_button;
	PixmapButton *params_button;
	
	VST_PersistentData *pdata;
	
protected slots:	
	
	void gui_show_slot();
	void midi_props_edit_slot();
	void params_show_slot(bool p_slot);
	void presets_choose_slot();
	void property_changed_by_editor_slot(int p_which);
	
public:

	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	VST_PluginUI(QWidget *p_parent,VST_Plugin *p_plugin);
	~VST_PluginUI();

};

}









#endif
#endif