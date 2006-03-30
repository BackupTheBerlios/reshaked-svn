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

#ifndef SAMPLE_VIEWER_ZOOM_H
#define SAMPLE_VIEWER_ZOOM_H



#include "ui_blocks/helpers.h"

#include "ui_blocks/sample_viewer.h"
#include <Qt/qpushbutton.h>
#include <Qt/qpixmap.h>
#include <Qt/qlabel.h>
#include <Qt/qscrollbar.h>
#include <Qt/qslider.h>
/**
 *
 * Juan Linietsky
 **/
 
 namespace ReShaked {
 
class SampleViewerZoom : public CVBox {

	Q_OBJECT
	SampleViewer * sample_viewer;

        bool updating_zoom;

	CHBox * zoom_box;

	QPushButton *zoom_more;
	QPushButton *zoom_less;
	QPushButton *zoom_all;
	QPushButton *zoom_select;

	QScrollBar *zoom_view_offset;

	QPixmap pix_zoom_more;
	QPixmap pix_zoom_less;
	QPixmap pix_zoom_all;
	QPixmap pix_zoom_select;

	Sample *sample_data;
	QSlider * zoom_slider;
	QLabel * zoom_slider_label;


protected slots:

	void zoom_more_cbk();
	void zoom_less_cbk();
	void zoom_all_cbk();
	void zoom_selection_cbk();
	void zoom_slider_cbk(int p_val);
	void zoom_offset_changed_cbk(int p_ofs);

public:

	void update_zoom_bar();

	void set_sample_data(Sample *p_sample_data);
	void set_sample_viewer(SampleViewer *p_sample_viewer);
	
	SampleViewerZoom(QWidget *p_parent);
	
	~SampleViewerZoom();
};

 }
 
#endif
