//
// C++ Implementation: chionic_window
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic_window.h"
#include "ui_blocks/helpers.h"
#include "plugin_UIs/chionic_pixmaps/chionic_bg.xpm"
#include <Qt/qpainter.h>
#include <Qt/qpushbutton.h>

namespace ReShaked {


ChionicWindow::ChionicWindow(QWidget *p_parent,Chionic *p_chionic) : QDialog(p_parent) {
	
	QPalette p=palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	setPalette(p);
	
	setLayout(new QVBoxLayout(this));
	CHBox *bg=new CHBox(this);
	bg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout()->addWidget(bg);

	bg->set_bg( QPixmap((const char*)chionic_bg_xpm) );
	bg->show();
	
	new QPushButton("hoe",bg);
	hide();
	
	chionic=p_chionic;
}


ChionicWindow::~ChionicWindow()
{
}


}
