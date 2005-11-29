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

	ERR_FAIL_COND_V(p_beat<0,-1);	
	int idx=bar_len_map.get_prev_index( p_beat );
	ERR_FAIL_COND_V(idx<0,-1);
	return idx;	
}
int BarMap::get_barcount_at_beat(int p_beat) {
	
	ERR_FAIL_COND_V(p_beat<0,-1);	
	int idx=bar_len_map.get_prev_index( p_beat );
	
	ERR_FAIL_COND_V(idx<0,-1);
	
	int bar_count=0;
	
	for (int i=0;i<idx;i++) {
		
		int barsize=bar_len_map.get_index_value( i );
		int beats=bar_len_map.get_index_pos( i+1 )-bar_len_map.get_index_pos( i );
		int bars=(beats/barsize)+1;
		bar_count+=bars;					
	}
	
	int barsize=bar_len_map.get_index_value( idx );
	int beats=p_beat + bar_len_map.get_index_pos( idx );
	int bars=(beats/barsize);
	bar_count+=bars;
	
	return bar_count;
	
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
