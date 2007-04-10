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


#include "base/painter.h"
#include "gui_custom/rsskin.h"
namespace ReShaked {

void LoopColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	
	
}

Size LoopColumn::get_minimum_size_internal() {
	
	return Size( constant( C_GLOBAL_VIEW_LOOP_COLUMN_WIDTH ), 0 );
}

void LoopColumn::numeric_input_dialog_beat(double p_val) {
	
	editor->bar_length_set( beat_at_click,(int)p_val);
				
	
	redraw_global_view_signal.call();
}

void LoopColumn::option_selected(int p_option) {
	
	int beat=beat_at_click;
	
	switch (p_option) {
		
		case OPTION_SET_LOOP_BEGIN: {
			editor->get_song()->set_loop_begin( beat );
		} break;
		case OPTION_SET_LOOP_END: {
			editor->get_song()->set_loop_end( beat );
		} break;
		case OPTION_CHANGE_BAR_LENGTH: {
					
			int current=editor->get_song()->get_bar_map().get_bar_idx( editor->get_song()->get_bar_map().get_bar_idx_from_beat( beat ) );
						
			input_dialog->show("Enter Bar Length (Beats):",2,16,1,current);
			
		} break;
		case OPTION_REMOVE_BAR_LENGTH: {
			
			editor->bar_length_remove( beat);
								
			
		} break;
	}
		
	update();
	
}
void LoopColumn::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	if (!p_press || p_button!=BUTTON_LEFT)
		return;
	
	beat_at_click=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);

	menu->clear();
	menu->add_item("Set Loop Begin",OPTION_SET_LOOP_BEGIN);
	menu->add_item("Set Loop End",OPTION_SET_LOOP_END);
	menu->add_separator();
	menu->add_item("Change Bar Length",OPTION_CHANGE_BAR_LENGTH);
	menu->add_item("Remove Bar Length",OPTION_REMOVE_BAR_LENGTH);
		
	menu->popup( get_global_pos() + p_pos );
	
}

void LoopColumn::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	
	Painter &p=*get_painter();
	p.draw_fill_rect(Point(), p_size,color(COLOR_GLOBAL_VIEW_BG));
	
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(p_size.height);
	
	FontID barfont=font( FONT_GLOBAL_VIEW_BAR );
	FontID beatfont=font( FONT_GLOBAL_VIEW_BEAT );
	Color barcol=color( COLOR_GLOBAL_VIEW_BAR_FONT );
	Color beatcol=color( COLOR_GLOBAL_VIEW_BEAT_FONT );
	
	int begin_pixel=global_view->get_beat_pixel( editor->get_song()->get_loop_begin() );
	int end_pixel=global_view->get_beat_pixel( editor->get_song()->get_loop_end() );
	
	p.draw_fill_rect(Point(0,begin_pixel),Size(p_size.width,end_pixel-begin_pixel),color( COLOR_GLOBAL_VIEW_LOOP ));
		
	
	p.draw_arrow( Point(0,begin_pixel), Size(p_size.width,p_size.width/2),DOWN,color( COLOR_GLOBAL_VIEW_LOOP_MARKER ),false);
	
	p.draw_arrow( Point(0,end_pixel-p_size.width/2), Size(p_size.width,p_size.width/2),UP,color( COLOR_GLOBAL_VIEW_LOOP_MARKER ),false);
	
	if (inside) {
			
		int h=global_view->get_beat_pixel( inside_beat );
		p.draw_fill_rect(Point(0,h),Size(p_size.width,1),color(COLOR_GLOBAL_VIEW_MOUSE_OVER_BEAT));
	}
	
	for (int i=0;i<editor->get_song()->get_bar_map().get_bar_idx_count();i++) {
		
		int beat=editor->get_song()->get_bar_map().get_bar_idx_pos(i);
		if (beat<beat_from || beat>beat_to)
			continue;
		
		
		
		int h=global_view->get_beat_pixel( beat );
		p.draw_fill_rect(Point(0,h),Size(p_size.width,1),color(COLOR_GLOBAL_VIEW_BARBEAT_CHANGE));
		
	}
	
	int line_pos=p_size.width-1;
	p.draw_fill_rect(Point(0,line_pos),Size(1,p_size.height),color(COLOR_GLOBAL_VIEW_SEPARATOR));
}

void LoopColumn::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {
	
	inside_beat=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);

	inside=true;
	update();
}

void LoopColumn::mouse_leave() {
	
	inside=false;
	update();
	
}

void LoopColumn::set_in_window() {

	input_dialog = new NumericInputDialog( get_window() );
	input_dialog->entered_number_signal.connect( this, &LoopColumn::numeric_input_dialog_beat );
	menu = new PopUpMenu(get_window());
	
	
	menu->selected_id_signal.connect( this, &LoopColumn::option_selected );
}
LoopColumn::LoopColumn(Editor *p_editor)  {
	
	editor=p_editor;
	
	set_bg_on_updates(false);
	//setToolTip("Loop, Bar Length");
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

	input_dialog=0;
}

LoopColumn::~LoopColumn() {
	
	if (input_dialog)
		delete input_dialog;
}
/***/
/*
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
	
	
	
	// Draw Arrow 
	p.setPen(QColor(150,150,150));
	p.setBrush(QColor(150,150,150));
	QPolygon arrow;
	arrow.push_back(QPoint(0,pos-beat_h/2));
	arrow.push_back(QPoint(0,pos+beat_h/2));
	arrow.push_back(QPoint(m.descent(),pos));
	
	p.drawPolygon(arrow,Qt::WindingFill);
	
	// Draw Text 
	
	
	
	p.setFont(f);
	p.rotate(90.0); //90 degrees!
	p.setPen(QColor(255,255,255));
	p.drawText(pos+beat_h,-m.descent(), QStrify( marker_text ) );
	
	p.restore();
	
}

void MarkerColumn::mousePressEvent(QMouseEvent *e) {
	
	int beat=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);
	int marker_idx=marker_list->get_exact_index( beat );
	
	if (p_button==BUTTON_RIGHT || (p_button==BUTTON_LEFT && e->modifiers()&Qt::ControlModifier)) {
		
		if (marker_idx!=INVALID_STREAM_INDEX) {
			
			//printf("size %i\n",marker_list->get_stream_size());
			editor->marker_remove(marker_idx);
			//printf("removed %i, size %i\n",marker_idx,marker_list->get_stream_size());
			update();
		}
		return;
	}
	
	if (p_button==BUTTON_LEFT) {
		
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
		
		
		// DRAW MARK 
					
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
	
	// Paint markers that may not be visible 
	if (marker_list->get_exact_index( beat_from )==INVALID_STREAM_INDEX && marker_list->get_prev_index( beat_from )>=0) {
		
		paint_marker(p, marker_list->get_prev_index( beat_from ) );
		
	}
	
	p.setPen(QColor(150,150,150));
	int line_pos=width()-1;
	p.drawLine(line_pos,0,line_pos,height());
}

void MarkerColumn::mouseMoveEvent(QMouseEvent *e) {
	
	inside_beat=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);

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
		
		// DRAW BEAT and optionally BAR *
		
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

*/
}
