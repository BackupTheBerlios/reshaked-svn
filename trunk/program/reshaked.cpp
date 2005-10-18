

#include "interface/main_window.h"
#include <Qt/qapplication.h>

int main(int argc, char *argv[]) {

	QApplication q(argc,argv);
	ReShaked::MainWindow w;
	//q.setMainWidget(&w);
	w.show();
	
	return q.exec();

}
