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
#include "ui_blocks/helpers.h"
#include "interface/sound_plugin_ui.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundPluginRack : public QScrollArea {
	
	Track *track;
	
	CHBox *rack_box;
	struct RackElement {
		
		SoundPluginUI *plugin_ui;
			
	};
	std::vector<RackElement> rack_elements;
public slots:
	
	void update_rack();
	
public:
	void set_track(Track *p_track);
	
	SoundPluginRack(QWidget *p_parent);
	~SoundPluginRack();
};

}

#endif
