//
// C++ Implementation: midi_input_handler
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_input_handler.h"
#include "engine/track_pattern.h"
namespace ReShaked {

MidiInputHandler * MidiInputHandler::singleton=NULL;


/* midi input logic diagram..

	if (note event) {

		
		if (playing && recording) {

			send to editor as record event
			send_to_cursor_overchan

		} else if (key input) {
	
			send to editor as input event
			send_to_cursor_overchan
			
		} else {
		
			send_to_rack channel
			
		}
		

	} else {
		if (playing && automation recording) {
		
			send to ui thread as record event
		} else {
			send_to_current_chan
		}
			}
		
	}

*/
void MidiInputHandler::sound_thread_callback() {
	
	if (midi_driver->get_driver_count()==0 || !midi_driver->is_current_driver_active())
		return;
	
	MidiDriver *driver=midi_driver->get_driver();
	
	ERR_FAIL_COND(!driver);

	while (driver->has_input_events_pending()) {
		
		DriverInputEvent ev;
		driver->get_input_event( &ev );
		
		EventMidi em=ev.midi;
		
		SongPlayback* playback=&editor->get_song()->get_song_playback();
		
		
		if (em.midi_type==EventMidi::MIDI_CONTROLLER) {
			
			last_control.available=true;
			last_control.channel=em.channel;
			last_control.control=em.data.control.index;
			last_control.value=em.data.control.parameter;
			
		}

		if (em.midi_type==EventMidi::MIDI_NOTE_ON || em.midi_type==EventMidi::MIDI_NOTE_OFF) {
			
			
			if (playback->is_recording() && playback->get_status()==SongPlayback::STATUS_PLAY && ev.tick>=0) {
			
				
				BlockList *bl=editor->get_blocklist( editor->get_current_blocklist() );
				
				if (bl==NULL)
					continue;
				
				Track_Pattern *pattern=dynamic_cast<Track_Pattern*>(bl);
				
				if (!pattern)
					continue; //not over pattern
					
				pattern->play_external_event( em );
				sound_to_gui_rb.push(ev);
				
				
			} else if (editor->is_pattern_midi_input_enabled()) {
			
					
				/* input the event */
				ev.tick=-1; //means it's not meant to be recorded, just inserted at editor
				sound_to_gui_rb.push(ev); 
				
			} else {
			
				/* send event to rack */
				
				int track_idx=editor->get_current_rack_track();
				
				if (track_idx<0 || track_idx>=editor->get_song()->get_track_count())
					continue;
							
				Track *track=editor->get_song()->get_track( track_idx );
				
				Track_Pattern *pattern=dynamic_cast<Track_Pattern*>(track);
				
				if (!pattern)
					continue; //not over pattern
					
				pattern->play_external_event( em );

					
			} 
				
			
		} else {
			
			if (playback->is_recording_automations() && playback->get_status()==SongPlayback::STATUS_PLAY  && ev.tick>=0) {
			
				
				sound_to_gui_rb.push(ev); 
				
			} else { //not recording or anything? just send the event to rack
				
				BlockList *bl=editor->get_blocklist( editor->get_current_blocklist() );
			
				if (bl==NULL)
					continue;
			
				Track_Pattern *pattern=dynamic_cast<Track_Pattern*>(bl);
			
				if (!pattern)
					continue; //not over pattern
				
				pattern->play_external_event( em );
				
			}
		
		}
	
	}
	
}
void MidiInputHandler::gui_thread_callback() {

	SongPlayback* playback=&editor->get_song()->get_song_playback();
		
	while (sound_to_gui_rb.pending()) {
		
		DriverInputEvent ev;
		
		ERR_CONTINUE(sound_to_gui_rb.pop(&ev));
		

		if ( ( playback->is_recording_automations() || playback->is_recording() ) && playback->get_status()==SongPlayback::STATUS_PLAY  && ev.tick>=0) {
				

			editor->midi_record_event(ev);
		} else {
			
			editor->midi_edit_event(ev.midi);
		}
	}
	
}


MidiInputHandler * MidiInputHandler::get_singleton() {
	
	return singleton;
}

void MidiInputHandler::set_editor(Editor *p_editor) {
	
	
	editor=p_editor;
}
void MidiInputHandler::set_midi_driver(MidiDriverList *p_midi_driver) {
	
	
	midi_driver=p_midi_driver;
}

bool MidiInputHandler::is_last_control_available() {
	
	return last_control.available;
}
bool MidiInputHandler::get_last_control(int *p_channel,int *p_control, int *p_value) {
	
	if (!last_control.available)
		return true;
	
	*p_channel=last_control.channel;
	*p_control=last_control.control;
	*p_value=last_control.value;
	
	last_control.available=false;
	return false;
	
}


MidiInputHandler::MidiInputHandler()
{
	
	last_control.available=false;
	editor=NULL;
	midi_driver=NULL;
	singleton=this;
}


MidiInputHandler::~MidiInputHandler()
{
}


}
