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
#include "ui_blocks/visual_settings.h"
#include <Qt/qmessagebox.h>
#include <Qt/qmenu.h>
#include <Qt/qcursor.h>

namespace ReShaked {

void PluginTop::set_skipping_state(bool p_state) {
	
	skip->set_pressed( p_state );	
	
}

void PluginTop::info_pressed() {
	
	action_signal(ACTION_INFO,plugin);
}
void PluginTop::file_pressed() {
	
	action_signal(ACTION_FILE,plugin);
	
}
void PluginTop::skip_toggled(bool p_toggle) {
	
	action_signal(p_toggle?ACTION_SKIP_ON:ACTION_SKIP_OFF,plugin);
	
}
void PluginTop::move_left_pressed() {
	
	action_signal(ACTION_MOVE_LEFT,plugin);
	
}
void PluginTop::move_right_pressed() {
	
	action_signal(ACTION_MOVE_RIGHT,plugin);
	
}
void PluginTop::remove_pressed() {
	
	action_signal(ACTION_REMOVE,plugin);
	
}
	
PluginTop::PluginTop(QWidget *p_parent,int p_plugin,bool p_skipping) :CHBox(p_parent) {
	
	plugin=p_plugin;
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__BEGIN));	
	
	PixmapButton *info = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__INFO),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__INFO_PUSHED)));
	QObject::connect(info,SIGNAL(mouse_pressed_signal()),this,SLOT(info_pressed()));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__PRESET_NAME),PixmapLabel::EXPAND_TILE_H);	
	
	PixmapButton *file = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__PRESET),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__PRESET_PUSHED)));
	QObject::connect(file,SIGNAL(mouse_pressed_signal()),this,SLOT(file_pressed()));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__SEPARATOR));	

	skip = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__SKIP),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__SKIP_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(skip,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(skip_toggled(bool)));
	

	
	skip->set_pressed( p_skipping );
	
	PixmapButton *move_left = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_LEFT),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_LEFT_PUSHED)));
	QObject::connect(move_left,SIGNAL(mouse_pressed_signal()),this,SLOT(move_left_pressed()));
	
	PixmapButton *move_right = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_RIGHT),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_RIGHT_PUSHED)));
	QObject::connect(move_right,SIGNAL(mouse_pressed_signal()),this,SLOT(move_right_pressed()));
	
	PixmapButton *remove = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__REMOVE),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__REMOVE_PUSHED)));
	QObject::connect(remove,SIGNAL(mouse_pressed_signal()),this,SLOT(remove_pressed()));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__END));	
	
}


void SoundPluginRack::plugin_action_signal(int p_action,int p_plugin) {
	
	ERR_FAIL_INDEX(p_plugin,track->get_plugin_count());
	int plugin_index=p_plugin;
	SoundPlugin *plugin=track->get_plugin(p_plugin);
	
	switch (p_action) {
		
		case PluginTop::ACTION_INFO: {
			
			QMessageBox *message = new QMessageBox(this);
			message->setWindowTitle(QStrify( plugin->get_info()->caption ));
			message->setText(QStrify( plugin->get_info()->long_description));
			
			if (plugin->get_info()->xpm_preview)
				message->setIconPixmap(QPixmap((const char**)plugin->get_info()->xpm_preview));
			else 
				message->setIcon( QMessageBox::Information );
			message->exec();
		} break;
		case PluginTop::ACTION_MOVE_LEFT: {
		
			editor->track_move_plugin_left(track,plugin_index);
		} break;		
		case PluginTop::ACTION_MOVE_RIGHT: {
		
			editor->track_move_plugin_right(track,plugin_index);
			
		} break;		
		case PluginTop::ACTION_REMOVE: {
		
			editor->remove_plugin_from_track(track,plugin_index);
			
		} break;		
		
		case PluginTop::ACTION_SKIP_ON: {
			
			editor->set_plugin_skips_processing(plugin, true);
			
		} break;
		case PluginTop::ACTION_SKIP_OFF: {
			
			editor->set_plugin_skips_processing(plugin, false);
			
		} break;
		
	}
}


