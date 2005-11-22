//
// C++ Implementation: pixmap_font
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pixmap_font.h"
#include <string.h>

namespace ReShaked {



void PixmapFont::render_string(QPainter& p, int p_x,int p_y,const char *p_string) {
	
	for (int i=0;i<strlen(p_string);i++) {
		
		std::map<int,QPixmap>::iterator I=char_list.find(p_string[i]);
		if (I==char_list.end())
			continue;
		p.drawPixmap(p_x+i*width,p_y,I->second);
	}
	
}

int PixmapFont::get_width() {
	
	
	return width;	
}
int PixmapFont::get_height() {
	
	return height;
}
void PixmapFont::set_font(const QPixmap& p_base,const char* p_chars) {
	
	int chars=strlen(p_chars);
	height=p_base.height()-2;
	width=(p_base.width()-(chars+1))/chars;
	
	for(int i=0;i<chars;i++) {
		
		QPixmap p=p_base.copy(1+(width+1)*i,1,width,height);
		char_list[p_chars[i]]=p;
	}
	
}

PixmapFont::PixmapFont() {
	
	width=height=1;
}
PixmapFont::PixmapFont(const QPixmap& p_base,const char* p_chars) {
	
	set_font(p_base,p_chars);
}
PixmapFont::~PixmapFont() {
	
	
	
}

	
	
};