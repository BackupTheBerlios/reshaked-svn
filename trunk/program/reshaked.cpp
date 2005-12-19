

#include "interface/main_window.h"
#include <Qt/qapplication.h>
#include "engine/sound_driver_list.h"
#include "drivers/sound_driver_jack.h"


int main(int argc, char *argv[]) {

	ReShaked::SoundDriverList driver_list;
	
#ifdef DRIVER_JACK_ENABLED
	
	ReShaked::SoundDriver_JACK driver_jack;
	driver_list.add_driver( &driver_jack );
	
#endif
	
	
	QApplication *q = new QApplication(argc,argv);
	ReShaked::MainWindow *w = new ReShaked::MainWindow;
	driver_list.init_driver( -1, false);
	
	//q.setMainWidget(&w);
	w->show();
	
	
	return q->exec();

}
