//
// C++ Implementation: bar_map
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bar_map.h"



void BarMap::insert_bar_len_at_beat(int p_beat,int p_len) {
	
	bar_len_map.insert(p_beat,p_len);
}


int BarMap::get_bar_len_idx_from_beat(int p_beat) {

	ERR_FAIL_COND_V(p_beat<0,-1);	
	int idx=bar_len_map.find( p_beat );
	ERR_FAIL_COND_V(idx<0,-1);
	return idx;	
}

int BarMap::get_bar_len_at_beat(int p_beat) {

	return get_bar_len_idx_len( get_bar_len_idx_from_beat( p_beat ) );
}

int BarMap::get_bar_at_beat(int p_beat) {
	
	ERR_FAIL_COND_V(p_beat<0,-1);	
	int idx=bar_len_map.find( p_beat );
	ERR_FAIL_COND_V(idx<0,-1);
	
	/* calculate bars until beat, since current "bar change" */
	
	int bar_count=0;

	for (int i=0;i<idx;i++) {
		
		int barsize=bar_len_map[i];
		int beats=bar_len_map.get_pos( i+1 )-bar_len_map.get_pos( i );
		int bars=(beats/barsize)+1;
		bar_count+=bars;					
	}
	
	int barsize=bar_len_map[idx];
	int beats=p_beat - bar_len_map.get_pos( idx );
	int bars=(beats/barsize);
	bar_count+=bars;

	return bar_count;
	
}
int BarMap::get_bar_beat(int p_beat) {
	
	int bar_len_idx=get_bar_len_idx_from_beat(p_beat);
	int bar_len= bar_len_map[bar_len_idx ];
	return (p_beat - bar_len_map.get_pos( bar_len_idx ))%bar_len;
	
}
void BarMap::remove_bar_len(int p_bar_len_idx) {
	
	if (p_bar_len_idx==0)
		return;//cant remove the first one, but act as if we did :)
	ERR_FAIL_INDEX(p_bar_len_idx,bar_len_map.size());
	bar_len_map.erase( p_bar_len_idx );
		
}

void BarMap::set_bar_len_idx_len(int p_bar_len_idx,int p_len) {
	
	ERR_FAIL_INDEX(p_bar_len_idx,bar_len_map.size());
	bar_len_map[p_bar_len_idx]=p_len;
}

int BarMap::get_bar_len_idx_pos(int p_bar_len_idx) {
	
	ERR_FAIL_INDEX_V(p_bar_len_idx,bar_len_map.size(),-1);
	return bar_len_map.get_pos(p_bar_len_idx);	
}

int BarMap::get_bar_len_idx_at_beat(int p_beat) {
	
	int idx=bar_len_map.find_exact( p_beat );
	if (idx<0)
		idx=-1;
	
	return idx;
}
int BarMap::get_bar_len_idx_len(int p_bar_len_idx) {
	
	ERR_FAIL_INDEX_V(p_bar_len_idx,bar_len_map.size(),-1);
	
	return bar_len_map[p_bar_len_idx];	
}	

int BarMap::get_bar_len_idx_count() {
	
	return bar_len_map.size();
}


void BarMap::reset() {
	
	bar_len_map.clear();
	bar_len_map.insert( 0, DEFAULT_BAR_LEN); //default	
}
BarMap::BarMap() {
	
	reset();
	
}


BarMap::~BarMap() {
}
