//
// C++ Interface: bar_map
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBAR_MAP_H
#define RESHAKEDBAR_MAP_H

#include "value_stream.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class BarMap{

	enum {
		DEFAULT_BAR_LEN=4
	};
	
	ValueStream<int,int> bar_len_map;
public:
	
	int insert_bar_len_at_beat(int p_beat,int p_len);
	int get_bar_from_beat(int p_beat);
	int get_barcount_at_beat(int p_beat);
	int get_bar_beat_from_beat(int p_beat);
	void remove_bar_len(int p_bar_idx);
	void set_bar_len(int p_bar_idx,int p_len);
	
	BarMap();
	~BarMap();

};

}

#endif
