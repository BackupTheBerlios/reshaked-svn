//
// File: global_view.cpp
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//

#include "global_view.h"
#include "engine/track_pattern.h"
#include <Qt/qpainter.h>

namespace ReShaked {

	
BlockList* GlobalView::get_block_list(int p_idx) {
	
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (blocks==p_idx)
			return t;
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
		
			if (blocks==p_idx)
				return t->get_automation(j);
			blocks++;
		}
	}
	
	return NULL;
	
}
int GlobalView::get_block_list_count() {
	
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
		
			blocks++;
		}
	}
	
	return blocks;
}
float GlobalView::get_block_list_offset(int p_index) {
	
	float offset=0;
	int block=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (p_index==block)
			return offset;
		block++;
		offset+=1;
		
		
		for (int j=0;j<t->get_automation_count();j++) {
		
			if (p_index==block)
				return offset;
			block++;
			offset+=0.5;
		}
	}

	
	return offset; //total size
}

float GlobalView::get_block_list_width(BlockList *p_block) {
	
	if (dynamic_cast<Automation*>(p_block))
		return 0.5;
	else
		return 1;
	
}
QColor GlobalView::get_block_list_color(BlockList *p_block) {

	if (dynamic_cast<Automation*>(p_block))
		return QColor(255,255,0);
	else if (dynamic_cast<Track_Pattern*>(p_block))
		return QColor(89,255,78);
	else 
		return QColor(255,55,78);
	
}
void GlobalView::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));	
	
	float ofs=0;
	for (int i=0;i<get_block_list_count();i++) {
		
		BlockList *blocklist=get_block_list(i);
		float f_from_x=ofs-display.ofs_x*display.zoom_width;
		float block_width=get_block_list_width(blocklist);
		float f_width=block_width*display.zoom_width;
		
		p.setPen(QColor(255,255,255));
		p.drawLine(f_from_x+f_width,0,f_from_x+f_width,height());
		for (int j=0;j<blocklist->get_block_count();j++) {
			
			BlockList::Block *block=blocklist->get_block(j);
			
			float f_from_y=((float)blocklist->get_block_pos(j)/(float)(TICKS_PER_BEAT))*display.zoom_height-display.ofs_y*display.zoom_height;
			float f_height=((float)blocklist->get_block(j)->get_length()/(float)(TICKS_PER_BEAT))*display.zoom_height;
			
			p.fillRect((int)f_from_x,(int)f_from_y,(int)f_width,(int)f_height,get_block_list_color(blocklist));	

		}
		
		ofs+=f_width;
		
	}
}

GlobalView::GlobalView(QWidget *p_widget,Song *p_song) : QWidget(p_widget)
{
	// TODO: put constructor code here

	setBackgroundRole(QPalette::NoRole);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	song=p_song;

	display.zoom_width=40;
	display.zoom_height=5;
	display.ofs_x=0;
	display.ofs_y=0;

}


GlobalView::~GlobalView()
{
	
	// TODO: put destructor code here
}


}
