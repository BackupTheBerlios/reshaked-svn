//
// C++ Implementation: reshaked
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <qmainwindow.h>
#include "interface/main_window.h"
#include <qapplication.h>


int main(int argc, char *argv[]) {


	QApplication app(argc,argv);
	ReShaked::MainWindow mainwin;
	mainwin.show();
	printf("wah?\n");
	
	app.exec();

 	return 0;
}