/***************************************************************************
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
#include "sample_viewer_zoom.h"
#include "pixmaps/zoom_all.xpm"
#include "pixmaps/zoom_minus.xpm"
#include "pixmaps/zoom_plus.xpm"
#include "pixmaps/zoom_selection.xpm"

#include <math.h>

namespace ReShaked {

void SampleViewerZoom::update_zoom_bar() {

	if (!sample_viewer || !sample_data)
		return;
	int zoom_offset=sample_viewer->get_zoom_offset();
	int max_zoom_offset=sample_viewer->get_max_zoom_offset();
	int sample_data_size=sample_data?sample_data->get_length():0;
	int pagestep=sample_data_size-max_zoom_offset;

//	//printf("zoom offset %i, max zoom offset %i, pagestep %i, sdatasize %i\n",zoom_offset,max_zoom_offset,pagestep,sample_data_size);

	zoom_view_offset->setMaximum(max_zoom_offset);
	zoom_view_offset->setPageStep(pagestep);
	zoom_view_offset->setMinimum(0);
	zoom_view_offset->setValue(zoom_offset);

	zoom_slider->setValue(lrint(sample_viewer->get_zoom()*100.0));

	char buf[40];
	sprintf(buf," %.2f%%",sample_viewer->get_zoom()*100.0f);
	zoom_slider_label->setText(buf);

}

void SampleViewerZoom::zoom_slider_cbk(int p_val) {
	if (!sample_viewer || !sample_data)
		return;

	if (updating_zoom) return;
	
	updating_zoom=true;
	float zoom_percent=p_val;
	zoom_percent/=100.0;

	float old_zoom=sample_viewer->get_zoom();
	float new_zoom=zoom_percent;

	float offset=sample_viewer->get_zoom_offset();
	float old_screen_in_samples=(float)sample_data->get_length()*old_zoom;
	float new_screen_in_samples=(float)sample_data->get_length()*new_zoom;
	float center=offset+(old_screen_in_samples/2.0f);
	offset=center-(new_screen_in_samples/2.0f);

	sample_viewer->set_zoom(new_zoom);
	sample_viewer->set_zoom_offset((int)offset);

	update_zoom_bar();
	updating_zoom=false;

}

void SampleViewerZoom::zoom_more_cbk() {
	if (!sample_viewer || !sample_data)
		return;

	if (!sample_data)
		return;
	if (sample_data->get_length()==0)
		return;

	updating_zoom=true;

	float old_zoom=sample_viewer->get_zoom();
	float new_zoom=old_zoom/2.0f;
	float offset=sample_viewer->get_zoom_offset();
	float old_screen_in_samples=(float)sample_data->get_length()*old_zoom;
	float new_screen_in_samples=(float)sample_data->get_length()*new_zoom;
	float center=offset+(old_screen_in_samples/2.0f);
	offset=center-(new_screen_in_samples/2.0f);

	sample_viewer->set_zoom(new_zoom);
	sample_viewer->set_zoom_offset((int)offset);
	zoom_slider->setValue(int(sample_viewer->get_zoom()*100.0));
	update_zoom_bar();
	updating_zoom=false;
}

void SampleViewerZoom::zoom_less_cbk() {
	if (!sample_viewer || !sample_data)
		return;

	updating_zoom=true;

	float old_zoom=sample_viewer->get_zoom();
	float new_zoom=old_zoom*2.0f;

	float offset=sample_viewer->get_zoom_offset();
	float old_screen_in_samples=(float)sample_data->get_length()*old_zoom;
	float new_screen_in_samples=(float)sample_data->get_length()*new_zoom;
	float center=offset+(old_screen_in_samples/2.0f);
	offset=center-(new_screen_in_samples/2.0f);

	sample_viewer->set_zoom(new_zoom);
	sample_viewer->set_zoom_offset((int)offset);

	zoom_slider->setValue((int)(sample_viewer->get_zoom()*100));

	update_zoom_bar();
	updating_zoom=false;

}
void SampleViewerZoom::zoom_all_cbk() {
	if (!sample_viewer || !sample_data)
		return;

	updating_zoom=true;
	sample_viewer->set_zoom(1.0f);
	zoom_slider->setValue((int)(sample_viewer->get_zoom()*100));
	update_zoom_bar();
	updating_zoom=false;

}
void SampleViewerZoom::zoom_selection_cbk() {
	if (!sample_viewer || !sample_data)
		return;

	Selection *selection=sample_viewer->get_selection();
	if (!selection || !selection->is_active() || !sample_data || !sample_data->get_length())
		return;

	updating_zoom=true;

	float selection_size=selection->get_end()-selection->get_begin();

	float zoom_request=selection_size/(float)sample_data->get_length();
	sample_viewer->set_zoom(zoom_request);
	sample_viewer->set_zoom_offset(selection->get_begin());
	sample_viewer->update();
	zoom_slider->setValue(int(sample_viewer->get_zoom()*100));
//	zoom_view_offset->setValue(selection->get_begin());

	update_zoom_bar();
	updating_zoom=false;

}
void SampleViewerZoom::zoom_offset_changed_cbk(int p_ofs) {
	if (!sample_viewer || !sample_data)
		return;

	sample_viewer->set_zoom_offset(p_ofs);
	////printf("Want new zoom %i\n",p_ofs);
}

void SampleViewerZoom::set_sample_viewer(SampleViewer *p_sample_viewer) {

	sample_viewer=p_sample_viewer;
	if (sample_data)
		update_zoom_bar();

}

void SampleViewerZoom::set_sample_data(Sample *p_sample_data) {

	sample_data=p_sample_data;
	if (sample_viewer)  {

		sample_viewer->set_sample_data(p_sample_data);
		update_zoom_bar();
	}
}


SampleViewerZoom::SampleViewerZoom(QWidget *p_parent) : CVBox(p_parent),
	pix_zoom_more((const char**)zoom_plus_xpm),
	pix_zoom_less((const char**)zoom_minus_xpm),
	pix_zoom_all((const char**)zoom_all_xpm),
	pix_zoom_select((const char**)zoom_selection_xpm)
{

	sample_data=NULL;

	zoom_view_offset = new QScrollBar(Qt::Horizontal,this);

	zoom_box = new CHBox(this);
	zoom_box->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));

	zoom_slider = new QSlider(Qt::Horizontal,zoom_box);
	zoom_slider->setRange(1,100);
	zoom_slider->setValue(100);
	//zoom_slider->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
	zoom_slider_label = new QLabel("100%",zoom_box);


	zoom_more = new QPushButton(zoom_box); zoom_more->setIcon(pix_zoom_more);
	zoom_less = new QPushButton(zoom_box); zoom_less->setIcon(pix_zoom_less);
	zoom_all = new QPushButton(zoom_box); zoom_all->setIcon(pix_zoom_all);
	zoom_select = new QPushButton(zoom_box); zoom_select->setIcon(pix_zoom_select);
	zoom_more->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	zoom_less->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	zoom_all->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	zoom_select->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));



	QObject::connect(zoom_more, SIGNAL(clicked()),this, SLOT(zoom_more_cbk()) );
	QObject::connect(zoom_less, SIGNAL(clicked()),this, SLOT(zoom_less_cbk()) );
	QObject::connect(zoom_all, SIGNAL(clicked()),this, SLOT(zoom_all_cbk()) );
	QObject::connect(zoom_select, SIGNAL(clicked()),this, SLOT(zoom_selection_cbk()) );

	QObject::connect(zoom_view_offset,SIGNAL(valueChanged(int)),this,SLOT(zoom_offset_changed_cbk(int)));
	QObject::connect(zoom_slider,SIGNAL(valueChanged(int)),this,SLOT(zoom_slider_cbk(int)));
	updating_zoom=false;

	sample_viewer=NULL;

}

SampleViewerZoom::~SampleViewerZoom()
{
}

}
