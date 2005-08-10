#ifndef CURSOR_H
#define CURSOR_H

#include "typedefs.h"

namespace ReShaked {

class Cursor {

	enum TrackEditTarget {
		EDIT_TRACK,
		EDIT_AUTOMATION
	};



	Tick cursor_tick;
	int current_track;
	TrackEditTarget edit_target;
	int current_automation;
	Tick snap; /* tick snap,beat divisor */
	int window_offset; /* offset in snapped_beats */
	int window_size; /* in snap-amounts beats */

	Tick snap_to_ticks(int p_snap);
	int ticks_to_snap(Tick p_ticks);

public:

	Tick get_tick();
	void set_tick(Tick p_tick);
	int get_track();
	void set_track(int p_track);
	int get_track_automation();
	void set_edited_track_automation(int p_track_automation);
	void set_snap(Tick p_snap);
	Tick get_snap();
	void set_window_offset(int p_offset);
	int get_window_offset();
	void set_window_size(int p_size);
	int get_window_size();
	Tick get_snapped_window_tick_pos(int p_pos);


        Cursor();

};


};

#endif