void SoundPluginRack::property_options_requested(Property *p_property) {
	
	if (!track)
		return;
	
	int prop_idx=-1;
	for (int i=0;i<track->get_property_count();i++) {
		
		if (track->get_property(i)==p_property) {
			
			prop_idx=i;
			break;
		}
	}
	
	if (prop_idx==-1)
		return;
	
	bool is_automated=track->has_property_visible_automation(prop_idx);
	
	QList<QAction*> ac_list;
	QAction *op_auto = new QAction(GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_AUTOMATIONS),is_automated?"Hide Automation":"Show Automation",this);
	ac_list.push_back(op_auto);
	QAction *res = QMenu::exec(ac_list,QCursor::pos());
	
	if (res==op_auto) {
		
		if (is_automated)
			editor->hide_automation( prop_idx, track );
		else
			editor->show_automation( prop_idx, track );
	}
	
	foreach(I,ac_list) {
		delete *I;
	}
	
}

void SoundPluginRack::property_edited_slot(Property *p_prop,double p_old_val) {
	
	editor->property_changed( p_prop,p_old_val,track );
	
}

void SoundPluginRack::set_track(Track *p_track) {
	
	if (track==p_track)
		return;
	track=p_track;
	update_rack();
}

void SoundPluginRack::rack_height_chanegd_slot(int p_to_height) {
	
	if (p_to_height>20) //for safety
		setMinimumHeight(p_to_height);
}


void SoundPluginRack::update_rack() {
	
	
	
	if (rack_box) {
		rack_box->hide();
		delete rack_box; //all will be deleted
		viewport()->setBackgroundRole(QPalette::NoRole);
		
	}

	rack_elements.clear();
	rack_box=NULL;
	
	if (track==NULL)
		return;
	rack_box = new RackHBox(this);
	rack_box->hide();
	
	QObject::connect(rack_box,SIGNAL(new_height_signal(int)),this,SLOT(rack_height_chanegd_slot( int )));
	setWidget(rack_box);
	setWidgetResizable(true);
	
	for (int i=0;i<track->get_plugin_count();i++) {
		
		CVBox *plugin_vb = new CVBox(rack_box);
		plugin_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
		
		SoundPluginUI *ui = SoundPluginUI_List::get_singleton()->create_UI( track->get_plugin( i ), plugin_vb );
		ERR_CONTINUE( ui==NULL );
		
		ui->set_property_editor_updater( property_edit_updater );
		RackElement e;
		e.plugin_ui=ui;
		e.top=new PluginTop(plugin_vb,i,track->get_plugin( i )->skips_processing());
		rack_elements.push_back(e);
		QObject::connect(ui,SIGNAL(property_edited_signal( Property*, double )),this,SLOT(property_edited_slot( Property*, double )));
		QObject::connect(e.top,SIGNAL(action_signal( int,int )),this,SLOT(plugin_action_signal( int,int )),Qt::QueuedConnection);
		QObject::connect(ui,SIGNAL(property_options_requested( Property* )),this,SLOT(property_options_requested( Property* )));
		
		
		
	}	
	
	QFrame *spacer = new QFrame(rack_box);
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	rack_box->layout()->setMargin(0);
	rack_box->layout()->setSpacing(0);
	rack_box->show();
}

void SoundPluginRack::repaint() {
	if (!track)
		return;
	
	ERR_FAIL_COND( rack_elements.size() != track->get_plugin_count() );
	
	for (int i=0;i<rack_elements.size();i++) {
			
		rack_elements[i].top->set_skipping_state( track->get_plugin( i)->skips_processing() );
	}
}

SoundPluginRack::SoundPluginRack(QWidget *p_parent,PropertyEditUpdater *p_updater,Editor *p_editor) : QScrollArea(p_parent) {
	
	editor=p_editor;
	property_edit_updater=p_updater;
	rack_box=NULL;
	track=NULL;
	viewport()->setBackgroundRole(QPalette::NoRole);
	QPalette p=viewport()->palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	viewport()->setPalette(p);
	setLineWidth(0);
	setFrameStyle(QFrame::NoFrame);
	viewport()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	setMinimumHeight(GET_CONSTANT(CONSTANT_RACK_MINIMUM_HEIGHT));

}


SoundPluginRack::~SoundPluginRack() {
	
}


}
