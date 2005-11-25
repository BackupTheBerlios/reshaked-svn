//
// C++ Implementation: bar_map
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bar_map.h"

namespace ReShaked {


int BarMap::insert_bar_len_at_beat(int p_beat,int p_len) {
	
	bar_len_map.insert(p_beat,p_len);
}
int BarMap::get_bar_from_beat(int p_beat) {
	
	int idx=bar_len_map.get_prev_index( p_beat );
	if (idx<0)
		idx=0;
	return idx;
}
int BarMap::get_bar_beat_from_beat(int p_beat) {
	
	int bar=get_bar_from_beat(p_beat);
	return p_beat - bar_len_map.get_index_pos( bar );
	
}
void BarMap::remove_bar_len(int p_bar_idx) {
	
	if (p_bar_idx==0)
		return;//cant remove the first one
	ERR_FAIL_INDEX(p_bar_idx,bar_len_map.get_stream_size());
	bar_len_map.erase_index( p_bar_idx );
		
}

void BarMap::set_bar_len(int p_bar_idx,int p_len) {
	
	ERR_FAIL_INDEX(p_bar_idx,bar_len_map.get_stream_size());
	bar_len_map.get_index_value_w(p_bar_idx)=p_len;
}
BarMap::BarMap() {
	
	bar_len_map.insert( 0, DEFAULT_BAR_LEN); //default
}


BarMap::~BarMap() {
}


}
