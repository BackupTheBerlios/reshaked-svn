//
// C++ Interface: style_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUISTYLE_BOX_H
#define PIGUISTYLE_BOX_H

#include "base/defs.h"
#include "base/geometry.h"
namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
struct StyleBox {
	
	
	enum Mode {
		MODE_NONE, //dont draw anything
		MODE_FLAT, //draw flat, superfast, one color only
		MODE_FLAT_BITMAP, // draw bitmap border, but color for center
		MODE_BITMAP, //draw with bitmaps, this looks great, but it's sightly slower
	};
	
	Mode mode;
	
	struct Flat { //Used if flat mode is selected, this is very fast, but looks simple
		
		Color border_upleft;
		Color border_downright;
		int margin; //if margin is zero, then no border is drawn
		Color center;
		bool blend;
	} flat;
	
	enum BitmapPos {
		
		POS_TOPLEFT,
		POS_TOP,
		POS_TOPRIGHT,
		POS_LEFT,
		POS_CENTER,
		POS_RIGHT,
		POS_BOTTOMLEFT,
		POS_BOTTOM,
		POS_BOTTOMRIGHT,
		POS_MAX
	};
	
	int margins[4];
	BitmapID bitmaps[POS_MAX];
	
	bool draw_center;
	

	StyleBox();
	StyleBox(int p_margin,Color p_center,Color p_upleft,Color p_downright,bool p_blend=true);

	
};

}

#endif