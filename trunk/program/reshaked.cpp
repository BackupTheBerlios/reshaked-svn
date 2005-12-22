

#include "interface/main_window.h"
#include <Qt/qapplication.h>
#include "engine/sound_driver_list.h"
#include "engine/audio_control.h"
#include "drivers/sound_driver_jack.h"

#ifdef POSIX_ENABLED

#include "drivers/mutex_lock_pthreads.h"

#endif

int main(int argc, char *argv[]) {

#ifdef POSIX_ENABLED
	
	MutexLock::create_mutex=MutexLock_Pthreads::create_mutex_pthreads;
#endif
	
	
	ReShaked::AudioControl::init();
	
	ReShaked::SoundDriverList driver_list;
	
#ifdef DRIVER_JACK_ENABLED
	
	ReShaked::SoundDriver_JACK driver_jack;
	driver_list.add_driver( &driver_jack );
	
#endif
	
	
	QApplication *q = new QApplication(argc,argv);
	ReShaked::MainWindow *w = new ReShaked::MainWindow;
	driver_list.init_driver( -1);
	
	//q.setMainWidget(&w);
	w->show();
	
	
	return q->exec();

}
