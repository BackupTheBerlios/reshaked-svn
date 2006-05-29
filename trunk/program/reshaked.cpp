

#include "editor/plugin_preset_manager.h"
#include "interface/main_window.h"
#include <Qt/qapplication.h>
#include "engine/sound_driver_list.h"
#include "engine/midi_driver_list.h"
#include "engine/audio_control.h"
#include "drivers/sound_driver_jack.h"
#include "drivers/vst_sound_plugin_source.h"
#include "plugin_UIs/vst_plugin_ui.h"
#include "engine/sound_plugin_list.h"
#include "plugins/amplifier_plugin.h"
#include "plugins/compressor_plugin.h"
#include "plugins/tool_plugins.h"
#include "plugins/moog_filter_plugin.h"
#include "plugins/sine_synth.h"
#include "plugins/simpler.h"
#include "plugins/chionic.h"
#include "plugins/reverb_plugin.h"
#include "plugins/delay_line_plugin.h"
#include "plugins/distortion_plugin.h"
#include "plugins/stereo_enhancer_plugin.h"
#include "plugins/filter_bank_plugin.h"
#include "plugin_UIs/chorus_plugin_ui.h"
#include "plugin_UIs/limiter_plugin_ui.h"
#include "plugins/chorus_plugin.h"
#include "plugin_UIs/sound_plugin_ui_generic.h"
#include "plugin_UIs/chionic_interface.h"
#include "plugin_UIs/simpler_ui.h"
#include "plugin_UIs/panner_plugin_ui.h"
#include "plugin_UIs/delay_line_ui.h"
#include "plugin_UIs/moog_filter_plugin_ui.h"
#include "plugins/limiter_plugin.h"
#include "plugins/panner_plugin.h"
#include "plugins/freq_splitter_plugin.h"
#include "plugins/sound_plugin_eq.h"
#include "plugin_UIs/reverb_plugin_ui.h"
#include "plugin_UIs/filter_bank_plugin_ui.h"
#include "drivers/get_time_posix.h"
#include "dsp/sample_file.h"
#include "editor/midi_input_handler.h"

#include "drivers/ladspa_sound_plugin_source.h"

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
	
	/* Synths */
	sound_plugin_list.add_info( ReShaked::Simpler::create_info() );
	sound_plugin_list.add_info( ReShaked::SineSynth::create_info() );
	sound_plugin_list.add_info( ReShaked::Chionic::create_info() );
	
	/* Effects */
	sound_plugin_list.add_info( ReShaked::AmplifierPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::PannerPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::ChorusPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::ReverbPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::DelayLinePlugin::create_info_msec() );
	sound_plugin_list.add_info( ReShaked::DelayLinePlugin::create_info_bpm() );
	sound_plugin_list.add_info( ReShaked::StereoEnhancerPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::LimiterPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::CompressorPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::FilterBankPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::MoogFilterPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::FreqSplitterPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::SoundPluginEQ::create_info_6() );
	sound_plugin_list.add_info( ReShaked::SoundPluginEQ::create_info_10() );
	sound_plugin_list.add_info( ReShaked::DistortionPlugin::create_info() );
	
	/* Tools */
	sound_plugin_list.add_info( ReShaked::SplitterPlugin::create_info() );
	sound_plugin_list.add_info( ReShaked::MergerPlugin::create_info() );
}


ReShaked::SoundPluginUI_List sound_plugin_UI_list;

static void init_sound_plugin_UI_list() {
		
	sound_plugin_UI_list.add_creator(    ReShaked::ChionicInterface::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::SimplerUI::create_this );
	
	sound_plugin_UI_list.add_creator(    ReShaked::ChorusPluginUI::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::ReverbPluginUI::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::FilterBankPluginUI::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::LimiterPluginUI::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::MoogFilterPluginUI::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::PannerPluginUI::create_this );
	sound_plugin_UI_list.add_creator(    ReShaked::DelayLinePluginUI::create_this );
#ifdef VST_ENABLED
	sound_plugin_UI_list.add_creator(    ReShaked::VST_PluginUI::create_this );
#endif
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
	} else
		config_dir.cd("..");
	
	if (!config_dir.cd("themes")) {
		
		ERR_FAIL_COND( !config_dir.mkdir("themes") );
		/* copy all default presets */
	} else
		config_dir.cd("..");
	
	if (!config_dir.cd("insdefs")) {
		
		ERR_FAIL_COND( !config_dir.mkdir("insdefs") );
		/* copy all default presets */
	} else
		config_dir.cd("..");
	
	if (!config_dir.cd("demos")) {
	
		ERR_FAIL_COND( !config_dir.mkdir("demos") );
		/* copy all default presets */
	} else
		config_dir.cd("..");
		
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
#ifdef LADSPA_ENABLED
		
	ReShaked::LADSPA_SoundPluginSource ladspa_plugin_source;
	
#endif
#ifdef VST_ENABLED
		
	ReShaked::VST_SoundPluginSource vst_plugin_source;
	
#endif
	init_sound_plugin_UI_list();
	
	ReShaked::AudioControl::init();
	
	

	test_config_dir();
	
	ReShaked::PluginPresetManager plugin_preset_manager(ReShaked::DeQStrify(CONFIG_DIR_PATH+"/"+CONFIG_DIR+"/presets"));
	
#ifdef DRIVER_JACK_ENABLED
	
	ReShaked::SoundDriver_JACK driver_jack;
	driver_list.add_driver( &driver_jack );
	
#endif
	
	
	ReShaked::MainWindow *w = new ReShaked::MainWindow(CONFIG_DIR_PATH+"/"+CONFIG_DIR,"settings.cfg");
	
	if (w->load_settings()) { //if failed loading settings, init the default drivers
		WARN_PRINT("Loading settings failed");
		driver_list.init_driver(0);
		midi_driver_list.init_driver(0);
	}
	
#ifdef VST_ENABLED
//settings (paths) were loaded, so scan VST plugins	
	
	vst_plugin_source.scan_plugins();
#endif	
	//q.setMainWidget(&w);
	w->show();
	
	int res=q->exec();
	
	delete w;
#ifdef DRIVER_PORTAUDIO_ENABLED
	
	ReShaked::SoundDriver_PortAudio::finalize_portaudio();
	
#endif
	
	return res;

}
