

#include "interface/main_window.h"
#include <Qt/qapplication.h>

int main(int argc, char *argv[]) {

	QApplication *q = new QApplication(argc,argv);
	ReShaked::MainWindow *w = new ReShaked::MainWindow;
	//q.setMainWidget(&w);
	w->show();
	
	return q->exec();

}
