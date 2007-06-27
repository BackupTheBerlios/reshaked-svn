

#include "editor/plugin_preset_manager.h"
#include "engine/sound_driver_list.h"
#include "engine/midi_driver_list.h"
#include "engine/audio_control.h"
#include "drivers/sound_driver_jack.h"
#include "drivers/vst_sound_plugin_source.h"
#include "engine/sound_plugin_list.h"
#include "plugins/amplifier_plugin.h"
#include "plugins/compressor_plugin.h"
#include "plugins/tool_plugins.h"
#include "plugins/moog_filter_plugin.h"
#include "plugins/sine_synth.h"
#include "plugins/sample_trigger.h"
#include "plugins/simpler.h"
#include "plugins/sinth.h"
#include "plugins/chionic.h"
#include "plugins/reverb_plugin.h"
#include "plugins/delay_line_plugin.h"
#include "plugins/distortion_plugin.h"
#include "plugins/stereo_enhancer_plugin.h"
#include "plugins/filter_bank_plugin.h"
#include "plugins/chorus_plugin.h"
#include "plugins/limiter_plugin.h"
#include "plugins/panner_plugin.h"
#include "plugins/freq_splitter_plugin.h"
#include "plugins/sound_plugin_eq.h"
#include "drivers/get_time_posix.h"
#include "dsp/sample_file.h"
#include "editor/midi_input_handler.h"

#include "drivers/ladspa_sound_plugin_source.h"

#include "drivers/mididriver_alsa.h"

#include "drivers/sound_driver_portaudio.h"

#include "drivers/get_time_win32.h"

#include "gui_custom/pixmap_data.h"
#include "gui_custom/rsskin.h"

#ifdef POSIX_ENABLED

#include "drivers/mutex_lock_pthreads.h"

#define CONFIG_DIR String(".reshaked")
#define CONFIG_DIR_PATH String(getenv("HOME"))
#endif

#ifdef WIN32_ENABLED

#include "drivers/mutex_lock_win32.h"

// for win9x, use current dir/config , for winXP use APPDATA
#define CONFIG_DIR String(getenv("APPDATA")?"ReShaked":"config")
#define CONFIG_DIR_PATH String(getenv("APPDATA")?getenv("APPDATA"):"./") 

#endif

#ifdef DRIVER_RTAUDIO_ENABLED

#include "drivers/sound_driver_rtaudio.h"


#endif

#ifdef SNDFILE_ENABLED

#include "drivers/sample_loader_sndfile.h"

#endif


#include "interface/main_window.h"

#include "gui/base/window.h"
#include "gui/drivers/painter_sdl.h"
#include "gui/drivers/timer_sdl.h"
#include "gui/drivers/keycodes_sdl.h"
#include "gui/drivers/file_system_dirent.h"
#include "gui/drivers/file_system_windows.h"
#include "gui/widgets/scroll_bar.h"

using namespace GUI;

/* WINDOWS CONFIG DIR SHOULD BE APPDATA */

ReShaked::SoundPluginList sound_plugin_list;

