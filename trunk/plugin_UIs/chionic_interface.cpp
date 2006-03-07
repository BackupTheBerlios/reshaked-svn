//
// C++ Implementation: chionic_interface
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <Qt/qpushbutton.h>
#include "chionic_interface.h"
#include "ui_blocks/helpers.h"
namespace ReShaked {



SoundPluginUI* ChionicInterface::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	Chionic *chionic=dynamic_cast<Chionic*>(p_plugin);
	if (chionic==NULL)
		return NULL;
	
	return new ChionicInterface(p_parent,chionic);
	
}

void ChionicInterface::show_ui() {
	
	chionic_ui->show();
	chionic_ui->exec();
}

ChionicInterface::ChionicInterface(QWidget *p_parent,Chionic *p_chionic) : SoundPluginUI(p_parent,p_chionic) {
	
	setLayout(new QVBoxLayout(this));
	chionic=p_chionic;
		
	if (!chionic->get_metadata()) {
		
		ChionicWindow *window = new ChionicWindow(topLevelOf(p_parent),p_chionic);
		p_chionic->set_metadata( window );
		
	}
	
	chionic_ui=dynamic_cast<ChionicWindow*>( chionic->get_metadata() );
	
	QPushButton *b = new QPushButton("Show UI",this);
	layout()->addWidget(b);
	
	QObject::connect(b ,SIGNAL(clicked()),this,SLOT(show_ui()));
	
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	show();
}


ChionicInterface::~ChionicInterface() {
	
}


}
