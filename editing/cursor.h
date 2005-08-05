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
	Tick snap; /* tick snap */
public:


	Tick get_cursor_tick();
	void set_cursor_tick(Tick p_tick);
	int get_edited_track();
	void set_edited_track(int p_track);
	int get_edited_track_automation();
	void set_edited_track_automation(int p_track_automation);
	void set_cursor_snap(Tick p_snap);
	Tick get_cursor_snap();

        Cursor();

};


};

#endif