static void init_sound_plugin_list() {
	
	/* Synths */
	sound_plugin_list.add_info( ReShaked::Simpler::create_info() );
	sound_plugin_list.add_info( ReShaked::Sinth::create_info() );
	sound_plugin_list.add_info( ReShaked::SampleTrigger::create_info() );
	//sound_plugin_list.add_info( ReShaked::SineSynth::create_info() );
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

/*
static void test_config_dir() {
	
	QDir config_dir(CONFIG_DIR_PATH);
	if (!config_dir.cd(CONFIG_DIR)) {
		ERR_FAIL_COND( !config_dir.mkdir(CONFIG_DIR) );
		config_dir.cd(CONFIG_DIR);
				
	}
	
	if (!config_dir.cd("presets")) {
		
		ERR_FAIL_COND( !config_dir.mkdir("presets") );
		// copy all default presets 
	} else
		config_dir.cd("..");
	
	if (!config_dir.cd("themes")) {
		
		ERR_FAIL_COND( !config_dir.mkdir("themes") );
		// copy all default presets
	} else
		config_dir.cd("..");
	
	if (!config_dir.cd("insdefs")) {
		
		ERR_FAIL_COND( !config_dir.mkdir("insdefs") );
		// copy all default presets 
	} else
		config_dir.cd("..");
	
	if (!config_dir.cd("demos")) {
	
		ERR_FAIL_COND( !config_dir.mkdir("demos") );
		// copy all default presets 
	} else
		config_dir.cd("..");
		
}

*/

#define DEFAULT_H 550
#define DEFAULT_W 750

int main_loop(Window& window,SDL_Surface *screen,TimerSDL *timer,unsigned int flags,ReShaked::MainWindow *interface) {
	KeycodesSDL keycode_sdl_translator;
	
	bool done=false;
	
	SDL_Event event;
	Uint32 last_click_tick=0;
	bool can_dblclick=false;
	while((!done) && (SDL_WaitEvent(&event))) {
		
		
		switch(event.type) {
			//case SDL_USEREVENT:
		//		HandleUserEvents(&event);
	//			break;
			case SDL_VIDEORESIZE:
				printf("??\n");	
	
				screen = SDL_SetVideoMode  (event.resize.w,event.resize.h, 32, flags);
				window.set_size( Size(event.resize.w,event.resize.h ) );
				
				window.redraw_all();
				window.update();
				
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN: {
                // Handle any key presses here.
		
				unsigned int mod=0;
				
				if (  event.key.keysym.mod & (KMOD_LSHIFT|KMOD_RSHIFT))
					mod|=KEY_MASK_SHIFT;
				
				if (  event.key.keysym.mod & (KMOD_LALT|KMOD_RALT))
					mod|=KEY_MASK_ALT;
				
				if (  event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL))
					mod|=KEY_MASK_CTRL;
								  

				if (  event.key.keysym.mod & (KMOD_LMETA|KMOD_RMETA)) {
#ifdef META_AS_ALT					
					mod|=KEY_MASK_ALT;
#else
					
					mod|=KEY_MASK_META;
#endif
				}
				
				window.key( event.key.keysym.unicode, keycode_sdl_translator.get_code(event.key.keysym.sym), event.key.state==SDL_PRESSED,false, mod );
				
				
				if (event.key.keysym.sym==SDLK_F1) {

					window.get_painter()->draw_fill_rect( Point() , Size( screen->w , screen->h ), Color(100,20,33) );
					SDL_UpdateRect(screen, 0,0,0,0);
				}

				//if (event.key.keysym.sym==SDLK_F2) {

				//painter->update_screen();
				//}

				if ((event.key.keysym.mod&KMOD_LSHIFT) && event.key.keysym.sym==SDLK_F12) {
					
					printf("DeadLocking Audio Thread on Purpose! (shift-f12)!!\n");
					SDL_LockAudio();
				}
				
//				printf("Pressed Key %s, unicode %i\n",Keyboard::get_code_name( keycode_sdl_translator.get_code(event.key.keysym.sym)).ascii().get_data(),event.key.keysym.unicode);
			} break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN: {
				
				
				do {
					
					Uint32 last_click_delta=0;
					
					
					if (event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==BUTTON_LEFT) {
						
						last_click_delta=SDL_GetTicks()-last_click_tick;
						last_click_tick=SDL_GetTicks();
					}
					
																
					SDLMod	mod_state=SDL_GetModState();
	
					unsigned int mod=0;
					
					if (  mod_state & (KMOD_LSHIFT|KMOD_RSHIFT))
						mod|=KEY_MASK_SHIFT;
					
					if (  mod_state & (KMOD_LALT|KMOD_RALT))
						mod|=KEY_MASK_ALT;
					
					if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
						mod|=KEY_MASK_CTRL;
									
					if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
						mod|=KEY_MASK_META;
					
					
					window.mouse_button( Point( event.button.x, event.button.y ), event.button.button, event.button.type==SDL_MOUSEBUTTONDOWN, mod );
					
					if (can_dblclick && last_click_delta<250 && event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==BUTTON_LEFT) {
						window.mouse_doubleclick( Point( event.button.x, event.button.y ), mod );
						can_dblclick=false;
	
					} else
						can_dblclick=true;
					
				} while (SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEMOTION))>0);

					
				
				
			} break;
				
			case SDL_MOUSEMOTION: {

				can_dblclick=false; //can't doubleclick! wah wah wah
				/* Motion compensation, in case there are MANY motion events pending */
				SDL_Event compensator;
				
				while (SDL_PeepEvents(&compensator,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEMOTION))>0) {
					
					event.motion.xrel+=compensator.motion.xrel;
					event.motion.yrel+=compensator.motion.yrel;
					event.motion.state=compensator.motion.state;
					event.motion.x=compensator.motion.x;
					event.motion.y=compensator.motion.y;
				}

				
				int state=event.motion.state&0xFF;
				SDLMod	mod_state=SDL_GetModState();

				if (  mod_state & (KMOD_LSHIFT|KMOD_RSHIFT))
					state|=KEY_MASK_SHIFT;
				
				if (  mod_state & (KMOD_LALT|KMOD_RALT))
					state|=KEY_MASK_ALT;
				
				if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
					state|=KEY_MASK_CTRL;
								
				if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
					state|=KEY_MASK_META;

				
				window.mouse_motion( Point( event.motion.x, event.motion.y ), Point( event.motion.xrel, event.motion.yrel ), state );
			} break;
				
			case SDL_USEREVENT: {
				
				if (event.user.code!=TimerSDL::SDL_TIMER_EVENT_CODE)
					break;
				
				TimerID *timer_id=(TimerID*)event.user.data1;
				timer->call(*timer_id);
			} break; 
			case SDL_QUIT:
				interface->quit_request();
				break;

			default: {}
				break;

		}   // End switch
            

