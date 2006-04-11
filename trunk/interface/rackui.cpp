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
#include <Qt/qinputdialog.h>
#include <Qt/qmessagebox.h>
#include "ui_blocks/visual_settings.h"
#include "engine/sound_plugin_list.h"
#include "interface/sound_plugin_chooser.h"
#include "engine/track_pattern.h"
#include "interface/plugin_preset_browser.h"
#include "editor/plugin_preset_manager.h"

namespace ReShaked {


void RackUI::add_plugin_slot() {

	Track *track;
	track=editor->get_song()->get_track( selected_rack-1 );
	ERR_FAIL_COND(track==NULL);
	
	SoundPluginChooser *plugin_chooser = new SoundPluginChooser(topLevelOf(this),track->can_use_synths());
	plugin_chooser->exec();
	int plugin_idx=plugin_chooser->get_selected_plugin_idx();
	delete plugin_chooser;
	if (plugin_chooser->get_selected_plugin_idx()<0)
		return;
	int custom_channels=-1;
	if (SoundPluginList::get_singleton()->get_plugin_info(plugin_idx)->can_custom_channels) {
		int default_channels=track->get_channels();
	
		bool ok;
		int channels=QInputDialog::getInteger ( topLevelOf(this), "Channels Instanced", "Channels to Instance?", default_channels, 1, 8, 1, &ok);
		if (!ok)
			return;
		
		custom_channels=channels;
		
	}
		
	SoundPlugin *plugin = SoundPluginList::get_singleton()->instance_plugin(plugin_idx,custom_channels);
	editor->add_plugin_to_track(track,plugin);
}

void RackUI::repaint_rack() {
	
	connections->update();
	plugins->repaint();
}

void RackUI::update_rack() {
	
	update_rack_combo_slot();
	update_selected_rack_slot();
	connections->update();
	plugins->update_rack();
}



void RackUI::update_rack_combo_names_slot() {
	
	ERR_FAIL_COND( (editor->get_song()->get_track_count()+1)!=rack_choose->get_item_count() );
	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		rack_choose->set_item_text(i+1,QStrify(editor->get_song()->get_track(i)->get_name()));
		
	}	
}

void RackUI::update_rack_combo_slot() {
	
	rack_choose->clear();
	rack_choose->add_item("Global Track Routing");
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		rack_choose->add_item(QStrify(editor->get_song()->get_track(i)->get_name()));
		
	}
	
	if (selected_rack<0)
		selected_rack=0;
	if (selected_rack>=rack_choose->get_item_count())
		selected_rack=rack_choose->get_item_count()-1;
	
	editor->set_current_rack_track( selected_rack - 1 );
	
	rack_choose->select_item(selected_rack);
}

void RackUI::rack_selected_slot(int p_selected_rack) {
	
	selected_rack=p_selected_rack;
	editor->set_current_rack_track( selected_rack - 1 );
	update_selected_rack_slot();
}

void RackUI::select_track(int p_track) {
	if ((p_track+1)==selected_rack)
		return;
	
	ERR_FAIL_INDEX(p_track,editor->get_song()->get_track_count());
	selected_rack=p_track+1;
	editor->set_current_rack_track( selected_rack - 1 );
	rack_choose->select_item( p_track+1 );
	update_selected_rack_slot();
	
	
}
void RackUI::select_global() {
	
	selected_rack=0;
	rack_choose->select_item( 0 );
	editor->set_current_rack_track( selected_rack - 1 );
	update_selected_rack_slot();
	
}

void RackUI::update_selected_rack_slot() {
	
	if ((editor->get_song()->get_track_count()+1)!=rack_choose->get_item_count())
		update_rack_combo_slot();
	
	if (selected_rack==0) {
		
		
		connections->set_audio_graph( &editor->get_song()->get_track_graph() );
		plugins->set_track( NULL );
		
		rack_back_selected();
		
		hbox_options->hide();
	} else {
		
		connections->set_audio_graph( &editor->get_song()->get_track(selected_rack-1)->get_plugin_graph() );		
		plugins->set_track( editor->get_song()->get_track(selected_rack-1) );
		
		if (rack_front->is_pressed())
			rack_front_selected();
		else
			rack_back_selected();
			
		hbox_options->show();
		update_rack_file_label();
		
	}
	
}
void RackUI::rack_front_selected() {
	
	stack->setCurrentWidget(plugins);
	rack_back->set_pressed(false);	
	rack_front->set_pressed(true);	
}
void RackUI::rack_back_selected() {
	
	rack_back->set_pressed(true);	
	rack_front->set_pressed(false);	
	stack->setCurrentWidget(connections);
	
}


void RackUI::update_rack_file_label() {
	
	Track *track;
	track=editor->get_song()->get_track( selected_rack-1 );
	
	if (!track) 
		return;
	
	QString label=QStrify(track->get_rack_file());
	
	if (label.lastIndexOf("/")!=-1)
		label.remove(0,label.lastIndexOf("/")+1);
	
	rack_label->set_text( label );
	
}

void RackUI::rack_presets() {
	
	
	Track *track;
	track=editor->get_song()->get_track( selected_rack-1 );
	
	if (!track)
		return;
	
	PluginPresetBrowser *ppb = new PluginPresetBrowser(topLevelOf(this),"");
	ppb->exec();
			
	switch (ppb->get_action()) {
				
		case PluginPresetBrowser::ACTION_OPEN: {
					
			
			editor->load_track_preset(track,DeQStrify( ppb->get_file() ));
			update_rack_file_label();
		} break;
		case PluginPresetBrowser::ACTION_SAVE: {
					
			if (PluginPresetManager::get_singleton()->save_track_rack(DeQStrify( ppb->get_file()), track)) {
				QMessageBox::critical ( this, "Error", "Error Saving File." , QMessageBox::Ok,QMessageBox::NoButton);
			} else {
				track->set_rack_file(  DeQStrify( ppb->get_file() ) );
			}
			
			update_rack_file_label();
		} break;
				
	}
		
	delete ppb;
	
	
}

