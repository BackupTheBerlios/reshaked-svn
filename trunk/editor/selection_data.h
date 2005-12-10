//
// C++ Interface: selection_data
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSELECTION_DATA_H
#define RESHAKEDSELECTION_DATA_H


#include "engine/track_pattern.h"
#include "engine/automation.h"
namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

struct ColumnData {

	enum Type {
		TYPE_PATTERN,
		TYPE_AUTOMATION
	};
	
				       
	virtual Type get_type()=0;
	
	virtual ~ColumnData();
	
};

struct ColumnDataPattern : public ColumnData {
	
	struct NoteData {
		
		Tick tick;
		Track_Pattern::Note note;
	};
	
	std::list<NoteData> notes;
	
	Type get_type() { return TYPE_PATTERN; }
};

struct ColumnDataAutomation : public ColumnData {
	
	struct PointData {
		
		Tick tick;
		Automation::AutomationValue point;
	};
	
	std::list<PointData> points;
	
	Type get_type() { return TYPE_AUTOMATION; }
};

class SelectionData {

	std::vector<ColumnData*> column_list;
public:
	void add_column_data(ColumnData* p_data);
	int get_column_count();
	ColumnData* get_column(int p_which);
	void clear();
	bool empty();
	
	SelectionData();
   	~SelectionData();

};

}

#endif
