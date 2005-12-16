//
// C++ Interface: skin_box
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSKIN_BOX_H
#define RESHAKEDSKIN_BOX_H


#include <Qt/qpixmap.h>

class QPainter;
namespace ReShaked {

/**
	@author red <red@killy>
*/
class SkinBox {
			    
	QPixmap pix_top_left;
	QPixmap pix_top;
	QPixmap pix_top_right;
	QPixmap pix_left;
	QPixmap pix_center;
	QPixmap pix_right;
	QPixmap pix_bottom_left;
	QPixmap pix_bottom;
	QPixmap pix_bottom_right;
	
	int top,left,right,bottom;	
	bool stretch;
public:
	
	int get_top();
	int get_left();
	int get_right();
	int get_bottom();
	
	void paint_into(QPainter&p, int p_x, int p_y, int p_w, int p_h);
	void load_from_xpm(const char** p_xpm,int p_top, int p_left, int p_bottom, int p_right,bool p_stretchable=false);
	void load_from_pixmap(const QPixmap& p_pixmap,int p_top, int p_left, int p_bottom, int p_right,bool p_stretchable=false);
	
	SkinBox(const char ** p_xpm,int p_top, int p_left, int p_bottom, int p_right,bool p_stretchable=false);
	SkinBox();
	~SkinBox();

};

}

#endif
