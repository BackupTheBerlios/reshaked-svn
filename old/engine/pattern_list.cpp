//
// C++ Implementation: pattern_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pattern_list.h"

namespace ReShaked {

int PatternList::get_pattern_count() {


	return pattern_list.get_stream_size();
}

String PatternList::get_pattern_name(int p_idx) {

	ERR_FAIL_INDEX_V(p_idx,pattern_list.get_stream_size(),"<ERROR>");
	return pattern_list.get_index_value(p_idx).name;
	
}

void PatternList::set_pattern_name(int p_idx,String p_name) {

	ERR_FAIL_INDEX(p_idx,pattern_list.get_stream_size());
	pattern_list.get_index_value_w(p_idx).name=p_name;


}

bool PatternList::insert_pattern(String p_name,int p_beat) {
	
	
	ERR_FAIL_COND_V(  pattern_list.get_exact_index( p_beat ) != INVALID_STREAM_INDEX ,true );	
	
	ERR_FAIL_COND_V(p_beat<0,true);
	
	pattern_list.insert(p_beat,PatternData(p_name));
	
	return false;	
	
}

int PatternList::get_pattern_beat_from_global_beat(int p_beat) {

	int prev_idx=pattern_list.get_prev_index(p_beat);
	//printf("p_beat is %i, prev_idx is %i\n",p_beat,prev_idx);
	ERR_FAIL_COND_V(prev_idx==INVALID_STREAM_INDEX,-1);
	ERR_FAIL_INDEX_V(prev_idx,pattern_list.get_stream_size(),-1);
	int pos = p_beat-pattern_list.get_index_pos(prev_idx);
	//printf("pos for beat %i is %i\n",p_beat,pos);
	return pos;

}

int PatternList::get_pattern_from_global_beat(int p_beat) {
	
	
	int prev_idx=pattern_list.get_prev_index(p_beat);
	//printf("p_beat is %i, prev_idx is %i\n",p_beat,prev_idx);
	ERR_FAIL_COND_V(prev_idx==INVALID_STREAM_INDEX,-1);
	ERR_FAIL_INDEX_V(prev_idx,pattern_list.get_stream_size(),-1);
	
	return prev_idx;

}

bool PatternList::erase_pattern(int p_pattern) {
	
	return false;
	
}

PatternList::PatternList() {
	
	insert_pattern("Begin",0);
	
}

}; //end of namespace