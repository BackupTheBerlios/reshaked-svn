//
// C++ Implementation: track_top
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_top.h"
#include "ui_blocks/visual_settings.h"
#include "ui_blocks/helpers.h"
#include <Qt/qpainter.h>
#include <Qt/qfont.h>
#include <Qt/qfontmetrics.h>
#include <Qt/qevent.h>
#include <Qt/qinputdialog.h>
#include "ui_blocks/helpers.h"
#include "interface/indexed_action.h"

#include "engine/sound_plugin_list.h"
#include "interface/automation_tree.h"

namespace ReShaked {

void TrackTop::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);

	GET_SKIN(SKINBOX_TRACK_TOP_BG)->paint_into(p,0,0,width(),height());
	QPixmap px = VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS );
	p.drawPixmap(0,0,px);
	
	if (!track)
		return;
	
	QFont f;
	f.setPixelSize(height()*2/3);
	f.setBold(true);
	p.setFont(f);
	QFontMetrics m(p.font());

	int ofs=px.width()+2;
	
	QString name=QStrify(track->get_name());
	
	if ( (ofs+m.boundingRect(name).width())>width()) { //doesnt fit!
		
		int eat=1;
		do {
			
			QString str=name.left( name.length() - eat )+"..";
			
			if ( (ofs+m.boundingRect(str).width()) < width() ) {
				
				name=str;
				break;
				
			}
			
			eat++;
			
		} while (eat<name.length());
	}
		
	
	p.setPen(GET_QCOLOR(COLORLIST_TRACK_TITLE));
	p.drawText(px.width()+2,height()-m.descent(),name);
	
	
}

void TrackTop::mousePressEvent(QMouseEvent *e) {
	
	QPixmap px = VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS );
	if (e->x()<px.width()) {
		
		automation_menu->rebuild();
		menu->popup(mapToGlobal( QPoint(0,height()) ) );
		
	} else
		rename();
}

void TrackTop::rename() {
	
	if (!can_rename)
		return;
	QString track_name = QStrify( track->get_name() );
	bool ok=false;
	
	QString res=QInputDialog::getText( this, "Rename Track", "Set Track Name:", QLineEdit::Normal, track_name, &ok);
	if (!ok)
		return;
	editor->set_track_name( editor->find_track_idx( track ), DeQStrify(res ) );

	update();
}

void TrackTop::action_slot(QAction *p_action) {
	
	if (automation_menu->check_action( p_action ))
		return; //handled by it
	
	IndexedAction *ia=dynamic_cast<IndexedAction*>(p_action);
	if (ia==NULL)
		return; //probably from the automation submenu
	
	switch (ia->get_index()) {
		
		/* for pattern tracks */
		case ACTION_PATTERN_ADD_COLUMN: {
			
			editor->track_pattern_add_column( editor->find_track_idx( track ) );
		} break;
		case ACTION_PATTERN_REMOVE_COLUMN: {
			
			editor->track_pattern_remove_column( editor->find_track_idx( track ) );
			
		} break;
		
		/* track generic */
		case ACTION_MOVE_TRACK_LEFT: {
			
			editor->move_track_left( editor->find_track_idx( track ) );
			
		} break;
		case ACTION_MOVE_TRACK_RIGHT: {
			
			editor->move_track_right( editor->find_track_idx( track ) );
			
		} break;
		
			
		case ACTION_RENAME: {
			
			rename();
		} break;
		case ACTION_DUPLICATE: {
			
			
		} break;
	
		case ACTION_DELETE: {
			
			editor->remove_track( editor->find_track_idx( track ) );
			
			
		} break;
	}
	
	
	
	
}

void TrackTop::automation_add_slot(int p_prop_idx) {

	editor->show_automation( p_prop_idx, track );

}
void TrackTop::automation_remove_slot(int p_prop_idx) {

	editor->hide_automation( p_prop_idx, track );
	
}


void TrackTop::automation_editor_requested_slot() {

	if (can_rename)
		editor->get_ui_update_notify()->automation_editor_popup(editor->find_track_idx(track));
	else
		editor->get_ui_update_notify()->automation_editor_popup(-1);
}

TrackTop::TrackTop(QWidget *p_parent,Track *p_track,Editor *p_editor,TrackType p_type) :QWidget(p_parent) {
	track=p_track;
	editor=p_editor;
	int fheight=VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS ).height();
	setFixedHeight(fheight);
	setBackgroundRole(QPalette::NoRole);
	
	menu =new QMenu("Track Options",this);
	
	if (p_type==TYPE_PATTERN) {
		menu->addAction(new IndexedAction(ACTION_PATTERN_ADD_COLUMN,"Add Column",GET_QPIXMAP(ICON_COLUMN_ADD),this));
		menu->addAction(new IndexedAction(ACTION_PATTERN_REMOVE_COLUMN,"Remove Column",GET_QPIXMAP(ICON_COLUMN_REMOVE),this));
		menu->addSeparator();
	}
	
	automation_menu=new AutomationPopup(this,track);
	QAction *amenu = menu->addMenu(automation_menu);
	amenu->setText("Automations..");
	amenu->setIcon(GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_AUTOMATIONS));
	QObject::connect(automation_menu,SIGNAL(editor_requested()),this,SLOT(automation_editor_requested_slot()));
	
	if (p_type!=TYPE_GLOBAL) {
	
	
		menu->addSeparator();
	
		menu->addAction(new IndexedAction(ACTION_MOVE_TRACK_LEFT,"Move Track Left",GET_QPIXMAP(ICON_TRACK_MOVE_LEFT),this));
		menu->addAction(new IndexedAction(ACTION_MOVE_TRACK_RIGHT,"Move Track Right",GET_QPIXMAP(ICON_TRACK_MOVE_RIGHT),this));
	
		menu->addSeparator();
	
		menu->addAction(new IndexedAction(ACTION_RENAME,"Rename Track",this));
		menu->addAction(new IndexedAction(ACTION_DUPLICATE,"Duplicate Track",GET_QPIXMAP(ICON_GLOBAL_VIEW_COPY_BLOCK),this));
		menu->addSeparator();
	
		menu->addAction(new IndexedAction(ACTION_DELETE,"Remove Track",GET_QPIXMAP(ICON_TRACK_DELETE),this));
	}
	QObject::connect(menu,SIGNAL(triggered(QAction*)),this,SLOT(action_slot( QAction* )),Qt::QueuedConnection);
	QObject::connect(automation_menu,SIGNAL(attempt_automation_add_signal( int )),this,SLOT(automation_add_slot(int)));
	QObject::connect(automation_menu,SIGNAL(attempt_automation_remove_signal( int )),this,SLOT(automation_remove_slot(int)));
	
	can_rename=p_type!=TYPE_GLOBAL;
	can_synths=p_type==TYPE_PATTERN;
			
}


TrackTop::~TrackTop()
{
}


}
