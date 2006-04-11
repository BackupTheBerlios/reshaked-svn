//
// C++ Implementation: editor_midi
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "editor.h"


namespace ReShaked {
/* THESE HELPER MACROS NEED pattern_track DEFINED */
static Track_Pattern::Note _fix_macro(const Track_Pattern::Note &p_note) { return p_note; } //fix template annoyance
#define SET_NOTE(m_pos,m_note) d->undo_stream.add_command( Command3(&commands,&EditorCommands::set_note,pattern_track,m_pos,_fix_macro(m_note)) )

void Editor::midi_reset() {
	
	d->pattern_edit.poly_input_offset=0;
}


void Editor::set_polyphonic_midi_input_enabled(bool p_enabled) {
	
	d->global_edit.polyphonic_midi_input_enabled=p_enabled;
	d->pattern_edit.poly_input_offset=0;
}

void Editor::midi_record_automation_event(Track *p_track,const DriverInputEvent& ev) {
	
	int prop_idx=p_track->find_recording_property( ev.midi.channel, ev.midi.data.control.index);
	if (prop_idx<0)
		return;

	Automation *a=p_track->get_property_automation(prop_idx);
	
	float val=(float)ev.midi.data.control.parameter/127.0;
	add_automation_point(a,ev.tick,val,0,true);
}


void Editor::midi_record_event(const DriverInputEvent& ev) {
	
	ERR_FAIL_COND(ev.tick<0);
		

	if (ev.midi.midi_type==EventMidi::MIDI_CONTROLLER && d->song->get_song_playback().is_recording_automations()) {

		midi_record_automation_event(&d->song->get_global_track(),ev);
		
		for (int i=0;i<d->song->get_track_count();i++) {
			
			midi_record_automation_event(d->song->get_track(i),ev);
			
		}
				   
	} else if (ev.midi.midi_type==EventMidi::MIDI_NOTE_ON) {
		
		
		
	}
	
	
}
void Editor::midi_edit_event(const EventMidi& ev) {
	
	BlockList *bl = get_blocklist( get_current_blocklist() );
	if (!bl)
		return;
	
	Track_Pattern *pattern_track = dynamic_cast<Track_Pattern*>(bl);
	if (!pattern_track)
		return;
	
	if (ev.midi_type==EventMidi::MIDI_NOTE_ON) {
		
		
		int column=d->pattern_edit.column;
		
		if (d->global_edit.polyphonic_midi_input_enabled) {
			
			column+=d->pattern_edit.poly_input_offset;
			d->pattern_edit.poly_input_offset++;
			if (column>=pattern_track->get_visible_columns())
				return;
		}
		
		if (!pattern_track->is_pos_editable( d->cursor.get_tick_pos() ) )
			return;
		
		int note=ev.data.note.note;
		Tick tickpos=d->cursor.get_tick_pos();
			
		int volume=99;
		
		if (d->pattern_edit.volume_mask_active) {
			volume=d->pattern_edit.volume_mask;
		} else {
				
			Track_Pattern::Note note=pattern_track->get_note(Track_Pattern::Position(tickpos,column));
				
			if (note.is_note())
				volume=note.volume;
		}
			
		d->undo_stream.begin("Set Note");
		SET_NOTE(Track_Pattern::Position(tickpos,column),Track_Pattern::Note(note,volume));
		d->undo_stream.end();
		play_note_at_cursor(column);
		
		if (!d->global_edit.polyphonic_midi_input_enabled)
			d->cursor.set_pos( d->cursor.get_pos() +1 );
			
		if (d->pattern_edit.volume_mask_active)
			d->pattern_edit.volume_mask=volume;
		d->ui_update_notify->volume_mask_changed();
		d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
			
	} if (ev.midi_type==EventMidi::MIDI_NOTE_OFF && d->global_edit.polyphonic_midi_input_enabled) {
		
		
		if (d->pattern_edit.poly_input_offset==1)
			d->cursor.set_pos( d->cursor.get_pos() +1 );

		
		d->pattern_edit.poly_input_offset--;
		if (d->pattern_edit.poly_input_offset<0)
			d->pattern_edit.poly_input_offset=0;
		
		
	}
}




}