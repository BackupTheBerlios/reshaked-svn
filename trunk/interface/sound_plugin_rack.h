//
// C++ Interface: sound_plugin_rack
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_RACK_H
#define RESHAKEDSOUND_PLUGIN_RACK_H

#include <Qt/qscrollarea.h>
#include "engine/track.h"
#include "editor/editor.h"
#include "ui_blocks/helpers.h"
#include "interface/sound_plugin_ui.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/pixmap_scrollbar.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
				    
				    

class PluginTop : public CHBox {
	
	Q_OBJECT
public:
	enum Action {
		
		ACTION_INFO,
		ACTION_FILE,
		ACTION_SKIP_ON,
		ACTION_SKIP_OFF,
		ACTION_MOVE_LEFT,
		ACTION_MOVE_RIGHT,
		ACTION_RENAME,
		ACTION_REMOVE,
	};
	
private:
			
	int plugin;
	PixmapButton *skip;
	PixmapButton *file;
	PixmapLabel *preset_name;
private slots:
	
	void label_pressed();
	void info_pressed();
	void file_pressed();
	void skip_toggled(bool p_toggle);
	void move_left_pressed();
	void move_right_pressed();
	void remove_pressed();
	
signals:
	
	void action_signal(int p_action,int p_plugin);
public:	
	void set_preset_name(QString p_name);
	void set_skipping_state(bool p_state);
	
	PluginTop(QWidget *p_parent,QString p_name,int p_plugin_index,bool p_skipping,int p_total_plugins);
};

class SoundPluginRack : public QScrollArea {
	
Q_OBJECT
		

		
		
	Track *track;
	Editor *editor;
	
	CHBox *rack_box;
	struct RackElement {
		
		SoundPluginUI *plugin_ui;
		PluginTop *top;
			
	};
	std::vector<RackElement> rack_elements;
	PropertyEditUpdater *property_edit_updater;
	PixmapScrollBar *scrollbar;
	
	QWidget *spacer;
	
	void update_scrollbar();
	

	void resizeEvent(QResizeEvent * event);
public slots:
	
	void h_qscrollbar_range_changed( int,int );
	
	void scrollbar_changed_slot(int p_val);
	
	void property_options_requested(Property *p_property);
	
	void plugin_action_signal(int,int);

	void rack_height_chanegd_slot(int p_to_height);
	void property_edited_slot(Property *p_prop,double p_old_val);
	void update_rack();
	void repaint();
public:
	void set_track(Track *p_track);
	void set_scrollbar(PixmapScrollBar *p_scroll);
	SoundPluginRack(QWidget *p_parent,PropertyEditUpdater *p_updater,Editor *p_editor);
	~SoundPluginRack();
};

}

#endif
