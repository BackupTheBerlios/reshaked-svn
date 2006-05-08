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
#include "interface/visual_settings.h"
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
	
	SoundPluginChooser *plugin_chooser = new SoundPluginChooser(topLevelOf(this),track->can_use_synths(),track->get_channels());
	plugin_chooser->exec();
	int plugin_idx=plugin_chooser->get_selected_plugin_idx();
	int custom_chans=plugin_chooser->get_selected_channels();
	delete plugin_chooser;
	if (plugin_chooser->get_selected_plugin_idx()<0)
		return;
	/*
	int custom_channels=-1;
	if (SoundPluginList::get_singleton()->get_plugin_info(plugin_idx)->can_custom_channels) {
		int default_channels=track->get_channels();
	
		bool ok;
		int channels=QInputDialog::getInteger ( topLevelOf(this), "Channels Instanced", "Channels to Instance?", default_channels, 1, 8, 1, &ok);
		if (!ok)
			return;
		
		custom_channels=channels;
		
	}
	*/	
	SoundPlugin *plugin = SoundPluginList::get_singleton()->instance_plugin(plugin_idx,custom_chans);
	if (plugin==NULL) 
		return;
	editor->add_plugin_to_track(track,plugin);
}

void RackUI::repaint_rack() {
	
	connections->update_rack();
	plugins->repaint();
}

void RackUI::update_rack() {
	
	update_rack_combo_slot();
	update_selected_rack_slot();
	connections->update_rack();
	plugins->update_rack();
}



void RackUI::update_rack_combo_names_slot() {
	
	ERR_FAIL_COND( (editor->get_song()->get_track_count()+1)!=rack_choose->get_item_count() );
	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		rack_choose->set_item_text(i+1,QString("   >") +QStrify(editor->get_song()->get_track(i)->get_name()));
		
	}	
}

