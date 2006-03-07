//
// C++ Interface: chionic_window
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHIONIC_WINDOW_H
#define RESHAKEDCHIONIC_WINDOW_H

#include <Qt/qdialog.h>
#include <Qt/qpixmap.h>

#include "plugins/chionic.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class ChionicWindow : public QDialog, public SoundPlugin_MetaData {
	
	Chionic *chionic;


public:
	ChionicWindow(QWidget *p_parent,Chionic *p_chionic);
	~ChionicWindow();
};

}

#endif
