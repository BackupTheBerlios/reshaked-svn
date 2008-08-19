
#include <stdio.h>

namespace GUI {

	void __print_error(const char* p_file,int p_line,const char *p_msg) {
	
		fprintf(stderr,"GUI ERROR: %s:%i - %s\n",p_file,p_line,p_msg);
	}
}

#include "engine/audio_driver.h"
#include "drivers/file_system_windows.h"
#include "drivers/file_system_dirent.h"
#include "drivers/timer_sdl.h"
#include "drivers/keycodes_sdl.h"
#include "drivers/painter_sdl.h"
#include "drivers/file_libc.h"
#include "drivers/file_windows.h"
#include "drivers/audio_driver_jack.h"
#include "widgets/scroll_bar.h"
#include "gui_main/main_window.h"
#include "gui_common/common_skin.h"
#include "gui_common/default_paths.h"
#include "gui_common/tree_saver_file.h"
#include "gui_common/tree_loader_file.h"

#include <SDL.h>

#define DEFAULT_H 550
#define DEFAULT_W 750
#define interface interface


#ifdef WINDOWS_ENABLED

#define BASE_PATH String( String(getenv("APPDATA"))+"\\Reshaked")

#else

#define BASE_PATH String( String(getenv("HOME"))+"/.reshaked")

#endif

int main_loop(SDL_Surface *screen,GUI::TimerSDL *timer,unsigned int flags,MainWindow *main_window) {
	GUI::KeycodesSDL keycode_sdl_translator;
	
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
				main_window->set_size( GUI::Size(event.resize.w,event.resize.h ) );
				
				main_window->redraw_all();
				main_window->update();
				
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN: {
                // Handle any key presses here.
		
				unsigned int mod=0;
				
				if (  event.key.keysym.mod & (KMOD_LSHIFT|KMOD_RSHIFT))
					mod|=GUI::KEY_MASK_SHIFT;
				
				if (  event.key.keysym.mod & (KMOD_LALT|KMOD_RALT))
					mod|=GUI::KEY_MASK_ALT;
				
				if (  event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL))
					mod|=GUI::KEY_MASK_CTRL;
								  

				if (  event.key.keysym.mod & (KMOD_LMETA|KMOD_RMETA)) {
#ifdef META_AS_ALT					
					mod|=GUI::KEY_MASK_ALT;
#else
					
					mod|=GUI::KEY_MASK_META;
#endif
				}
			

			
				main_window->key( event.key.keysym.unicode, keycode_sdl_translator.get_code(event.key.keysym.sym), event.key.state==SDL_PRESSED,false, mod );
				
				
				if (event.key.keysym.sym==SDLK_F12) {

					main_window->get_painter()->draw_fill_rect( GUI::Point() , GUI::Size( screen->w , screen->h ), GUI::Color(100,20,33) );
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
					
					
					if (event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==GUI::BUTTON_LEFT) {
						
						last_click_delta=SDL_GetTicks()-last_click_tick;
						last_click_tick=SDL_GetTicks();
					}
					
																
					SDLMod	mod_state=SDL_GetModState();
	
					unsigned int mod=0;
					
					if (  mod_state & (KMOD_LSHIFT|KMOD_RSHIFT))
						mod|=GUI::KEY_MASK_SHIFT;
					
					if (  mod_state & (KMOD_LALT|KMOD_RALT))
						mod|=GUI::KEY_MASK_ALT;
					
					if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
						mod|=GUI::KEY_MASK_CTRL;
									
					if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
						mod|=GUI::KEY_MASK_META;
					
					
					main_window->mouse_button( GUI::Point( event.button.x, event.button.y ), event.button.button, event.button.type==SDL_MOUSEBUTTONDOWN, mod );
					
					if (can_dblclick && last_click_delta<250 && event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==GUI::BUTTON_LEFT) {
						main_window->mouse_doubleclick( GUI::Point( event.button.x, event.button.y ), mod );
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
					state|=GUI::KEY_MASK_SHIFT;
				
				if (  mod_state & (KMOD_LALT|KMOD_RALT))
					state|=GUI::KEY_MASK_ALT;
				
				if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
					state|=GUI::KEY_MASK_CTRL;
								
				if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
					state|=GUI::KEY_MASK_META;

				
				main_window->mouse_motion( GUI::Point( event.motion.x, event.motion.y ), GUI::Point( event.motion.xrel, event.motion.yrel ), state );
			} break;
				
			case SDL_USEREVENT: {
				
				if (event.user.code!=GUI::TimerSDL::SDL_TIMER_EVENT_CODE)
					break;
				
				GUI::TimerID *timer_id=(GUI::TimerID*)event.user.data1;
				timer->call(*timer_id);
			} break; 
			case SDL_QUIT:
				main_window->quit_request();
				break;

			default: {}
				break;

		}   // End switch
            

//		printf("check updates! - %i\n",i++);
		timer->loop_iterate();
		main_window->check_for_updates();
		//SDL_UpdateGUI::Rect   (screen, 0, 0, 0, 0);
		
		done=main_window->must_quit();
	}   // End while
        	
	return 0;
	
}

int main(int argc, char *argv[]) {

		
#ifdef WINDOWS_ENABLED	
	GUI::FileSystemWindows::set_default_filesystem();
	GUI::FileWindows::set_as_default();
#else
	GUI::FileSystemDirent::set_default_filesystem();
	GUI::FileLibC::set_as_default();

#endif
	
	TreeSaverFile::make_default();
	TreeLoaderFile::make_default();
	
#ifdef JACK_ENABLED
	
	AudioDriverJACK driver_jack;
	AudioDriver::add_audio_driver( &driver_jack );
	
#endif
	
	AudioDriver::initialize_audio(0); // for now, this
	
	DefaultPaths::set_base_path(BASE_PATH);
	
	/** ENABLE GUI AND SDL */
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
 
	SDL_EnableUNICODE(1);

	const SDL_VideoInfo *vinfo=SDL_GetVideoInfo();
	Uint32 flags =SDL_SWSURFACE | SDL_RESIZABLE;
	SDL_Surface *screen = SDL_SetVideoMode  (DEFAULT_W, DEFAULT_H,vinfo->vfmt->BitsPerPixel , flags);
	
	GUI::PainterSDL *painter = new GUI::PainterSDL(screen);
	GUI::TimerSDL *timer = new GUI::TimerSDL;
	
	GUI::ScrollBar::set_can_focus_by_default( false );
	
	PixmapData::set_painter(painter);
	CommonSkin skin(painter);	
	
	MainWindow * main_window = new MainWindow( painter, timer, &skin );
	
	
	main_window->set_size( GUI::Size( DEFAULT_W,DEFAULT_H ) );
	main_window->update();
	main_window->redraw_all();
	painter->update_screen();
			
			
			
	/**** GUI AND SDL INIT END */
		
	/* config */
	
	main_window->init(""); // no config
	
	
	if (AudioDriver::get_current_driver())
		AudioDriver::get_current_driver()->restart();

	
	int res = main_loop(screen,timer,flags,main_window);
	
	//AudioDriver *current_driver = AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	
	//if (current_driver && current_driver->is_active())
		//current_driver->finish();
	
	return res;

}

