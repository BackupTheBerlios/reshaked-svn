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
#include "interface/visual_settings.h"
#include <Qt/qmessagebox.h>
#include <Qt/qinputdialog.h>
#include <Qt/qmenu.h>
#include <Qt/qcursor.h>
#include "interface/plugin_preset_browser.h"
#include "editor/plugin_preset_manager.h"
#include <Qt/qscrollbar.h>
namespace ReShaked {

void PluginTop::set_skipping_state(bool p_state) {
	
	skip->set_pressed( p_state );	
	
}

void PluginTop::info_pressed() {
	
	action_signal(ACTION_INFO,plugin);
}
void PluginTop::file_pressed() {
	
	
	//QList<QAction *> actions
	
	//QAction * exec ( QList<QAction *> actions, const QPoint & pos, QAction * at = 0 )	
	action_signal(ACTION_FILE,plugin);
	
}
void PluginTop::label_pressed() {
	
	action_signal(ACTION_RENAME,plugin);
	
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
	
void PluginTop::set_preset_name(QString p_name,bool p_referenced) {
	
	if (p_referenced)
		preset_name->set_color(QColor(255,200,200));
	else
		preset_name->set_color(QColor(200,220,255));
	
	preset_name->set_text( p_name );
}
	
PluginTop::PluginTop(QWidget *p_parent,QString p_name,int p_plugin,bool p_skipping,int p_total_plugins) :CHBox(p_parent) {
	
	plugin=p_plugin;
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__BEGIN));	
	
	PixmapButton *info = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__INFO),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__INFO_PUSHED)));
	QObject::connect(info,SIGNAL(mouse_pressed_signal()),this,SLOT(info_pressed()));
	
	preset_name=
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__PRESET_NAME),PixmapLabel::EXPAND_TILE_H);	
	
	preset_name->set_text( p_name );
	QObject::connect(preset_name,SIGNAL(clicked_signal()),this,SLOT(label_pressed()));
	
	file = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__PRESET),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__PRESET_PUSHED)));
	QObject::connect(file,SIGNAL(mouse_pressed_signal()),this,SLOT(file_pressed()));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__SEPARATOR));	

	skip = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__SKIP),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__SKIP_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(skip,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(skip_toggled(bool)));
	

	
	skip->set_pressed( p_skipping );
	
	if (p_plugin>0) {
		PixmapButton *move_left = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_LEFT),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_LEFT_PUSHED)));
		QObject::connect(move_left,SIGNAL(mouse_pressed_signal()),this,SLOT(move_left_pressed()));
	}
	if (p_plugin<(p_total_plugins-1)) {	
		PixmapButton *move_right = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_RIGHT),GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__MOVE_RIGHT_PUSHED)));
		QObject::connect(move_right,SIGNAL(mouse_pressed_signal()),this,SLOT(move_right_pressed()));
	}	
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
			
			QMessageBox *message = new QMessageBox(topLevelOf(this));
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
		case PluginTop::ACTION_RENAME: {
		
			QString current=QStrify(plugin->get_current_preset_name());
			bool ok;
			QString text = QInputDialog::getText(this, "Rename Plugin",
					"Name:", QLineEdit::Normal,
					current, &ok);
			
			if (ok)
				editor->set_plugin_preset_name(plugin,DeQStrify(text));

			//editor->track_move_plugin_left(track,plugin_index);
		} break;		
		
		case PluginTop::ACTION_FILE: {
			
			PluginPresetBrowser *ppb = new PluginPresetBrowser(topLevelOf(this),plugin);
			ppb->exec();
			
			switch (ppb->get_action()) {
				
				case PluginPresetBrowser::ACTION_OPEN: {
					
					editor->load_plugin_preset(plugin,DeQStrify( ppb->get_file() ),track,DeQStrify( get_file_from_path( ppb->get_file()) ),ppb->is_reference());
					
				} break;
				case PluginPresetBrowser::ACTION_SAVE: {
					
					if (PluginPresetManager::get_singleton()->save_plugin_preset(DeQStrify( ppb->get_file() ), plugin )) {
						
						QMessageBox::critical ( this, "Error", "Error Saving File." , QMessageBox::Ok,QMessageBox::NoButton);
					} else {
						
						editor->begin_meta_undo_block( "Saved File: " + DeQStrify( get_file_from_path( ppb->get_file()) )) ;
						
						editor->set_plugin_preset_name(plugin,DeQStrify( get_file_from_path( ppb->get_file()) ));
						editor->set_plugin_preset_file(plugin,DeQStrify( ppb->get_file() ),ppb->is_reference());
						
						editor->end_meta_undo_block();
						
					}
					
				} break;
				
			}
		
			delete ppb;
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
	
	//if (p_to_height>20) //for safety
	//	setMinimumHeight(p_to_height);
}


