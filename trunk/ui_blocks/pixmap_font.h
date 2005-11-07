//
// C++ Interface: pixmap_font
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PIXMAP_FONT_H
#define PIXMAP_FONT_H


#include <Qt/qpainter.h>
#include <Qt/qstring.h>
#include <map>

namespace ReShaked {


class PixmapFont {

	std::map<int,QPixmap> char_list;
	
	int width,height;

public:
	
	void render_string(QPainter& p, int p_x,int p_y,const char *p_string);
	
	int get_width();
	int get_height();
	void set_font(const QPixmap& p_base,const char* p_chars);
	PixmapFont();
	PixmapFont(const QPixmap& p_base,const char* p_chars);
	~PixmapFont();
};

};


#endif