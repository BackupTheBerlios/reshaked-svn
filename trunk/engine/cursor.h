#ifndef CURSOR_H
#define CURSOR_H

#include "typedefs.h"


#include "engine/ui_updatenotify.h"

//@TODO Make an object for pattern changed notify or something

namespace ReShaked {

class Cursor {
public:

private:

	int cursor_pos;
	Tick snap; /* tick snap,beat divisor */
	int window_offset; /* offset in snapped_beats */
	int window_size; /* in snap-amounts beats */

	Tick snap_to_ticks(int p_snap);
	int ticks_to_snap(Tick p_ticks);

	UI_UpdateNotify *ui_update_notify;
	
	Cursor();
public:

	int get_pos();
	void set_pos(int p_pos);
	Tick get_tick_pos();
	void set_from_tick_pos(Tick p_pos); ///< set tick pos and snap it
	Tick get_tick_from_pos(int p_snap);
	
	Tick get_snap_tick_size();
	
	void set_snap(Tick p_snap);
	Tick get_snap();
	int get_beat();
	void set_window_offset(int p_offset);
	int get_window_offset();
	void set_window_size(int p_rows); //in rows
	int get_window_size();
	Tick get_snapped_window_tick_pos(int p_row);
	int get_snapped_window_beat(int p_row);
	int get_snapped_window_subsnap(int p_row);

	
	/* reimplement these for your UI */
	void window_moved();
	void cursor_moved();
	
        Cursor(UI_UpdateNotify *p_ui_update_notify);
	virtual ~Cursor();

};


};

#endif