void SoundPluginRack::update_rack() {
	
	
	spacer=NULL;
	
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
		
		QString preset_name=QStrify(track->get_plugin( i )->get_current_preset_name());
		
		e.top=new PluginTop(plugin_vb,preset_name,i,track->get_plugin( i )->skips_processing(),track->get_plugin_count());
		rack_elements.push_back(e);
		QObject::connect(ui,SIGNAL(property_edited_signal( Property*, double )),this,SLOT(property_edited_slot( Property*, double )));
		QObject::connect(e.top,SIGNAL(action_signal( int,int )),this,SLOT(plugin_action_signal( int,int )),Qt::QueuedConnection);
		QObject::connect(ui,SIGNAL(property_options_requested( Property* )),this,SLOT(property_options_requested( Property* )),Qt::QueuedConnection); //queue is necesary to avoid crashes
		QObject::connect(ui,SIGNAL(preset_name_changed_signal()),this,SLOT(plugin_renamed_slot())); //queue is necesary to 		
		
		
	}	
	 
	spacer = new QFrame(rack_box);
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	rack_box->layout()->setMargin(0);
	rack_box->layout()->setSpacing(0);
	rack_box->adjustSize();
	rack_box->show();
	
	
	repaint();
	update_scrollbar();
}

void SoundPluginRack::repaint() {
	if (!track)
		return;
	
	ERR_FAIL_COND( rack_elements.size() != track->get_plugin_count() );
	
	for (int i=0;i<rack_elements.size();i++) {
			
		rack_elements[i].top->set_skipping_state( track->get_plugin( i)->skips_processing() );
		rack_elements[i].top->set_preset_name( QStrify( track->get_plugin( i)->get_current_preset_name() ), track->get_plugin(i)->is_current_file_referenced());
	}
}

void SoundPluginRack::h_qscrollbar_range_changed( int,int ) {
	
	update_scrollbar();
}

void SoundPluginRack::scrollbar_changed_slot(int p_val) {
	
	horizontalScrollBar()->setValue(p_val);
	
}

void SoundPluginRack::resizeEvent(QResizeEvent * event) {
	
	QAbstractScrollArea::resizeEvent(event);
	update_scrollbar();
}


void SoundPluginRack::plugin_renamed_slot() {
	
	
	repaint();
}

void SoundPluginRack::update_scrollbar() {
	
	if (!spacer)
		return;
	
	int tracklist_width=spacer->x();
	
	scrollbar->set_max( tracklist_width );
	
	scrollbar->set_pagesize( width() );
	
	/*
	if (tracklist_width<width())
		scrollbar->hide();
	else
		scrollbar->show();
	*/
	scrollbar->set_value( horizontalScrollBar()->value() );
}

void SoundPluginRack::set_scrollbar(PixmapScrollBar *p_scroll) {
	
	scrollbar=p_scroll;
	update_scrollbar();
	QObject::connect(p_scroll,SIGNAL(value_changed_signal( int )),this,SLOT(scrollbar_changed_slot(int)));

	QObject::connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),scrollbar,SLOT(set_value( int )));
	
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
	setFocusPolicy(Qt::NoFocus);
	viewport()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);
	scrollbar=NULL;
	spacer=NULL;
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	setFixedHeight(GET_CONSTANT(CONSTANT_RACK_MINIMUM_HEIGHT));

	setBackgroundRole(QPalette::NoRole);
	p=palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	setPalette(p);
	
	QObject::connect(horizontalScrollBar(),SIGNAL(rangeChanged( int,int )),this,SLOT(h_qscrollbar_range_changed( int,int )));
	
}


SoundPluginRack::~SoundPluginRack() {
	
}


}