//		printf("check updates! - %i\n",i++);
		timer->loop_iterate();
		window.check_for_updates();
		//SDL_UpdateRect   (screen, 0, 0, 0, 0);
		
		done=interface->must_quit();
	}   // End while
        	
	return 0;
	
}

int main(int argc, char *argv[]) {

#ifdef WINDOWS_ENABLED	
	FileSystemWindows::set_default_filesystem();
#else
	FileSystemDirent::set_default_filesystem();

#endif
	

	
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
//	init_sound_plugin_UI_list();
	
	ReShaked::AudioControl::init();
	
	

//	test_config_dir();
	
	ReShaked::PluginPresetManager plugin_preset_manager((CONFIG_DIR_PATH+"/"+CONFIG_DIR+"/presets"));
	
#ifdef DRIVER_JACK_ENABLED
	
	ReShaked::SoundDriver_JACK driver_jack;
	driver_list.add_driver( &driver_jack );
	
#endif
	
	/** ENABLE GUI AND SDL */
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
 
	SDL_EnableUNICODE(1);

	const SDL_VideoInfo *vinfo=SDL_GetVideoInfo();
	Uint32 flags =SDL_SWSURFACE | SDL_RESIZABLE;
	SDL_Surface *screen = SDL_SetVideoMode  (DEFAULT_W, DEFAULT_H,vinfo->vfmt->BitsPerPixel , flags);
	
	PainterSDL *painter = new PainterSDL(screen);
	TimerSDL *timer = new TimerSDL;
	
	ScrollBar::set_can_focus_by_default( false );
	
	ReShaked::PixmapData::set_painter( painter ); //configure in-memory pixmaps
	
	
	ReShaked::RSSkin skin(painter);		
	
	
	Window window( painter, timer, &skin );
	
	
	
	ReShaked::MainWindow *interface = new ReShaked::MainWindow(CONFIG_DIR_PATH+"/"+CONFIG_DIR,"reshaked.cfg");
	window.set_root_frame(interface);
	interface->initialize();
	
	window.set_size( Size( DEFAULT_W,DEFAULT_H ) );
	window.update();
	window.redraw_all();
	painter->update_screen();
			
	/**** GUI AND SDL INIT END */
	
	if (true/*w->load_settings()*/) { //if failed loading settings, init the default drivers
		WARN_PRINT("Loading settings failed");
		driver_list.init_driver(0);
		midi_driver_list.init_driver(0);
	}
	
#ifdef VST_ENABLED
//settings (paths) were loaded, so scan VST plugins	
	
	vst_plugin_source.scan_plugins();
#endif	
	
	
	int res = main_loop(window,screen,timer,flags,interface);
	
	
	midi_driver_list.finish_driver();
	driver_list.finish_driver(); //avoid multithread problems

	
#ifdef DRIVER_PORTAUDIO_ENABLED
	
	ReShaked::SoundDriver_PortAudio::finalize_portaudio();
	delete driver_portaudio;
	
#endif
	
	return res;

}
