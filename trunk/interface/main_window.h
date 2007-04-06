//
// C++ Interface: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMAIN_WINDOW_H
#define RESHAKEDMAIN_WINDOW_H


#include "containers/box_container.h"
#include "widgets/label.h"


using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class MainWindow : public VBoxContainer {
	
	
	
	
	
public:
	
	bool must_quit();
	void quit_request();
	MainWindow();
	~MainWindow();

};

}

#endif
