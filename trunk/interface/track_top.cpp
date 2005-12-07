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

namespace ReShaked {

void TrackTop::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	QPixmap px = VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS );
	p.drawPixmap(0,0,px);
	
	if (!track)
		return;
	QFont f;
	f.setPixelSize(height()-4);
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
		
	p.setPen(QColor(200,255,200));
	p.drawText(px.width()+2,m.ascent(),name);
	
	
}

void TrackTop::mousePressEvent(QMouseEvent *e) {
	
	QPixmap px = VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS );
	if (e->x()<px.width())
		menu->popup(mapToGlobal( QPoint(0,0) ) );
	else
		rename();
}

void TrackTop::rename() {
	
	QString track_name = QStrify( track->get_name() );
	bool ok=false;
	
	QString res=QInputDialog::getText( this, "Rename Track", "Set Track Name:", QLineEdit::Normal, track_name, &ok);
	if (!ok)
		return;
	editor->set_track_name( editor->find_Track( track ), DeQStrify(res ) );

	update();
}

void TrackTop::action_slot(QAction *p_action) {
	
	IndexedAction *ia=dynamic_cast<IndexedAction*>(p_action);
	ERR_FAIL_COND(ia==NULL);
	
	switch (ia->get_index()) {
		
		/* for pattern tracks */
		case ACTION_PATTERN_ADD_COLUMN: {
			
			editor->track_pattern_add_column( editor->find_Track( track ) );
		} break;
		case ACTION_PATTERN_REMOVE_COLUMN: {
			
			editor->track_pattern_remove_column( editor->find_Track( track ) );
			
		} break;
		
		/* track generic */
		case ACTION_MOVE_TRACK_LEFT: {
			
			
		} break;
		case ACTION_MOVE_TRACK_RIGHT: {
			
			
		} break;
		
		case ACTION_RENAME: {
			
			rename();
		} break;
		case ACTION_DUPLICATE: {
			
			
		} break;
	
		case ACTION_DELETE: {
			
			editor->remove_track( editor->find_Track( track ) );
			
			
		} break;
	}
	
}

TrackTop::TrackTop(QWidget *p_parent,Track *p_track,Editor *p_editor) :QWidget(p_parent) {
	track=p_track;
	editor=p_editor;
	int wheight=VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS ).height();
	setFixedHeight(wheight);
	setBackgroundRole(QPalette::NoRole);
	
	menu =new QMenu("Track Options",this);
	menu->addAction(new IndexedAction(ACTION_PATTERN_ADD_COLUMN,"Add Column",GET_QPIXMAP(ICON_COLUMN_ADD),this));
	menu->addAction(new IndexedAction(ACTION_PATTERN_REMOVE_COLUMN,"Remove Column",GET_QPIXMAP(ICON_COLUMN_REMOVE),this));
	menu->addSeparator();
	menu->addAction(new IndexedAction(ACTION_MOVE_TRACK_LEFT,"Move Track Left",GET_QPIXMAP(ICON_TRACK_MOVE_LEFT),this));
	menu->addAction(new IndexedAction(ACTION_MOVE_TRACK_RIGHT,"Move Track Right",GET_QPIXMAP(ICON_TRACK_MOVE_RIGHT),this));
	menu->addSeparator();
	
	menu->addAction(new IndexedAction(ACTION_RENAME,"Rename Track",this));
	menu->addAction(new IndexedAction(ACTION_DUPLICATE,"Duplicate Track",GET_QPIXMAP(ICON_GLOBAL_VIEW_COPY_BLOCK),this));
	menu->addSeparator();
	
	menu->addAction(new IndexedAction(ACTION_DELETE,"Remove Track",GET_QPIXMAP(ICON_TRACK_DELETE),this));
	
	QObject::connect(menu,SIGNAL(triggered(QAction*)),this,SLOT(action_slot( QAction* )),Qt::QueuedConnection);
			
}


TrackTop::~TrackTop()
{
}


}
