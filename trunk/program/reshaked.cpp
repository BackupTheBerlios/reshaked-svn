

#include "interface/main_window.h"
#include <Qt/qapplication.h>
#include "engine/sound_driver_list.h"
#include "engine/audio_control.h"
#include "drivers/sound_driver_jack.h"
#include "engine/sound_plugin_list.h"
#include "plugins/amplifier_plugin.h"
#include "plugins/sine_synth.h"
#include "plugin_UIs/sound_plugin_ui_generic.h"
#include "drivers/get_time_posix.h"

#include "interface/sound_plugin_ui_list.h"
#ifdef POSIX_ENABLED

#include "drivers/mutex_lock_pthreads.h"

#endif


ReShaked::SoundPluginList sound_plugin_list;

static void init_sound_plugin_list() {
	
	sound_plugin_list.add_info( ReShaked::AmplifierPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::SineSynth::create_info() );
	
}


ReShaked::SoundPluginUI_List sound_plugin_UI_list;

static void init_sound_plugin_UI_list() {
		
	sound_plugin_UI_list.add_creator(    ReShaked::SoundPluginUI_Generic::create_this );
	
}

int main(int argc, char *argv[]) {

#ifdef POSIX_ENABLED
	
	MutexLock::create_mutex=MutexLock_Pthreads::create_mutex_pthreads;
	ReShaked::GetTime_Posix get_time_posix;
#endif
	
	
	init_sound_plugin_list();
	init_sound_plugin_UI_list();
	
	
	
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
