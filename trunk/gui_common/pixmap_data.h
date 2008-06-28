//
// C++ Interface: pixmap_data
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_DATA_H
#define RESHAKEDPIXMAP_DATA_H

#include "base/painter.h"
#include "pixmap_data_enum.h"	
/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class PixmapData{
	
	/* Array to the data (in png format) for each pixmap */
	static const unsigned char * pixmap_list[PIXMAP_MAX];
	static GUI::BitmapID create_bitmap_from_buffer(const unsigned char *p_png_buffer);
	static GUI::BitmapID pixmap_id_list[PIXMAP_MAX];
	
	
	static GUI::Painter *painter;
public:
	
	static GUI::BitmapID get_pixmap(PixmapDataList p_pixmap);
	static void set_painter(GUI::Painter *p_painter);
};


#endif
