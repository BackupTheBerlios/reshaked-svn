//
// C++ Implementation: sound_plugin_rack
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_rack.h"
#include "interface/sound_plugin_ui_list.h"

namespace ReShaked {


void SoundPluginRack::set_track(Track *p_track) {
	
	if (track==p_track)
		return;
	track=p_track;
	update_rack();
}

void SoundPluginRack::update_rack() {
	if (rack_box)
		delete rack_box; //all will be deleted
	
	rack_elements.clear();
	rack_box=NULL;
	
	if (track==NULL)
		return;
	rack_box = new CHBox(this);
	setWidget(rack_box);
	setWidgetResizable(true);
	
	for (int i=0;i<track->get_plugin_count();i++) {
		
		SoundPluginUI *ui = SoundPluginUI_List::get_singleton()->create_UI( track->get_plugin( i ), rack_box );
		ERR_CONTINUE( ui==NULL );
		
		RackElement e;
		e.plugin_ui=ui;
		rack_elements.push_back(e);
	}	
	
	rack_box->show();
}

SoundPluginRack::SoundPluginRack(QWidget *p_parent) : QScrollArea(p_parent) {
	
	rack_box=NULL;
	track=NULL;
}


SoundPluginRack::~SoundPluginRack() {
	
}


}
