//
// C++ Implementation: compressor_curve_display
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "compressor_curve_display.h"
#include "common_skin.h"
#include "base/painter.h"

GUI::Size CompressorCurveDisplay::get_minimum_size_internal() {

	return GUI::Size( constant( C_NODEUI_COMPRESSOR_CURVE_SIZE ), constant( C_NODEUI_COMPRESSOR_CURVE_SIZE ) );
}

void CompressorCurveDisplay::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	GUI::Painter *p = get_painter();
		
	p->draw_stylebox( stylebox( SB_NODEUI_COMRESSOR_CURVE ), GUI::Point(), p_size );
	
	GUI::Size areasize = p_size - p->get_stylebox_min_size( stylebox( SB_NODEUI_COMRESSOR_CURVE ) );
	
	GUI::Point margin = GUI::Point( p->get_stylebox_margin( stylebox( SB_NODEUI_COMRESSOR_CURVE ), GUI::MARGIN_LEFT), 
		p->get_stylebox_margin( stylebox( SB_NODEUI_COMRESSOR_CURVE ), GUI::MARGIN_RIGHT) );
		
	GUI::Point thres_point = areasize;
	
	thres_point.x = (int)( thres_point.x * threshold->get_normalized() );
	thres_point.y = (int)( thres_point.y * threshold->get_normalized() );
	
	GUI::Point ratio_point = areasize;
	
	ratio_point.y = thres_point.y + (int)( (areasize.y-thres_point.y)/ratio->get() );
	
	GUI::Point p0 = margin+GUI::Point( 0, areasize.y );
	GUI::Point p1 = margin+GUI::Point( thres_point.x, areasize.y - thres_point.y -1 );
	GUI::Point p2 = margin+GUI::Point( ratio_point.x, areasize.y - ratio_point.y -1 );
	 
	p->draw_line( p0 , p1, color( COLOR_NODEUI_COMPRESSOR_PLOT ) );
	p->draw_line( p1, p2, color( COLOR_NODEUI_COMPRESSOR_PLOT ) );
	p->draw_fill_rect( p1-GUI::Point(1,1), GUI::Size(3,3), color( COLOR_NODEUI_COMPRESSOR_PLOT ) );
	
}


void CompressorCurveDisplay::check_for_changes() {

	float new_ratio=ratio->get_normalized();
	float new_threshold=threshold->get_normalized();
	
	if (new_ratio!=last_ratio || new_threshold!=last_threshold) {
	
		update();
		last_ratio=new_ratio;
		last_threshold=new_threshold;
	}
}


CompressorCurveDisplay::CompressorCurveDisplay(ControlPort *p_threshold,ControlPort *p_ratio) {

	threshold = p_threshold;
	ratio=p_ratio;
	last_threshold=-1;
	last_ratio=-1;
	
}



CompressorCurveDisplay::~CompressorCurveDisplay()
{
}


