//
// C++ Implementation: selection_data
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "selection_data.h"

namespace ReShaked {


ColumnData::~ColumnData() {
	
	
}

void SelectionData::add_column_data(ColumnData* p_data) {
	
	column_list.push_back(p_data);
}
int SelectionData::get_column_count() {
	
	return column_list.size();
}
ColumnData* SelectionData::get_column(int p_which) {
	
	ERR_FAIL_INDEX_V(p_which,column_list.size(),NULL);
	return column_list[p_which];
	
}
void SelectionData::clear() {
	
	for (int i=0;i<column_list.size();i++) {
		
		delete column_list[i];
	}
	column_list.clear();
}

bool SelectionData::empty() {
	
	return column_list.size()==0;
}

void SelectionData::set_length(Tick p_length) {
	
	length=p_length;
}

Tick SelectionData::get_length() {
	
	return length;
}


SelectionData::SelectionData()
{
}


SelectionData::~SelectionData()
{
	
	clear();
}


}
