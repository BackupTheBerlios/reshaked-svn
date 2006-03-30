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

#ifndef SAMPLE_VIEWER_H
#define SAMPLE_VIEWER_H

#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include "dsp/sample.h"
#include "dsp/selection.h"

namespace ReShaked {

/**
 *
 * Juan Linietsky
 **/
class SampleViewer : public QWidget
{
	Q_OBJECT
public:

	enum Colors {

		Col_BackGround,
		Col_ForeGround,
		Col_Center,
		Col_Loop_BackGround,
		Col_Loop_ForeGround,
		Col_Played_Position,

		Col_BackGround_Selected,
		Col_ForeGround_Selected,
		Col_Center_Selected,
		Col_Loop_BackGround_Selected,
		Col_Loop_ForeGround_Selected,
		Col_Played_Position_Selected,

		Col_Max
	};


protected:

	static int color_list[Col_Max*3];
	static QColor colors[Col_Max];
	static bool colors_initialized;


	int screen_pos_to_sample_pos(int p_pos);

	//int get_factor();


	void screen_to_sample(int p_int, FrameData *p_max_peak,FrameData *p_min_peak, int sample_cache_idx,int cur_width,int *p_final_sample);


	void paintEvent( QPaintEvent * p_event );

	float zoom;
	int offset;


	SampleViewer();
	Sample *sample_data;


	void draw_screen_pos(int p_x,QPainter &painter,int factor);
	bool event ( QEvent * e );
	bool grabbing_mouse;
	int closer_point; //1 or 2

	enum GrabType {

		SET_SELECTION,
		SET_SELECTION_BEGIN,
		SET_SELECTION_END
	};

	GrabType grab_type;


	Selection *selection;

public:

	static QColor * get_color(Colors p_color);
	static void init_colormap();

	//QSizePolicy sizePolicy() const;
	void sample_changed_notification() { update(); }
	float get_zoom();
	void set_zoom(float p_factor);
	void set_selection(Selection *p_selection) { selection=p_selection; }
	Selection *get_selection() { return selection; }

	int get_zoom_offset();
	void set_zoom_offset(int p_offset);
	int get_max_zoom_offset();



	SampleViewer( QWidget *parent=0);
	void set_sample_data(Sample *p_sample_Data);

	~SampleViewer();
};

}

#endif
