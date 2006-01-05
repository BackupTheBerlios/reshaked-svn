//
// C++ Implementation: sound_plugin_ui_generic
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_ui_generic.h"
#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>
#include "ui_blocks/helpers.h"
namespace ReShaked {

SoundPluginUI* SoundPluginUI_Generic::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	
	return new SoundPluginUI_Generic(p_parent,p_plugin);
}

SoundPluginUI_Generic::SoundPluginUI_Generic(QWidget *p_parent,SoundPlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	
	new QHBoxLayout(this);
	QPushButton *pb=new QPushButton(QStrify(p_plugin->get_info()->caption ),this);
	pb->setMinimumSize(80,20);
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	layout()->addWidget(pb);
	//setFixedSize(80,20);
	
}


SoundPluginUI_Generic::~SoundPluginUI_Generic()
{
}


}
