/**************************f*************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_viewer.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include <math.h>

namespace ReShaked {

bool SampleViewer::colors_initialized;

int SampleViewer::color_list[Col_Max*3] = {

		0,0,0,
		180,190,200,
		255,255,255,
		255,255,255,
		0,0,0,
		255,0,0,

		70,120,190,
		77,77,77,
		0,0,0,
		0,0,0,
		255,255,255,
		77,99,124
};

QColor SampleViewer::colors[Col_Max];

void SampleViewer::init_colormap() {

	if (colors_initialized)
		return;

	int i;

	for (i=0;i<Col_Max*3;i+=3) {

		colors[i/3].setRgb(color_list[i],color_list[i+1],color_list[i+2]);
	}

	colors_initialized=true;
}

QColor * SampleViewer::get_color(Colors p_color) {

	if (p_color==Col_Max)
		return NULL;
	return &colors[p_color];
}



int SampleViewer::screen_pos_to_sample_pos(int p_pos) {

	float pos=p_pos;

	pos=(double)pos*((double)sample_data->get_length()*zoom)/(double)width();
	pos+=(float)offset;

	return (int)pos;

}

bool SampleViewer::event ( QEvent * e ) {

	if (!sample_data)
		return QWidget::event(e);;
	switch (e->type()) {

		case QEvent::MouseMove: {

			if (sample_data->get_length()==0)
				break;
			if (!selection)
				break;

			QMouseEvent * m = static_cast<QMouseEvent *>(e);

			if (grabbing_mouse) {

				switch( grab_type ){

					case SET_SELECTION: {

						selection->set_point_2( screen_pos_to_sample_pos( m->x() ) );

					} break;
					case SET_SELECTION_BEGIN:
					case SET_SELECTION_END: {

						if (closer_point==1)
							selection->set_point_1( screen_pos_to_sample_pos( m->x() ) );
						else
							selection->set_point_2( screen_pos_to_sample_pos( m->x() ) );

					} break;
				}

				update();
			}
		} break;

		case QEvent::MouseButtonRelease: {
			if (sample_data->get_length()==0)
				break;
			if (!selection)
				break;

			QMouseEvent * m = static_cast<QMouseEvent *>(e);

                        if (grabbing_mouse) {

				bool ungrab=false;

				switch(grab_type) {

					case SET_SELECTION: {

						if (m->button()==Qt::LeftButton)
							ungrab=true;

					} break;
					case SET_SELECTION_BEGIN: {

						if (m->button()==Qt::MidButton)
							ungrab=true;



					} break;
					case SET_SELECTION_END: {

						if (m->button()==Qt::RightButton)
							ungrab=true;

					} break;
				}

				if (ungrab) {


					grabbing_mouse=false;
					if (selection->get_begin()==selection->get_end())
						selection->set_active(false);
					update();

				}

			}


		} break;


		case QEvent::MouseButtonPress: {
			if (!selection)
				break;

			if (sample_data->get_length()==0)
				break;

			if (grabbing_mouse)
				break; //ignore it

			update();
			QMouseEvent * m = static_cast<QMouseEvent *>(e);

			switch (m->button()) {

				case Qt::LeftButton: {

					grab_type=SET_SELECTION;
					selection->clear();
					selection->set_active(true);
					selection->set_max( sample_data->get_length() );
					selection->set_point_1( screen_pos_to_sample_pos( m->x() ) );
					selection->set_point_2( screen_pos_to_sample_pos( m->x() ) );
					grabbing_mouse=true;



				} break;
				case Qt::RightButton: {

					grab_type=SET_SELECTION_END;

					if (!selection->is_active())
						break;
					grabbing_mouse=true;

					closer_point=(selection->get_point_1()>selection->get_point_2())?1:2;

				} break;
				case Qt::MidButton: {
					grab_type=SET_SELECTION_BEGIN;
					if (!selection->is_active())
						break;
					grabbing_mouse=true;

					closer_point=(selection->get_point_1()<selection->get_point_2())?1:2;
				} break;

                        	default: {}
			}
			
		} break;

		default: {

		};

	}

	return QWidget::event(e);
}




void SampleViewer::screen_to_sample(int p_screen_sample, FrameData *p_max_peak,FrameData *p_min_peak, int sample_cache_idx,int cur_width,int *p_final_sample) {

	int sample_idx;
	int size=sample_data->get_length();

	sample_idx=lrint((double)p_screen_sample*((double)size*zoom)/(double)cur_width);
	sample_idx+=offset;
	*p_final_sample=sample_idx;
	
	
	if (sample_data->get_peak_cache().size()*zoom < (float)cur_width ) { //CANT use peakcache!
	
		int subsamples=lrint((double)size*zoom/(double)cur_width);
		if (subsamples<2)
			subsamples=1;
		else
			subsamples+=1;
		
		for (int ch=0;ch<sample_data->get_channels();ch++) {
			float max_peak=-2.0;
			float min_peak=2.0; //impossible values :)
			//if (p_screen_sample==5)
			//	printf("READING RAW\n");
			
			for (int i=sample_idx;i<(sample_idx+subsamples);i++) {
				
				if (i>=size)
					break;
				
				float samp=sample_data->get_frame(i).get(ch);
				if (samp>max_peak)
					max_peak=samp;
				if (samp<min_peak)
					min_peak=samp;
				
			}
			
			p_max_peak->set(ch,max_peak);
			p_min_peak->set(ch,min_peak);
		}
		
		
		
	} else { // CAN use peakcache
		
		int pk_sample_idx=lrint((double)p_screen_sample*((double)sample_data->get_peak_cache().size()*zoom)/(double)cur_width);
		pk_sample_idx+=lrint((double)offset*(double)sample_data->get_peak_cache().size()/(double)size);
		
		int subsamples=lrint((double)sample_data->get_peak_cache().size()*zoom/(double)cur_width);
		if (subsamples<2)
			subsamples=1;
		else
			subsamples+=1;
		
		for (int ch=0;ch<sample_data->get_channels();ch++) {
		
			float max_peak=-2.0;
			float min_peak=2.0; //impossible values :)
			//if (p_screen_sample==5)
			//	printf("READING CACHED\n");
			
			
			for (int i=pk_sample_idx;i<(pk_sample_idx+subsamples);i++) {
				
				if (i>=sample_data->get_peak_cache().size())
					break;
				
				
				if (sample_data->get_peak_cache()[i].max.get(ch)>max_peak)
					max_peak=sample_data->get_peak_cache()[i].max.get(ch);
				if (sample_data->get_peak_cache()[i].min.get(ch)<min_peak)
					min_peak=sample_data->get_peak_cache()[i].min.get(ch);
				
			}
			
			p_max_peak->set(ch,max_peak);
			p_min_peak->set(ch,min_peak);
		}
			
	}
}

float SampleViewer::get_zoom() {

	return zoom;
}
void SampleViewer::set_zoom(float p_factor) {

	float zoom_min=0.0001;
        if (p_factor<zoom_min) //sucks
		p_factor=zoom_min;
	if (p_factor>1.0f)
		p_factor=1.0f;

	zoom=p_factor;


	update();
}

int SampleViewer::get_zoom_offset() {

	return offset;
}
void SampleViewer::set_zoom_offset(int p_offset) {

	offset=p_offset;
	if (offset<0) offset=0;
	if (offset>get_max_zoom_offset())
		offset=get_max_zoom_offset();

	update();
}
int SampleViewer::get_max_zoom_offset() {

	return sample_data?sample_data->get_length()- int ((double)sample_data->get_length()*(double)zoom):0;
}


void SampleViewer::draw_screen_pos(int p_x,QPainter &painter,int factor) {

	int cur_width=width();
//	int cur_height=height();

	FrameData prev_min_peak( sample_data->get_channels() );
	FrameData prev_max_peak( sample_data->get_channels() );

	FrameData min_peak( sample_data->get_channels() );
	FrameData max_peak( sample_data->get_channels() );
	int current_sample_idx;
	int next_sample_idx;

	screen_to_sample(p_x,&min_peak,&max_peak,factor,cur_width,&current_sample_idx);

	min_peak+=1.0f;
	min_peak/=2.0f;
	max_peak+=1.0f;
	max_peak/=2.0f ;

	if (p_x>0) {

		screen_to_sample(p_x-1,&prev_min_peak,&prev_max_peak,factor,cur_width,&current_sample_idx);

		prev_min_peak+=1.0f;
		prev_min_peak/=2.0f;
		prev_max_peak+=1.0f;
		prev_max_peak/=2.0f ;

	} else {
		prev_max_peak=FrameData(sample_data->get_channels())+0.5;
		prev_min_peak=FrameData(sample_data->get_channels())+0.5;
	}

	/* Check for loops */

	next_sample_idx=int ( (double)(p_x+1)*((double)sample_data->get_length()*zoom)/(double)cur_width );
	next_sample_idx+=offset;


        QColor bg_col;
        QColor fg_col;

	bool selected=false;
	if (selection) selected=selection->is_in_range(current_sample_idx);


	if (
		(
			(current_sample_idx<=sample_data->get_loop_begin() && next_sample_idx>=sample_data->get_loop_begin()) ||
			(current_sample_idx<=sample_data->get_loop_end() && next_sample_idx>=sample_data->get_loop_end())
		)
		) {

                bg_col=selected?colors[Col_Loop_BackGround_Selected]:colors[Col_Loop_BackGround];
                fg_col=selected?colors[Col_Loop_ForeGround_Selected]:colors[Col_Loop_ForeGround];

	} else {

                bg_col=selected?colors[Col_BackGround_Selected]:colors[Col_BackGround];
		fg_col=selected?colors[Col_ForeGround_Selected]:colors[Col_ForeGround];

	}

	painter.setPen(bg_col);
	painter.drawLine(p_x,0,p_x,height());
	
	for (int i=0;i<sample_data->get_channels();i++) {
		
		int cheight=height()/sample_data->get_channels();
		int cofs=i*cheight;
		
		int min_px=lrintf(min_peak.get(i)*(float)cheight);
		int prev_min_px=lrintf(prev_min_peak.get(i)*(float)cheight);
		
		min_px=(min_px>(prev_min_px+1))?prev_min_px:min_px;
		min_px+=cofs;		
		int max_px=lrintf(max_peak.get(i)*(float)cheight);
		int prev_max_px=lrintf(prev_max_peak.get(i)*(float)cheight);
		
		max_px=(max_px>(prev_max_px+1))?prev_max_px:max_px;
		max_px+=cofs;		

		painter.setPen(fg_col);
		painter.drawLine(p_x,min_px,p_x,max_px);

		painter.setPen(selected?colors[Col_Center_Selected]:colors[Col_Center]);
		painter.drawPoint(p_x,cofs+cheight/2);
	}

}

