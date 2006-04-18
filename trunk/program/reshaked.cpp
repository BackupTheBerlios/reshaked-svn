

#include "editor/plugin_preset_manager.h"
#include "interface/main_window.h"
#include <Qt/qapplication.h>
#include "engine/sound_driver_list.h"
#include "engine/midi_driver_list.h"
#include "engine/audio_control.h"
#include "drivers/sound_driver_jack.h"
#include "engine/sound_plugin_list.h"
#include "plugins/amplifier_plugin.h"
#include "plugins/sine_synth.h"
#include "plugins/simpler.h"
#include "plugins/chionic.h"
#include "plugin_UIs/sound_plugin_ui_generic.h"
#include "plugin_UIs/chionic_interface.h"
#include "plugin_UIs/simpler_ui.h"
#include "drivers/get_time_posix.h"
#include "dsp/sample_file.h"
#include "editor/midi_input_handler.h"

#include "drivers/mididriver_alsa.h"

#include "drivers/sound_driver_portaudio.h"

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

#ifdef DRIVER_RTAUDIO_ENABLED

#include "drivers/sound_driver_rtaudio.h"


#endif

#ifdef SNDFILE_ENABLED

#include "drivers/sample_loader_sndfile.h"

#endif

/* WINDOWS CONFIG DIR SHOULD BE APPDATA */

ReShaked::SoundPluginList sound_plugin_list;

static void init_sound_plugin_list() {
	
	sound_plugin_list.add_info( ReShaked::AmplifierPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::SineSynth::create_info() );
	sound_plugin_list.add_info( ReShaked::Chionic::create_info() );
	sound_plugin_list.add_info( ReShaked::Simpler::create_info() );
	
}


ReShaked::SoundPluginUI_List sound_plugin_UI_list;

static void init_sound_plugin_UI_list() {
		
	sound_plugin_UI_list.add_creator(    ReShaked::ChionicInterface::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::SimplerUI::create_this );
	/* this one must go last, since it's the last resort */
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
	
	ReShaked::MidiInputHandler midi_input_handler;
	
	ReShaked::SoundDriverList driver_list;
	driver_list.set_midi_input_handler( &midi_input_handler );
	
	ReShaked::MidiDriverList midi_driver_list;
	midi_input_handler.set_midi_driver( &midi_driver_list );
	
	ReShaked::SampleFile sample_file;
		
#ifdef SNDFILE_ENABLED
	
	ReShaked::SampleLoader_SNDFile loader_sndfile;
	sample_file.add_loader( &loader_sndfile );
	
	
#endif
	
#ifdef DRIVER_ALSA_ENABLED
	
	ReShaked::MidiDriver_Alsa midi_driver_alsa;
	midi_driver_list.add_driver( &midi_driver_alsa );
	
#endif

	
#ifdef POSIX_ENABLED
	
	MutexLock::create_mutex=MutexLock_Pthreads::create_mutex_pthreads;
	ReShaked::GetTime_Posix get_time_posix;
	
# ifdef DRIVER_RTAUDIO_ENABLED
	
//	ReShaked::SoundDriver_RtAudio rtaudio_oss(RtAudio::LINUX_OSS);
//	driver_list.add_driver(&rtaudio_oss);
#  ifdef DRIVER_ALSA_ENABLED
	// Works like crap, i'm not using it for now
	//ReShaked::SoundDriver_RtAudio rtaudio_alsa(RtAudio::LINUX_ALSA);
	//driver_list.add_driver(&rtaudio_alsa);
	
#  endif
# endif	

#endif
	
#ifdef WIN32_ENABLED

        MutexLock::create_mutex=ReShaked::MutexLock_Win32::create_mutex_pthreads;
	ReShaked::GetTime_Win32 get_time_win32;
#ifdef DRIVER_RTAUDIO_ENABLED
	
//	ReShaked::SoundDriver_RtAudio rtaudio_ds(RtAudio::WINDOWS_DS);
//	driver_list.add_driver(&rtaudio_ds);
#endif

#endif
	
#ifdef DRIVER_PORTAUDIO_ENABLED
	
	ReShaked::SoundDriver_PortAudio::initialize_portaudio();
	ReShaked::SoundDriver_PortAudio *driver_portaudio = new ReShaked::SoundDriver_PortAudio;
	driver_list.add_driver(driver_portaudio);
	
#endif
	
	init_sound_plugin_list();
	init_sound_plugin_UI_list();
	
	ReShaked::AudioControl::init();
	
	

	test_config_dir();
	
	ReShaked::PluginPresetManager plugin_preset_manager(ReShaked::DeQStrify(CONFIG_DIR_PATH+"/"+CONFIG_DIR+"/presets"));
	
#ifdef DRIVER_JACK_ENABLED
	
	ReShaked::SoundDriver_JACK driver_jack;
	driver_list.add_driver( &driver_jack );
	
#endif
	
	
	ReShaked::MainWindow *w = new ReShaked::MainWindow;
	
	driver_list.init_driver(0);
	midi_driver_list.init_driver(0);
	
	//q.setMainWidget(&w);
	w->show();
	
	int res=q->exec();
	
#ifdef DRIVER_PORTAUDIO_ENABLED
	
	ReShaked::SoundDriver_PortAudio::finalize_portaudio();
	
#endif
	
	return res;

}
