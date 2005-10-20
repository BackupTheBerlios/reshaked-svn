//
// File: global_view.cpp
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//

#include "global_view.h"
#include "engine/track_pattern.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
namespace ReShaked {

	
/******************* HELPERS *********************/
	
bool GlobalView::is_block_selected(int p_list_index,int p_block) {
	
	int id=p_list_index+p_block*MAX_LISTS;
	std::set<int>::iterator I = selection.find(id);
	return (I!=selection.end());
}
	
void GlobalView::add_block_to_selection(int p_list_index,int p_block) {
	
	int id=p_list_index+p_block*MAX_LISTS;
	std::set<int>::iterator I = selection.find(id);
	if (I==selection.end())
		selection.insert(id);
	else {
		selection.erase(I);
	}
	
	update();
}
void GlobalView::clear_selection() {
	
	selection.clear();
	update();
}
	
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


bool GlobalView::get_screen_to_blocklist_and_tick( int p_x, int p_y, int *p_blocklist, Tick *p_tick ) {


	/******* FIND BLOCKLISt ********/
	int which_blocklist=-1;
	int blcount=get_block_list_count();
	float ofs=0;
	for (int i=0;i<blcount;i++) {
	
		BlockList *blocklist=get_block_list(i);
	
		float f_from_x=ofs-display.ofs_x*display.zoom_width;
		float block_width=get_block_list_width(blocklist)*display.zoom_width;
		
		printf("comparing %i to %i\n",(int)(f_from_x+block_width),p_x);
		if ((int)(f_from_x+block_width)>p_x) {
			which_blocklist=i;
			break;
		}
		
		ofs+=block_width;
	}


	if (which_blocklist==-1)
		return true; //not fund

	/******* FIND TICK ********/
	
	double tick=p_y/display.zoom_height-display.ofs_y;
	tick*=(double)TICKS_PER_BEAT;

//	printf("coords %i,%i, got block %i, beat %.2f\n",p_x,p_y,which_blocklist,(float)tick/(float)TICKS_PER_BEAT);	
		
	if (tick<0)
		return true;
		
	*p_blocklist=which_blocklist;
	*p_tick=(Tick)tick;
		
	return false;
}

bool GlobalView::get_click_location( int p_x, int p_y, int *p_blocklist, int *p_block ) {
	
	int blocklist;
	Tick tick;

	if (get_screen_to_blocklist_and_tick(p_x,p_y,&blocklist,&tick))
		return true;
		
	/* find right block */
	BlockList *bl = get_block_list( blocklist );
	ERR_FAIL_COND_V(bl==NULL,true);

	int block_idx = bl->get_block_idx_at_pos( tick );
	
	if (block_idx<0)
		return true;

	printf("block %i, tick %.2f - blocks idx %i\n",blocklist,(float)tick/(float)TICKS_PER_BEAT,block_idx);
		
	*p_blocklist=blocklist;
	*p_block=block_idx;
	
	return false;	
	
}

/***************** MOUSE *********************/

void GlobalView::mouseMoveEvent ( QMouseEvent * e ) {
	
		
}
void GlobalView::mousePressEvent ( QMouseEvent * e ) {
	
	int blocklist=-1,block=-1;
	if (get_click_location(e->x(),e->y(),&blocklist,&block))
		return; //no location
	
	add_block_to_selection(blocklist,block);
}
void GlobalView::mouseReleaseEvent ( QMouseEvent * e ) {
	
	
	
}

/****************** PAINTING ********************/

void GlobalView::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));	
	
	float ofs=0;
	int blocklist_count=get_block_list_count();
	for (int i=0;i<blocklist_count;i++) {
		
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
	
			QColor col=get_block_list_color(blocklist);
			if (is_block_selected(i,j))
				col=QColor(255,0,0);
			p.fillRect((int)f_from_x,(int)f_from_y,(int)f_width,(int)f_height,col);	

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
