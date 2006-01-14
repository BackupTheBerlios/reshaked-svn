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


void SoundPluginRack::property_edited_slot(Property *p_prop,double p_old_val) {
	
	editor->property_changed( p_prop,p_old_val,track );
	
}

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
		
		ui->set_property_editor_updater( property_edit_updater );
		RackElement e;
		e.plugin_ui=ui;
		rack_elements.push_back(e);
		QObject::connect(ui,SIGNAL(property_edited_signal( Property*, double )),this,SLOT(property_edited_slot( Property*, double )));
		
	}	
	
	QFrame *spacer = new QFrame(rack_box);
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	rack_box->layout()->setMargin(0);
	rack_box->layout()->setSpacing(0);
	rack_box->show();
}

SoundPluginRack::SoundPluginRack(QWidget *p_parent,PropertyEditUpdater *p_updater,Editor *p_editor) : QScrollArea(p_parent) {
	
	editor=p_editor;
	property_edit_updater=p_updater;
	rack_box=NULL;
	track=NULL;
	QPalette p=viewport()->palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	viewport()->setPalette(p);
	setLineWidth(5);
	
}


SoundPluginRack::~SoundPluginRack() {
	
}


}
