//
// C++ Interface: midi_input_handler_base
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_INPUT_HANDLER_BASE_H
#define RESHAKEDMIDI_INPUT_HANDLER_BASE_H

namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiInputHandlerBase {
public:
	
	virtual void sound_thread_callback()=0; ///< Called from the sound thread
	MidiInputHandlerBase();
	virtual ~MidiInputHandlerBase();

};

}

#endif
