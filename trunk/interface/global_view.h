//
// File: global_view.h
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//


#ifndef _GLOBALVIEW_H_
#define _GLOBALVIEW_H_

#include <Qt/qwidget.h>
#include "engine/song.h"
#include <set>

namespace ReShaked {
	
class GlobalView : public QWidget {
	
	Song *song;
	void paintEvent(QPaintEvent *p);
	
	enum {
		MAX_LISTS=1000
	}; //dont think there will be more than that..
	
	struct DisplayParams {
		float zoom_width;
		float zoom_height;
		float ofs_x,ofs_y;
	} display;
	

	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	//selection
	
	std::set<int> selection;
	
	//helpers
	BlockList* get_block_list(int p_idx);
	int get_block_list_count();
	float get_block_list_offset(int p_idx);
	float get_width();
	float get_block_list_width(BlockList *p_block);
	QColor get_block_list_color(BlockList *p_block);
	void add_block_to_selection(int p_list_index,int p_block); // add twice to deselect
	void clear_selection();
	bool is_block_selected(int p_list,int p_block);
	bool get_screen_to_blocklist_and_tick( int p_x, int p_y, int *p_blocklist, Tick *p_tick );
	bool get_click_location( int p_x, int p_y, int *p_blocklist, int *p_block );

	
	
	
public:
	GlobalView(QWidget *p_widget,Song *p_song);
	~GlobalView();

	
};

};
#endif	//_GLOBALVIEW_H_
