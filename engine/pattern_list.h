//
// C++ Interface: pattern_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PATTERN_LIST_H
#define PATTERN_LIST_H

#include "typedefs.h"
#include "value_stream.h"


namespace ReShaked {


class PatternList {
	
	struct PatternData {
		
		String name;
		int repeats;	
		
		PatternData(String p_name="",int p_repeats=1) { name = p_name; repeats = p_repeats; }
	};
	
	ValueStream<int,PatternData> pattern_list;
	
public:	
	
	int get_pattern_count();
	String get_pattern_name(int p_idx);
	int get_pattern_repeats(int p_idx);
	void set_pattern_name(int p_idx,String p_name);
	void set_pattern_repeats(int p_idx,int p_repeats);
	
	bool insert_pattern(String p_name,int p_beat); //return true if it beat is invalid or it already exists
	bool erase_pattern(int p_pattern);
	
	int get_pattern_beat(int p_beat);
	
	PatternList();
};



};

#endif