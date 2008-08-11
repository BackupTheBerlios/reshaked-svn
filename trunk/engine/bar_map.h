//
// C++ Interface: bar_map
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BAR_MAP_H
#define BAR_MAP_H

#include "value_stream.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class BarMap{

	enum {
		DEFAULT_BAR_LEN=4
	};
	
	ValueStream<int,int> bar_len_map;
public:
	
	void insert_bar_len_at_beat(int p_beat,int p_len);
	int get_bar_len_idx_from_beat(int p_beat);
	int get_bar_at_beat(int p_beat);
	int get_bar_len_at_beat(int p_beat);
	int get_bar_beat(int p_beat);
	void remove_bar_len(int p_bar_len_idx);
	void set_bar_len_idx_len(int p_bar_len_idx,int p_len);
	
	int get_bar_len_idx_pos(int p_bar_len_idx);
	
	int get_bar_len_idx_at_beat(int p_beat);
	int get_bar_len_idx_len(int p_bar_len_idx);
	int get_bar_len_idx_count();
	
	
	void reset();
	BarMap();
	~BarMap();

};

#endif
