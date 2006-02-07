

#include "editor/plugin_preset_manager.h"
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

#include "drivers/get_time_win32.h"


#include <Qt/qdir.h>

#include "interface/sound_plugin_ui_list.h"
#ifdef POSIX_ENABLED

#include "drivers/mutex_lock_pthreads.h"

#define CONFIG_DIR QString(".reshaked")
#define CONFIG_DIR_PATH QString(getenv("HOME"))
#endif

#ifdef WIN32_ENABLED

#include "drivers/mutex_lock_win32.h"

#define CONFIG_DIR QString("ReShaked")
#define CONFIG_DIR_PATH QString(getenv("APPDATA"))

#endif



/* WINDOWS CONFIG DIR SHOULD BE APPDATA */

ReShaked::SoundPluginList sound_plugin_list;

static void init_sound_plugin_list() {
	
	sound_plugin_list.add_info( ReShaked::AmplifierPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::SineSynth::create_info() );
	
}


ReShaked::SoundPluginUI_List sound_plugin_UI_list;

static void init_sound_plugin_UI_list() {
		
	sound_plugin_UI_list.add_creator(    ReShaked::SoundPluginUI_Generic::create_this );
	
}

static void test_config_dir() {
	
	QDir config_dir(CONFIG_DIR_PATH);
	if (!config_dir.cd(CONFIG_DIR)) {
		ERR_FAIL_COND( !config_dir.mkdir(CONFIG_DIR) );
		config_dir.cd(CONFIG_DIR);
				
	}
	
	if (!config_dir.cd("presets")) {
		
		ERR_FAIL_COND( !config_dir.mkdir("presets") );
		
		
		/* copy all default presets */
	}
	
}

int main(int argc, char *argv[]) {

	
	QApplication *q = new QApplication(argc,argv);
	
#ifdef POSIX_ENABLED
	
	MutexLock::create_mutex=MutexLock_Pthreads::create_mutex_pthreads;
	ReShaked::GetTime_Posix get_time_posix;
	
#endif
	
#ifdef WIN32_ENABLED

        MutexLock::create_mutex=ReShaked::MutexLock_Win32::create_mutex_pthreads;
	ReShaked::GetTime_Win32 get_time_win32;

#endif
	
	init_sound_plugin_list();
	init_sound_plugin_UI_list();
	
	
	
	ReShaked::AudioControl::init();
	
	ReShaked::SoundDriverList driver_list;
	
	test_config_dir();
	
	ReShaked::PluginPresetManager plugin_preset_manager(ReShaked::DeQStrify(CONFIG_DIR_PATH+"/"+CONFIG_DIR+"/presets"));
	
#ifdef DRIVER_JACK_ENABLED
	
	ReShaked::SoundDriver_JACK driver_jack;
	driver_list.add_driver( &driver_jack );
	
#endif
	
	
	ReShaked::MainWindow *w = new ReShaked::MainWindow;
	driver_list.init_driver();
	
	//q.setMainWidget(&w);
	w->show();
	
	
	return q->exec();

}
