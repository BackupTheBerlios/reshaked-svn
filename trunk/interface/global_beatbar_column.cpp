//
// C++ Implementation: global_beatbar_column
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_beatbar_column.h"
#include "visual_settings.h"
#include "ui_blocks/helpers.h"
#include <Qt/qpainter.h>
#include <Qt/qinputdialog.h>
#include <Qt/qevent.h>
#include <Qt/qpointer.h>
#include <Qt/qaction.h>
#include <Qt/qmenu.h>

namespace ReShaked {

void LoopColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	QObject::connect(global_view,SIGNAL(drawing_signal()),this,SLOT(update()));
	
}

void LoopColumn::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton) 
		return;
	
	int beat=global_view->get_beat_at_pixel(e->y()+global_view->get_beat_pixel_size()/2);

	
	QList<QAction*> ac_list;
	QAction *op_1 = new QAction(GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_BEGIN),"Set Loop Begin",this);
	op_1->setData("begin");
	ac_list.push_back(op_1);
	QAction *op_2 = new QAction(GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_END),"Set Loop End",this);
	op_2->setData("end");
	ac_list.push_back(op_2);
	QAction *op_3 = new QAction("Change Bar Length",this);
	op_3->setData("beatsbar");
	ac_list.push_back(op_3);
	
	QAction *op_4 = new QAction("Remove Bar Length",this);
	op_4->setData("beatsbar_remove");
	if (editor->get_song()->get_bar_map().get_bar_idx_at_beat(beat)>=0 && beat>0)
		ac_list.push_back(op_4);
	
	QAction *res = QMenu::exec(ac_list,mapToGlobal(e->pos()));
	
	if (res!=NULL) {
		
		if (res->data().toString()=="begin") {
			
			editor->get_song()->set_loop_begin( beat );
		} else if (res->data().toString()=="end") {
			editor->get_song()->set_loop_end( beat );
	
		} else if (res->data().toString()=="beatsbar") { 
			
			bool ok;
			int current=editor->get_song()->get_bar_map().get_bar_idx( editor->get_song()->get_bar_map().get_bar_idx_from_beat( beat ) );
			int bbar=QInputDialog::getInteger ( this, "Change Bar Length", "Set Bar Length: (beats)", current,2,16,1,&ok);
			
			if (ok) {
				
				editor->bar_length_set( beat,bbar);
				
			}
			
		} else if (res->data().toString()=="beatsbar_remove") { 
			
			editor->bar_length_remove( beat);
			
		}
	}
	delete op_1;
	delete op_2;
	delete op_3;
	
}

void LoopColumn::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(height());
	
	PixmapFont *pf=VisualSettings::get_singleton()->get_global_bar_font();
	PixmapFont *bf=VisualSettings::get_singleton()->get_global_beat_font();
	
	
	int begin_pixel=global_view->get_beat_pixel( editor->get_song()->get_loop_begin() );
	int end_pixel=global_view->get_beat_pixel( editor->get_song()->get_loop_end() );
	p.fillRect(0,begin_pixel,width(),end_pixel-begin_pixel,QColor(50,50,50));
		
	
	p.drawPixmap(0,begin_pixel,GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_BEGIN));
	p.drawPixmap(0,end_pixel-GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_END).height(),GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_END));
	
	if (inside) {
			
		p.setPen(QColor(20,180,100));
		int h=global_view->get_beat_pixel( inside_beat );
		p.drawLine(0,h,width(),h);
	}
	
	for (int i=0;i<editor->get_song()->get_bar_map().get_bar_idx_count();i++) {
		
		int beat=editor->get_song()->get_bar_map().get_bar_idx_pos(i);
		if (beat<beat_from || beat>beat_to)
			continue;
		p.setPen(QColor(180,120,100));
		int h=global_view->get_beat_pixel( beat );
		p.drawLine(0,h,width(),h);
		
	}
	
	
	p.setPen(QColor(150,150,150));
	int line_pos=width()-1;
	p.drawLine(line_pos,0,line_pos,height());
}

void LoopColumn::mouseMoveEvent(QMouseEvent *e) {
	
	inside_beat=global_view->get_beat_at_pixel(e->y()+global_view->get_beat_pixel_size()/2);

	inside=true;
	update();
}
void LoopColumn::enterEvent(QEvent *ee) {
	
}
void LoopColumn::leaveEvent(QEvent *ee) {
	
	inside=false;
	update();
	
}


LoopColumn::LoopColumn(QWidget *p_parent,Editor *p_editor) : QWidget(p_parent) {
	
	editor=p_editor;
	setFixedWidth(GET_QPIXMAP(ICON_EDIT_VIEW_ROW_PLAYED).width());
	setBackgroundRole(QPalette::NoRole);
	setMouseTracking(true);
	setToolTip("Loop, Bar Length");
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

}


/***/

void MarkerColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	QObject::connect(global_view,SIGNAL(drawing_signal()),this,SLOT(update()));
	
}

void MarkerColumn::paint_marker(QPainter& p, int p_marker_idx) {
	
	String marker_text=marker_list->get_index_value(p_marker_idx);
	int beat=marker_list->get_index_pos(p_marker_idx);
	int fontwidth=VisualSettings::get_singleton()->get_global_bar_font()->get_width();
	int pos=global_view->get_beat_pixel( beat );
	int beat_h=global_view->get_beat_pixel( beat+1 )-pos;
	
	p.save();
	
	QFont f;
	f.setPixelSize(fontwidth*2);
	QFontMetrics m(f);
	
	
	
	/* Draw Arrow */
	p.setPen(QColor(150,150,150));
	p.setBrush(QColor(150,150,150));
	QPolygon arrow;
	arrow.push_back(QPoint(0,pos-beat_h/2));
	arrow.push_back(QPoint(0,pos+beat_h/2));
	arrow.push_back(QPoint(m.descent(),pos));
	
	p.drawPolygon(arrow,Qt::WindingFill);
	
	/* Draw Text */
	
	
	
	p.setFont(f);
	p.rotate(90.0); //90 degrees!
	p.setPen(QColor(255,255,255));
	p.drawText(pos+beat_h,-m.descent(), QStrify( marker_text ) );
	
	p.restore();
	
}