void RackUI::update_rack_combo_slot() {
	
	rack_choose->clear();
	rack_choose->add_item(">Track Rack");
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		rack_choose->add_item(QString("   >") +QStrify(editor->get_song()->get_track(i)->get_name()));
		
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
	
	tracks->rebuild_track_rack();
	
	if (selected_rack==0) {
		
		
		connections->set_audio_graph( &editor->get_song()->get_track_graph() );
		plugins->set_track( NULL );
		

		hbox_options->hide();
	} else {
		
		connections->set_audio_graph( &editor->get_song()->get_track(selected_rack-1)->get_plugin_graph() );		
		plugins->set_track( editor->get_song()->get_track(selected_rack-1) );
			
		hbox_options->show();
		update_rack_file_label();
		
	}
	
	if (rack_front->is_pressed())
		rack_front_selected();
	else
		rack_back_selected();
	
	
}
void RackUI::rack_front_selected() {
	
	if (selected_rack==0)
		stack->setCurrentWidget(tracks_vbox);
	else
		stack->setCurrentWidget(plugins_vbox);
	
	rack_back->set_pressed(false);	
	rack_front->set_pressed(true);	
}
void RackUI::rack_back_selected() {
	
	rack_back->set_pressed(true);	
	rack_front->set_pressed(false);	
	stack->setCurrentWidget(connections_vbox);
	connections->update_rack();
	
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

void RackUI::ui_update_callback() {
	
	tracks->update_VUs();
}

RackUI::RackUI(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_updater) : CVBox(p_parent) {
	
	editor=p_editor;
	
	CHBox *hbox_main = new CHBox(this); //holding all
	
	
	new PixmapLabel(hbox_main,GET_QPIXMAP(THEME_RACK_TOOLBAR__BEGIN));
	

	/*
	CVBox *rack_choose_vb = new CVBox(hbox_main);
	new PixmapLabel(rack_choose_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__TRACK_DROPDOWN_TOP));
	rack_choose = new PixmapCombo(rack_choose_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__TRACK_DROPDOWN));
	new PixmapLabel(rack_choose_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__TRACK_DROPDOWN_BOTTOM));
	rack_choose_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	*/
	
	rack_front = new PixmapButton(hbox_main,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_FRONT),GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_FRONT_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	rack_front->setToolTip("Show Rack Front (Controls)");

	rack_back = new PixmapButton(hbox_main,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_BACK),GET_QPIXMAP(THEME_RACK_TOOLBAR__SHOW_BACK_ACTIVE)),PixmapButton::TYPE_TOGGLE);	
	rack_back->setToolTip("Show Rack Back (Connections)");
	
	new PixmapLabel(hbox_main,GET_QPIXMAP(THEME_RACK_TOOLBAR__SEPARATOR));

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
	
	
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__SPACER),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_ICON));
	
	CVBox *preset_name_vb = new CVBox(hbox_options);
	new PixmapLabel(preset_name_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_LABEL_TOP));
	rack_label = new PixmapLabel(preset_name_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_LABEL));
	new PixmapLabel(preset_name_vb,GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_LABEL_BOTTOM));
	preset_name_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	rack_file = new PixmapButton(hbox_options,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_FILE),GET_QPIXMAP(THEME_RACK_TOOLBAR__PRESET_FILE_PUSHED)));	
	QObject::connect(rack_file,SIGNAL(mouse_pressed_signal()),this,SLOT(rack_presets()));
	rack_file->setToolTip("Presets");
	
	
	new PixmapLabel(hbox_options,GET_QPIXMAP(THEME_RACK_TOOLBAR__END));
	
	CHBox *hbox_rack = new CHBox(this); 
	
	PixmapList::Skin list_skin;
	
	list_skin.font_color=QColor(200,200,200);
	list_skin.font_height=11;
	list_skin.margin=10;
	list_skin.selected_bg_color=QColor(0xca,0xf3,0xfb);
	list_skin.selected_font_color=QColor(0,0,0);
	list_skin.separator=3;
	list_skin.separator_color=QColor(50,50,50);
	list_skin.skin_bg=GET_SKIN( SKINBOX_RACK_LIST );
	
	rack_choose = new PixmapList(hbox_rack,list_skin);
	rack_choose->setMinimumWidth(150);
	
	rack_scrollbar = new PixmapScrollBar(hbox_rack,PixmapScrollBar::Skin(GET_SKIN(SKINBOX_THEME_SCROLLBAR_V_BG),GET_SKIN(SKINBOX_THEME_SCROLLBAR_GRABBER)),PixmapScrollBar::TYPE_VERTICAL);
	
	rack_choose->set_scrollbar( rack_scrollbar );
	
	stack = new QStackedWidget(hbox_rack);
	
	connections_vbox = new CVBox(stack);
	
	connections = new ConnectionRack(connections_vbox,editor);
	connections->set_scrollbar( new PixmapScrollBar(connections_vbox,PixmapScrollBar::Skin(GET_SKIN(SKINBOX_THEME_SCROLLBAR_H_BG),GET_SKIN(SKINBOX_THEME_SCROLLBAR_GRABBER)),PixmapScrollBar::TYPE_HORIZONTAL) );
	connections->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	stack->addWidget(connections_vbox);
	
	plugins_vbox = new CVBox(stack);
	plugins = new SoundPluginRack(plugins_vbox,p_updater,editor);
	plugins->set_scrollbar( new PixmapScrollBar(plugins_vbox,PixmapScrollBar::Skin(GET_SKIN(SKINBOX_THEME_SCROLLBAR_H_BG),GET_SKIN(SKINBOX_THEME_SCROLLBAR_GRABBER)),PixmapScrollBar::TYPE_HORIZONTAL) );
	
	stack->addWidget(plugins_vbox);
	
	stack->setCurrentWidget(plugins_vbox);
	stack->setMinimumHeight(GET_CONSTANT(CONSTANT_RACK_MINIMUM_HEIGHT));	
	QObject::connect(rack_choose,SIGNAL(item_selected_signal( int )),this,SLOT(rack_selected_slot( int )));
	
	tracks_vbox = new CVBox(stack);
	tracks = new TrackRack(tracks_vbox,editor,p_updater);
	tracks->set_scrollbar( new PixmapScrollBar(tracks_vbox,PixmapScrollBar::Skin(GET_SKIN(SKINBOX_THEME_SCROLLBAR_H_BG),GET_SKIN(SKINBOX_THEME_SCROLLBAR_GRABBER)),PixmapScrollBar::TYPE_HORIZONTAL) );
	
	stack->addWidget(tracks_vbox);
	
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
