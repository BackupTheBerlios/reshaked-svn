//
// C++ Interface: compressor_curve_display
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPRESSOR_CURVE_DISPLAY_H
#define COMPRESSOR_CURVE_DISPLAY_H

#include "base/widget.h"
#include "engine/hl_audio_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class CompressorCurveDisplay : public GUI::Widget{

	ControlPort *threshold;
	ControlPort *ratio;
	
	float last_threshold;
	float last_ratio;
	
	GUI::Size get_minimum_size_internal();
	
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	
public:

	void check_for_changes();

	CompressorCurveDisplay(ControlPort *p_threshold,ControlPort *p_ratio);
	
	~CompressorCurveDisplay();

};

#endif
