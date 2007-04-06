//
// C++ Implementation: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "main_window.h"

namespace ReShaked {

	
bool MainWindow::must_quit() {
	
	return false;	
}

	
void MainWindow::quit_request() {
		

}
	
MainWindow::MainWindow() 
{
	
	add(new Label("hello"));
}


MainWindow::~MainWindow()
{
}


}