void MarkerColumn::mousePressEvent(QMouseEvent *e) {
	
	int beat=global_view->get_beat_at_pixel(e->y()+global_view->get_beat_pixel_size()/2);
	int marker_idx=marker_list->get_exact_index( beat );
	
	if (e->button()==Qt::RightButton || (e->button()==Qt::LeftButton && e->modifiers()&Qt::ControlModifier)) {
		
		if (marker_idx!=INVALID_STREAM_INDEX) {
			
			printf("size %i\n",marker_list->get_stream_size());
			editor->marker_remove(marker_idx);
			printf("removed %i, size %i\n",marker_idx,marker_list->get_stream_size());
			update();
		}
		return;
	}
	
	if (e->button()==Qt::LeftButton) {
		
		QString current_text=QStrify( (marker_idx>=0)?marker_list->get_index_value_w( marker_idx):"" );
		QString text=QInputDialog::getText ( this,"Insert Marker", "Marker Text:", QLineEdit::Normal, current_text);
		if (text=="")
			return;
		
		editor->marker_set(beat,DeQStrify(text));
		update();
	}
	
	
}

void MarkerColumn::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(height());
	
	PixmapFont *pf=VisualSettings::get_singleton()->get_global_bar_font();
	PixmapFont *bf=VisualSettings::get_singleton()->get_global_beat_font();
	
	
		
	
	for (int i=beat_from;i<=beat_to;i++) {
		
		
		/* DRAW MARK */
					
		if (i==inside_beat && inside) {
			
			p.setPen(QColor(20,180,100));
			int h=global_view->get_beat_pixel( i );
			p.drawLine(0,h,width(),h);
		}
		
		int marker_idx=marker_list->get_exact_index( i );
		if (marker_idx>=0) { //marker at pos
			paint_marker(p,marker_idx);
			
		}
		
		
	}
	
	/* Paint markers that may not be visible */
	if (marker_list->get_exact_index( beat_from )==INVALID_STREAM_INDEX && marker_list->get_prev_index( beat_from )>=0) {
		
		paint_marker(p, marker_list->get_prev_index( beat_from ) );
		
	}
	
	p.setPen(QColor(150,150,150));
	int line_pos=width()-1;
	p.drawLine(line_pos,0,line_pos,height());
}

void MarkerColumn::mouseMoveEvent(QMouseEvent *e) {
	
	inside_beat=global_view->get_beat_at_pixel(e->y()+global_view->get_beat_pixel_size()/2);

	inside=true;
	update();
}
void MarkerColumn::enterEvent(QEvent *ee) {
	
}
void MarkerColumn::leaveEvent(QEvent *ee) {
	
	inside=false;
	update();
	
}


MarkerColumn::MarkerColumn(QWidget *p_parent,Editor *p_editor) : QWidget(p_parent) {
	
	editor=p_editor;
	bar_map=&p_editor->get_song()->get_bar_map();
	marker_list = &p_editor->get_song()->get_marker_list();
	setFixedWidth(VisualSettings::get_singleton()->get_global_bar_font()->get_width()*3);
	setBackgroundRole(QPalette::NoRole);
	setMouseTracking(true);
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
	setToolTip("Markers");

}



void GlobalBeatBarColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	QObject::connect(global_view,SIGNAL(drawing_signal()),this,SLOT(update()));
	
}

void GlobalBeatBarColumn::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(height());
	
	PixmapFont *pf=VisualSettings::get_singleton()->get_global_bar_font();
	PixmapFont *bf=VisualSettings::get_singleton()->get_global_beat_font();
	
	
		
	
	for (int i=beat_from;i<=beat_to;i++) {
		
		/* DRAW BEAT and optionally BAR */
		
		if (global_view->get_beat_pixel_size()>bf->get_height()) { //paint beat

				
			QString s = QString::number( bar_map->get_bar_beat( i)+1 );
			int ofs=width()-(s.length()+1)*bf->get_width();
			bf->render_string( p, ofs, global_view->get_beat_pixel( i ), s.toAscii().data() );
		}
		
		if (!bar_map->get_bar_beat( i )) { // paint bar
			
			QString s = QString::number(bar_map->get_bar_at_beat( i)+1 );
			int ofs=width()-(s.length()+3)*pf->get_width();
			pf->render_string( p, ofs, global_view->get_beat_pixel( i ), s.toAscii().data() );
		}
		
			
	}
	
	
	p.setPen(QColor(150,150,150));
	int line_pos=width()-bf->get_width()*5/2;
	p.drawLine(line_pos,0,line_pos,height());
}

GlobalBeatBarColumn::GlobalBeatBarColumn(QWidget *p_parent,Editor *p_editor) : QWidget(p_parent) {
	
	editor=p_editor;
	bar_map=&p_editor->get_song()->get_bar_map();
	setFixedWidth(VisualSettings::get_singleton()->get_global_bar_font()->get_width()*9);
	setBackgroundRole(QPalette::NoRole);
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
	setToolTip("Bar/Beat");

}


GlobalBeatBarColumn::~GlobalBeatBarColumn()
{
}


}
