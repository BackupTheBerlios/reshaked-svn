//
// C++ Implementation: global_beatbar_column
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_beatbar_column.h"
#include "visual_settings.h"
#include <Qt/qpainter.h>

namespace Reshaked {

void GlobalBeatBarColumn::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
}

GlobalBeatBarColumn::GlobalBeatBarColumn(QWidget *p_parent) : QWidget(p_parent) {
	
	
}


GlobalBeatBarColumn::~GlobalBeatBarColumn()
{
}


}
