#ifndef CURSOR_H
#define CURSOR_H

#include "typedefs.h"


//@TODO Make an object for pattern changed notify or something

namespace ReShaked {

class Cursor {
public:
	enum TrackEditTarget {
		EDIT_TRACK,
		EDIT_AUTOMATION
	};

private:

	int cursor_pos;
	int current_track;
	TrackEditTarget edit_target;
	int current_automation;
	Tick snap; /* tick snap,beat divisor */
	int window_offset; /* offset in snapped_beats */
	int window_size; /* in snap-amounts beats */

	Tick snap_to_ticks(int p_snap);
	int ticks_to_snap(Tick p_ticks);

public:

	int get_pos();
	void set_pos(int p_pos);
	Tick get_tick_pos();
	
	int get_track();
	void set_track(int p_track);
	int get_track_automation();
	void set_edited_track_automation(int p_track_automation);
	void set_snap(Tick p_snap);
	Tick get_snap();
	int get_beat();
	void set_window_offset(int p_offset);
	int get_window_offset();
	void set_window_size(int p_size);
	int get_window_size();
	Tick get_snapped_window_tick_pos(int p_pos);
	int get_snapped_window_beat(int p_pos);
	int get_snapped_window_subsnap(int p_pos);
	void set_edit_target(TrackEditTarget p_target);
	TrackEditTarget get_edit_target();

	
	/* reimplement these for your UI */
	virtual void window_moved();
	virtual void cursor_moved();
	virtual void cursor_changed_track();
	
        Cursor();
	virtual ~Cursor();

};


};

#endif
