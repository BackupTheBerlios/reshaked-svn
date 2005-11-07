#ifndef CURSOR_H
#define CURSOR_H

#include "typedefs.h"


//@TODO Make an object for pattern changed notify or something

namespace ReShaked {

class Cursor {
public:

private:

	int cursor_pos;
	int current_blocklist;
	Tick snap; /* tick snap,beat divisor */
	int window_offset; /* offset in snapped_beats */
	int window_size; /* in snap-amounts beats */

	Tick snap_to_ticks(int p_snap);
	int ticks_to_snap(Tick p_ticks);

public:

	int get_pos();
	void set_pos(int p_pos);
	Tick get_tick_pos();
	
	int get_blocklist();
	void set_blocklist(int p_blocklist);
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
	virtual void window_moved();
	virtual void cursor_moved();
	virtual void cursor_changed_blocklist();
	
        Cursor();
	virtual ~Cursor();

};


};

#endif
