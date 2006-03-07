//
// C++ Interface: chionic_interface
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHIONIC_INTERFACE_H
#define RESHAKEDCHIONIC_INTERFACE_H

#include "interface/sound_plugin_ui.h"
#include "plugins/chionic.h"
#include "plugin_UIs/chionic_window.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class ChionicInterface : public SoundPluginUI {
	
	Q_OBJECT
	
	ChionicWindow *chionic_ui;
	Chionic *chionic;	
	
	ChionicInterface(QWidget *p_parent,Chionic *p_chionic);	
private slots:	
	
	void show_ui();
	
public:
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	
	~ChionicInterface();

};

}

#endif
