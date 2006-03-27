//
// C++ Implementation: envelope_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "envelope_editor.h"

#include "envelope_editor.h"
#include <Qt/qpainter.h>
#include <Qt/qcursor.h>
#include <Qt/qevent.h>
#include <math.h>
#include "ui_blocks/helpers.h"
#include <Qt/qtimer.h>

namespace ReShaked {


static const float ZOOM_ADJUST_MARGIN_MIN=30.0; //percent of screen
static const float ZOOM_ADJUST_MARGIN_MAX=5.0; //percent of screen

bool EnvelopeEditor::adjust_zoom_to_window() {

	float cur_width=width();
	float env_len=envelope->get_node_offset(envelope->get_node_count()-1);

	float adjust_margin_min=ZOOM_ADJUST_MARGIN_MIN*cur_width/100.0;
	float adjust_margin_max=ZOOM_ADJUST_MARGIN_MAX*cur_width/100.0;

	bool changed=false;

	float old_zoom=zoom;

	if ( (env_len/zoom) < (cur_width-adjust_margin_min) ) {

		zoom=env_len/(cur_width-adjust_margin_max);
	}

	if ( (env_len/zoom) > (cur_width-adjust_margin_max) ) {

		zoom=env_len/(cur_width-adjust_margin_min);
	}


	if (zoom<min_zoom) {
		zoom=min_zoom;
	}

	if (zoom!=old_zoom)
		changed=true;

	return changed;

}

void EnvelopeEditor::move_mouse_to_selected_node() {

//	int cur_width=width();
	int cur_height=height();
//	int real_height=height();

	if (cur_height<=0)
		return;

	float env_normalize=-envelope->get_min_value();
	float env_total=envelope->get_max_value()-envelope->get_min_value();
	if (env_total==0)
		return; //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;


	float node_x=(float)envelope->get_node_offset(grab.node);
	float node_y=(float)envelope->get_node_value(grab.node);

	node_x/=zoom;
	node_y+=env_normalize;
	node_y=node_y*(float)cur_height/env_total;
	node_y=cur_height-node_y;

	QCursor::setPos(mapToGlobal(QPoint((int)node_x,(int)node_y)));
}


bool EnvelopeEditor::event ( QEvent * e ) {

	if (envelope==NULL) return true;

//	int cur_width=width();
	int cur_height=height();
//	int real_height=height();

	if (cur_height<=0)
		return true;

	float env_normalize=-envelope->get_min_value();
	float env_total=envelope->get_max_value()-envelope->get_min_value();
	if (env_total==0)
		return false; //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;

	if (QEvent::Resize == e->type()) {
		
		if (envelope) {
			adjust_zoom_to_window();
			update();
		}
	}

	if (QEvent::MouseMove == e->type()) {
		QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(e);

		if (grab.moving_point) {

			float gx=mouseEvent->x(),gy=mouseEvent->y();
			gx*=zoom;

			gy=float(cur_height-gy)*env_total/(float)cur_height;
			gy-=env_normalize;

			if ( ((int)gx!=envelope->get_node_offset(grab.node)) || (gy!=envelope->get_node_value(grab.node)) ) {

				//avoid pointless overdraw
				AudioControl::mutex_lock();
				envelope->set_node(grab.node,(int)gx,gy);
				AudioControl::mutex_unlock();
				if (adjust_zoom_to_window())
					move_mouse_to_selected_node();
				update();
			}
			//must setup new coordinates
		}
	}

	if (QEvent::MouseButtonRelease == e->type()) {
		QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(e);

		if (grab.moving_point && (mouseEvent->button()==Qt::LeftButton)) {

			grab.moving_point=false;
			//releaseMouse();
			update();
		}
	}

	if (QEvent::MouseButtonPress == e->type()) {

		QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(e);

		float gx=mouseEvent->x(),gy=mouseEvent->y();

		// check if we grabbed anything.. distance in pixels
		int closest_node=-1;
		float closest_node_dist=1000;
		for (int i=0;i<envelope->get_node_count();i++) {


			float node_x,node_y;

			node_x=(float)envelope->get_node_offset(i);
			node_y=(float)envelope->get_node_value(i);

			node_x/=zoom;
			node_y+=env_normalize;
			node_y=node_y*(float)cur_height/env_total;
			node_y=cur_height-node_y;

			float distance=sqrt(pow(node_x-gx,2)+pow(node_y-gy,2));

			if ( (distance<closest_node_dist) && (distance<(float)MIN_DRAG_DISTANCE)) {

				closest_node_dist=distance;
				closest_node=i;
			}

		}

		switch (mouseEvent->button()) {

			case Qt::LeftButton: {

				// check the fish
				if (closest_node!=-1) {

					//convert to local coords

					grab.moving_point=true;
					grab.node=closest_node;
					grab.x=envelope->get_node_offset(grab.node);
					grab.y=envelope->get_node_value(grab.node);
					//grabMouse();
					move_mouse_to_selected_node();
				} else { //couldnt find close node, adding the point!


					gx*=zoom; // to env coordinates
					gy=((float)cur_height-gy)*env_total/(float)cur_height;
					gy-=env_normalize;


					AudioControl::mutex_lock();
					int which=envelope->add_node_at_offset((int)gx,gy);
					AudioControl::mutex_unlock();

					if (which>=0) {
						grab.moving_point=true;

						grab.node=which;
						grab.x=(int)gx;
						grab.y=(int)gy;
						//grabMouse();
						move_mouse_to_selected_node();
						update();
						envelopeChanged();

					}
				}

			} break;
			case Qt::RightButton: {

				if (closest_node!=-1) {

					AudioControl::mutex_lock();
					envelope->delete_node(closest_node);
					AudioControl::mutex_unlock();
					update();
					envelopeChanged();
					adjust_zoom_to_window();
				}
			} break;
			default: {}
		}


	}

	QWidget::event(e);
	return true;
}


void EnvelopeEditor::set_envelope(Envelope *p_envelope) {

	envelope=p_envelope;
	adjust_zoom_to_window();
	update();
}


void EnvelopeEditor::paint_envelope_points(QPainter &painter) {

	int cur_height=height();
	int cur_width=width();
	int real_height=height();

	if (cur_height<=0)
		return;

	float env_normalize=-envelope->get_min_value();
	float env_total=envelope->get_max_value()-envelope->get_min_value();

	if (envelope->is_loop_enabled()) {

		float node_begin,node_end;

		node_begin=envelope->get_node_offset( envelope->get_loop_begin() )/zoom;
		node_end=envelope->get_node_offset( envelope->get_loop_end() )/zoom;

		painter.setPen(QColor(170,170,170));

		painter.drawLine((int)node_begin,0,(int)node_begin,cur_height);
		painter.drawLine((int)node_end,0,(int)node_end,cur_height);
	}

	if (envelope->is_sustain_loop_enabled()) {

		float node_begin,node_end;

		node_begin=envelope->get_node_offset( envelope->get_sustain_loop_begin() )/zoom;
		node_end=envelope->get_node_offset( envelope->get_sustain_loop_end() )/zoom;

		painter.setPen(QColor(220,220,220));

		painter.drawLine((int)node_begin,0,(int)node_begin,cur_height);
		painter.drawLine((int)node_end,0,(int)node_end,cur_height);
	}

	painter.fillRect(0,cur_height,cur_width,real_height-cur_height,QColor(0,0,0));

	painter.setPen(skin.point_color);

	for (int i=0;i<envelope->get_node_count();i++) {


		float node_x,node_y;

		node_x=(float)envelope->get_node_offset(i)/zoom;

		node_y=(float)envelope->get_node_value(i);
		node_y+=env_normalize;
		node_y=node_y*(float)cur_height/env_total;
		node_y=cur_height-node_y;

		painter.drawRect((int)node_x-2,(int)node_y-2,6,6);
	}
	/*
	painter.setFont(display_font);
	const QFontMetrics &display_font_metrics=painter.fontMetrics();

	char buf[200];

//	painter.setPen(QColor(100, 200, 100));


	float increment=1.0;
	int step=0;

	while ((increment/zoom)<avg_string_width) {


		if (step % 2)
			increment*=2;
		else
			increment*=5;
		step++;

	}

	for (float i=0.0f;i<(cur_width*zoom);i+=increment) {

		int real_screen_offset=int((float)i/zoom);

		sprintf(buf,"%i",(int)i);
		if (i>0.0f)
			painter.drawText(real_screen_offset-display_font_metrics.width(buf)/2,cur_height+display_font_metrics.height(),buf);
	}



	if (grab.moving_point) {

		sprintf(buf,"Zoom %i%%,node %i/%i(%i,%i)",int(zoom*100.0),grab.node+1,envelope->get_node_count(),envelope->get_node_offset(grab.node),envelope->get_node_value(grab.node));
	} else {

		sprintf(buf,"Zoom %i%%, %i nodes. ",int(zoom*100.0),envelope->get_node_count());
	}


	painter.drawText(2,cur_height+3+display_font_metrics.height()*2,buf);
	*/
}

void EnvelopeEditor::draw_envelope_pos(int p_x,QPainter &painter) {

	int cur_height=height();

	ERR_FAIL_COND( cur_height<=0 );

	float env_normalize=-envelope->get_min_value();
	float env_total=envelope->get_max_value()-envelope->get_min_value();
	
	ERR_FAIL_COND( env_total==0 );
	
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;

/*	if (position_cache[p_x]) {

		painter.setPen(QColor(255,255,255));

} else {*/

	//}


	int real_x_offset=int(p_x*zoom);
	int prev_real_x_offset=int((p_x-1)*zoom);

	int line_size=1;


	float val=envelope->get_value_at_pos((float)p_x*zoom);

	if (p_x*zoom>envelope->get_node_offset(envelope->get_node_count()-1))
		return; //dont draw what it's pointless to draw

	if (lfo)
		val+=lfo->get_value((float)p_x*zoom);
	
	val+=env_normalize;
	val=val*(float)cur_height/env_total;
	val=cur_height-val;
	
	

	float prev_val=envelope->get_value_at_pos((float)(p_x-1)*zoom);
	if (lfo)
		prev_val+=lfo->get_value((float)(p_x-1)*zoom);
	
	prev_val+=env_normalize;
	prev_val=prev_val*(float)cur_height/env_total;
	prev_val=cur_height-prev_val;

	

	painter.setPen(QColor(skin.line_color.red()/2,skin.line_color.green()/2,skin.line_color.blue()/2) );
#define MIN(m_a,m_b) ( ((m_a)<(m_b))?(m_a):(m_b))
#define MAX(m_a,m_b) ( ((m_a)>(m_b))?(m_a):(m_b))
	painter.drawLine(p_x,(int)MIN(prev_val,val)-1,p_x,(int)MAX(prev_val,val)+1); //must make it better

	painter.setPen(skin.line_color);
	painter.drawLine(p_x,(int)prev_val,p_x,(int)val); //must make it better

}

void EnvelopeEditor::paintEvent( QPaintEvent * p_event ) {


	ERR_FAIL_COND(envelope==NULL);
	
	QPainter painter;
	painter.begin(this );
	
	

	if (skin.bg.isNull()) {
		
		painter.fillRect(0,0,width(),height(),QColor(0,0,0));
	} else {
		
		painter.drawPixmap(0,0,skin.bg);
	}

	painter.setClipRect(QRect(skin.border_size,skin.border_size,width()-skin.border_size*2,height()-skin.border_size*2));
	
	float env_total=envelope->get_max_value()-envelope->get_min_value();
	float env_normalize=-envelope->get_min_value();
	
	ERR_FAIL_COND( env_total==0 );
	
	int center=int(env_normalize*(float)height()/env_total);

	painter.setPen( skin.zero_color );
	
	painter.drawLine(0,center,width(),center);

	for (int i=0;i<width();i++) {

		draw_envelope_pos(i,painter);
	}

	paint_envelope_points(painter);

	char buf[100]; //sprintf is still the best :(

	QString len;
	
	int max_ofs=envelope->get_node_offset( envelope->get_node_count() -1 );
	
	if (max_ofs>=1000) {
	
		sprintf(buf,"Len %.3fs",((float)max_ofs)/1000.0);
		len=buf;
	} else {
		
		len="Len: " + QString::number(max_ofs) + "ms";
	}
	
	if (grab.moving_point ) {
		
		float x=envelope->get_node_offset( grab.node );
		float y=envelope->get_node_value( grab.node );
		
		QString mpstr;
		
		mpstr="Idx " + QString::number(grab.node) + " at ";
		
		if (x>=1000) {
	
			sprintf(buf,"%.3fs, ",((float)x)/1000.0);
			mpstr+=buf;
		} else {
		
			mpstr+=QString::number(x) + "ms, ";
		}
		
		mpstr+=QStrify( String::num( y,  envelope->get_digits() ) ) + QStrify(envelope->get_prefix())+". ";
				
		len=mpstr+len;
	} 
		
	QString str=len;
	QFont font;
	font.setPixelSize(12);
	painter.setFont(font);
	QFontMetrics fm(painter.font());
	
	int txt_x=width()-(fm.width(str)+3);
	int txt_y=fm.ascent()+3;
	
	painter.setPen( QColor(0,0,0) );
	painter.drawText(txt_x-1,txt_y-1,str);
	painter.drawText(txt_x-1,txt_y+1,str);
	painter.drawText(txt_x+1,txt_y-1,str);
	painter.drawText(txt_x+1,txt_y+1,str);
	painter.setPen( QColor(255,255,255) );
	painter.drawText(txt_x,txt_y,str);
	
}

void EnvelopeEditor::set_lfo(LFO *p_lfo) {
	
	lfo=p_lfo;
	update();
}

EnvelopeEditor::EnvelopeEditor(QWidget *p_parent,const Skin& p_skin,float p_min_zoom) : QWidget(p_parent) {

	if (!p_skin.bg.isNull()) {
		
		setFixedSize(p_skin.bg.size());
	}
	
	skin=p_skin;
	
	envelope=NULL;
	
	min_zoom=p_min_zoom;
	setBackgroundRole(QPalette::NoRole);

	envelope=NULL;
	zoom=0.25f;

	//display_font.setFamily("fixed");
	//display_font.setPointSize(12);
	//display_font.setBold(true);
	//display_font.setStyleHint(QFont::Courier,QFont::NoAntialias);



	grab.moving_point=false;
	grab.node=-1;
	grab.x=0;
	grab.y=0;

	
	lfo=NULL;
	
}

EnvelopeEditor::~EnvelopeEditor()
{
}

}