void RackUI::test_note(int p_note,int p_vel) {
	
	if (selected_rack==0)
		return;
	Track *track;
	track=editor->get_song()->get_track( selected_rack-1 );

	if (!track)
		return;
	
	
	Track_Pattern *tp=dynamic_cast<Track_Pattern*>(track);
	
	EventMidi em;
	em.channel=0;
	em.data.note.note=p_note;
	em.data.note.velocity=p_vel;
	em.midi_type=EventMidi::MIDI_NOTE_ON;
	
	tp->add_edit_event( em, -1 );
}
void RackUI::test_note_off(int p_note_off) {
	
	if (selected_rack==0)
		return;
	Track *track;
	track=editor->get_song()->get_track( selected_rack-1 );

	if (!track)
		return;
	
	
	Track_Pattern *tp=dynamic_cast<Track_Pattern*>(track);
	
	EventMidi em;
	em.channel=0;
	em.data.note.note=p_note_off;
	em.data.note.velocity=0;
	em.midi_type=EventMidi::MIDI_NOTE_OFF;
	
	tp->add_edit_event( em, -1 );
	
}

void RackUI::blocklist_changed_slot() {
	
	if (!follow_cursor->is_pressed())
		return;
	
	int ct=editor->get_current_track();
	
	if (ct<0)
		return;
	
	select_track(ct);
	
}


RackUI::RackUI(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_updater) : CVBox(p_parent) {
	
	editor=p_editor;
	
	CHBox *hbox_main = new CHBox(this); //holding all
	
	
	new PixmapLabel(hbox_main,GET_QPIXMAP(THEME_RACK_TOOLBAR__BEGIN));
	

	CVBox *rack_choose_vb = new CVBox(hbox_main);
	new PixmapLabel(rack_choose_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__TRACK_DROPDOWN_TOP));
	rack_choose = new PixmapCombo(rack_choose_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__TRACK_DROPDOWN));
	new PixmapLabel(rack_choose_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__TRACK_DROPDOWN_BOTTOM));
	rack_choose_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	follow_cursor = new PixmapButton(hbox_main,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__RACK_FOLLOW_CURSOR),GET_QPIXMAP(THEME_RACK_TOOLBAR__RACK_FOLLOW_CURSOR_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	follow_cursor->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	follow_cursor->setToolTip("Rack Follows Cursor");
	
	
	new PixmapLabel(hbox_main,GET_QPIXMAP(THEME_RACK_TOOLBAR__SPACER),PixmapLabel::EXPAND_TILE_H);
		
	hbox_options = new CHBox(hbox_main); //
	
	hbox_options->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__SEPARATOR));

	add_plugin = new PixmapButton(hbox_options,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__ADD_PLUGIN),GET_QPIXMAP(THEME_RACK_TOOLBAR__ADD_PLUGIN_PUSHED)));
	QObject::connect(add_plugin,SIGNAL(mouse_pressed_signal()),this,SLOT(add_plugin_slot()));
	add_plugin->setToolTip("Add Synth/Effect Plugin");
	
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__SEPARATOR));
	
	rack_front = new PixmapButton(hbox_options,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_FRONT),GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_FRONT_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	rack_front->setToolTip("Show Rack Front (Controls)");

	rack_back = new PixmapButton(hbox_options,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_BACK),GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_BACK_ACTIVE)),PixmapButton::TYPE_TOGGLE);	
	rack_back->setToolTip("Show Rack Back (Connections)");
	
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__SEPARATOR));
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__SPACER),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_ICON));
	
	CVBox *preset_name_vb = new CVBox(hbox_options);
	new PixmapLabel(preset_name_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_LABEL_TOP));
	rack_label = new PixmapLabel(preset_name_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_LABEL));
	new PixmapLabel(preset_name_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_LABEL_BOTTOM));
	preset_name_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	rack_file = new PixmapButton(hbox_options,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_FILE),GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_FILE_PUSHED)));	
	QObject::connect(rack_file,SIGNAL(mouse_pressed_signal()),this,SLOT(rack_presets()));
	rack_back->setToolTip("Presets");
	
	
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__END));
	
	
	
	stack = new QStackedWidget(this);
	
	connections = new ConnectionRack(stack,editor);
	stack->addWidget(connections);
	
	plugins = new SoundPluginRack(stack,p_updater,editor);
	stack->addWidget(plugins);
	stack->setCurrentWidget(plugins);
	stack->setMinimumHeight(GET_CONSTANT(CONSTANT_RACK_MINIMUM_HEIGHT));	
	QObject::connect(rack_choose,SIGNAL(item_selected( int )),this,SLOT(rack_selected_slot( int )));
	
	hbox_options->layout()->setSpacing(0);
	hbox_options->layout()->setMargin(0);
	
	selected_rack=0;
	update_rack();
	
	QObject::connect(rack_back,SIGNAL(mouse_pressed_signal()),this,SLOT(rack_back_selected()));
	QObject::connect(rack_front,SIGNAL(mouse_pressed_signal()),this,SLOT(rack_front_selected()));
	
}


RackUI::~RackUI()
{
}


}
