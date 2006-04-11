//
// C++ Interface: midi_input_handler
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_INPUT_HANDLER_H
#define RESHAKEDMIDI_INPUT_HANDLER_H

#include "editor/editor.h"
#include "engine/midi_driver_list.h"
#include "engine/midi_input_handler_base.h"
#include "dsp/lock_free_ring_buffer.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiInputHandler : public MidiInputHandlerBase {
	
	
	
	static MidiInputHandler * singleton;
	
	Editor *editor;
	MidiDriverList *midi_driver; 
	
	
	LockFreeRingBuffer<DriverInputEvent,1024> sound_to_gui_rb;
	
	struct LastControl {
		
		int channel;
		int control;
		int value;
		bool available;		
	} last_control;
	
public:
	
	
	static MidiInputHandler * get_singleton();
	
	     
	void sound_thread_callback(); ///< Called from the sound thread
	void gui_thread_callback(); ///< Called from the GUI thread
		
	
	void set_editor(Editor *p_editor);
	void set_midi_driver(MidiDriverList *p_midi_driver);

	/* this is used for assigning controls to automations */
	bool is_last_control_available();
	bool get_last_control(int *p_channel,int *p_control, int *p_value);
	
	MidiInputHandler();
	~MidiInputHandler();
};

}

#endif
