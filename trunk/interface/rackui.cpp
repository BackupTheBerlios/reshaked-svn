//
// C++ Implementation: rackui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rackui.h"
#include <Qt/qlabel.h>
#include "ui_blocks/visual_settings.h"

namespace ReShaked {


void RackUI::repaint_rack() {
	
	connections->update();
}

void RackUI::update_rack() {
	
	update_rack_combo_slot();
	update_selected_rack_slot();
	connections->update();
	plugins->update_rack();
}

void RackUI::update_rack_combo_names_slot() {
	
	ERR_FAIL_COND( (editor->get_song()->get_track_count()+1)!=rack_choose->count() );
	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		rack_choose->setItemText(i+1,QStrify(editor->get_song()->get_track(i)->get_name()));
		
	}	
}

void RackUI::update_rack_combo_slot() {
	
	rack_choose->clear();
	rack_choose->addItem("Track Routing");
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		rack_choose->addItem(QStrify(editor->get_song()->get_track(i)->get_name()));
		
	}
	
	if (selected_rack<0)
		selected_rack=0;
	if (selected_rack>=rack_choose->count())
		selected_rack=rack_choose->count()-1;
	
	rack_choose->setCurrentIndex(selected_rack);
}

void RackUI::rack_selected_slot(int p_selected_rack) {
	
	selected_rack=p_selected_rack;
	update_selected_rack_slot();
}

void RackUI::update_selected_rack_slot() {
	
	if ((editor->get_song()->get_track_count()+1)!=rack_choose->count())
		update_rack_combo_slot();
	
	if (selected_rack==0) {
		
		connections->set_audio_graph( &editor->get_song()->get_track_graph() );
		plugins->set_track( NULL );
	} else {
		
		connections->set_audio_graph( &editor->get_song()->get_track(selected_rack-1)->get_plugin_graph() );		
		plugins->set_track( editor->get_song()->get_track(selected_rack-1) );
	}
	
}
void RackUI::rack_front_selected() {
	
	stack->setCurrentWidget(plugins);
	rack_back->setChecked(false);	
	rack_front->setChecked(true);	
}
void RackUI::rack_back_selected() {
	
	rack_back->setChecked(true);	
	rack_front->setChecked(false);	
	stack->setCurrentWidget(connections);
	
}

RackUI::RackUI(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_updater) : CVBox(p_parent) {
	
	editor=p_editor;
	
	CHBox *hbox_top = new CHBox(this);
	
	new QLabel(" Rack: ",hbox_top);
	
	rack_choose = new QComboBox(hbox_top);
	rack_choose->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

	rack_front = new QPushButton(hbox_top);
	rack_front->setIcon(GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_EFFECTS)); 
	rack_front->setCheckable(true);
	rack_front->setChecked(true);
	rack_back = new QPushButton(hbox_top);
	rack_back->setIcon(GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_CONNECTIONS)); 
	rack_back->setCheckable(true);
	rack_back->setChecked(false);
	
	QFrame *separate = new QFrame(hbox_top);
	separate->setFrameStyle(QFrame::VLine+QFrame::Sunken);
	separate->setFixedWidth(24);
	
	new QLabel("Preset: ",hbox_top);
	
	rack_name = new QLineEdit(hbox_top);
	rack_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	rack_load = new QPushButton(hbox_top);
	rack_save = new QPushButton(hbox_top);
	
	stack = new QStackedWidget(this);
	
	connections = new ConnectionRack(stack,editor);
	stack->addWidget(connections);
	
	plugins = new SoundPluginRack(stack,p_updater,editor);
	stack->addWidget(plugins);
	stack->setCurrentWidget(plugins);
	
	QObject::connect(rack_choose,SIGNAL(activated(int)),this,SLOT(rack_selected_slot( int )));
	
	hbox_top->layout()->setSpacing(5);
	
	selected_rack=0;
	update_rack();
	
	QObject::connect(rack_back,SIGNAL(clicked()),this,SLOT(rack_back_selected()));
	QObject::connect(rack_front,SIGNAL(clicked()),this,SLOT(rack_front_selected()));
	
}


RackUI::~RackUI()
{
}


}
