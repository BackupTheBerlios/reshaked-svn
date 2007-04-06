//
// C++ Interface: rsskin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDRSSKIN_H
#define RESHAKEDRSSKIN_H

#include "base/skin.h"

using namespace GUI;

namespace ReShaked {
enum StyleBoxList{
	
	
	RS_SB_MAX=SB_MAX
};
	
enum ColorList {
		
	RS_COLOR_MAX=COLOR_MAX
};

enum ConstantList {

	RS_C_MAX=C_MAX
};
	
enum FontList {
	
	RS_FONT_MAX=FONT_MAX
};

enum BitmapList {
	
	RS_BITMAP_MAX=BITMAP_MAX
};

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class RSSkin : public Skin {
public:
	
	
    RSSkin();
    ~RSSkin();

};

}

#endif
