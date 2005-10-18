//
// File: global_view.h
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//


#ifndef _GLOBALVIEW_H_
#define _GLOBALVIEW_H_

#include <Qt/qwidget.h>
#include "engine/song.h"

namespace ReShaked {
	
class GlobalView : public QWidget {
	
	Song *song;
	void paintEvent(QPaintEvent *p);
	
	struct DisplayParams {
		float zoom_width;
		float zoom_height;
		float ofs_x,ofs_y;
	} display;
	
	
	//helpers
	BlockList* get_block_list(int p_idx);
	int get_block_list_count();
	float get_block_list_offset(int p_idx);
	float get_width();
	float get_block_list_width(BlockList *p_block);
	QColor get_block_list_color(BlockList *p_block);
	
public:
	GlobalView(QWidget *p_widget,Song *p_song);
	~GlobalView();

	
};

};
#endif	//_GLOBALVIEW_H_
