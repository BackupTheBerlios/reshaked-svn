//
// C++ Implementation: scrolled_viewport
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "scrolled_viewport.h"
#include <Qt/qpushbutton.h>


namespace ReShaked {




CHBox * ScrolledViewport::get_hbox() {
	
	
	return hbox;	
}


ScrolledViewport::ScrolledViewport(QWidget * p_parent) : QScrollArea(p_parent) {
	
	hbox = new CHBox(this);
	setWidget(hbox);	
	
}

ScrolledViewport::~ScrolledViewport() {
	
	
	
}

	
	
	
}