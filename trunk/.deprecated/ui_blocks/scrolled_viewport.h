//
// C++ Interface: scrolled_viewport
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef SCROLLED_VIEWPORT_H
#define SCROLLED_VIEWPORT_H


#include "ui_blocks/helpers.h"
#include <Qt/qscrollarea.h>

namespace ReShaked {



class ScrolledViewport : public QScrollArea {

	CHBox *hbox;
	
public:	

	CHBox * get_hbox();

	ScrolledViewport(QWidget * p_parent);
	~ScrolledViewport();

};


};



#endif