void SampleViewer::paintEvent( QPaintEvent * p_event ) {

	if ((sample_data==NULL) || (sample_data->get_length()==0)) {

		QPainter painter(this);
		painter.fillRect(0,0,width(),height(),QColor(0,0,0));
		return;
	}
	
	if (sample_data->is_peak_cache_dirty())
		sample_data->generate_peak_cache();
		
	

	int cur_width=width();
	int cur_height=height();




	QPainter painter(this);


	painter.setPen(QColor(0,0,200));


	int factor=1;

	for (int i=0;i<cur_width;i++) {

		draw_screen_pos(i,painter,factor);
	}

	//bitBlt(this,0,0,backing_store);


   //     p.drawText( 100,100, "hohohoho" );
}

//QSizePolicy SampleViewer::sizePolicy() const {


//}

void SampleViewer::set_sample_data(Sample *p_sample_Data) {

	sample_data=p_sample_Data;
	zoom=1; //1:1
	offset=0;

	update();
}


SampleViewer::SampleViewer( QWidget *p_parent) : QWidget( p_parent ) {

	sample_data=NULL;
	zoom=1; //1:1
	offset=0;

	setBackgroundRole(QPalette::NoRole);
	
	selection=NULL;
	grabbing_mouse=false;
	init_colormap();
	grab_type=SET_SELECTION;

	
}


SampleViewer::~SampleViewer()
{


}

}
