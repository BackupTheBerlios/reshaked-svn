//
// C++ Implementation: keyboard_regions_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "keyboard_regions_editor.h"

#include <Qt/qpainter.h>
#include <Qt/qevent.h>


namespace ReShaked {


void KeyboardRegionsEditor::paintEvent(QPaintEvent *e) {
	
	
	int note_height=height()/MAX_NOTES;
	QPainter p(this);
	
	p.drawPixmap(0,0,skin.bg);
	
	if (!region_map)
		return;
	
	for (int i=0;i<region_map->get_stream_size();i++) {
		
		int begin=region_map->get_index_pos(i);
		int end=(i==region_map->get_stream_size()-1)?120:region_map->get_index_pos(i+1);
		
		int region_height=(end-begin)*note_height;
		int region_pos=begin*note_height;
		
		
		
		p.fillRect(0,region_pos,width(),region_height,(i==selected_region)?skin.region_selected:skin.region);
		p.setPen(skin.border);
		p.drawRect(0,region_pos,width(),region_height);
				
	}
	
}

void KeyboardRegionsEditor::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	if (!region_map)
		return;
	
	int note_height=height()/MAX_NOTES;
	int note=e->y()/note_height;
	
	switch(mode) {
			
		case MODE_SELECT: {
			
			int which=region_map->get_prev_index(note);
			if (which==-1)
				break;
			
			selected_region=which;
			region_selected_signal(which);
			update();
			
		} break;
		case MODE_ADD: {
			
			if (region_map->get_exact_index(note)>=0)
				break; //cant add where already exists
					
			Region new_region;
			new_region.base_note=note;
			new_region.source_idx=0;
			region_map->insert(note,new_region);
			
			selected_region=region_map->get_exact_index(note);
			region_selected_signal(selected_region);
			update();
			
		} break;
		case MODE_REMOVE: {
			
			int which=region_map->get_prev_index(note);
			if (which==-1)
				break;
			
			region_map->erase_index(which);
			selected_region=which-1;
			region_selected_signal(selected_region);
			update();
			
		} break;
	}
}

void KeyboardRegionsEditor::mouseMoveEvent(QMouseEvent *e) {
	
	if (!region_map)
		return;
	
	int note_height=height()/MAX_NOTES;
	int note=e->y()/note_height;
	mouse_over_note_signal(note);	
	
}

void KeyboardRegionsEditor::leaveEvent ( QEvent * ) {
	
	mouse_over_note_signal(-1);	
}

void KeyboardRegionsEditor::set_selected_region(int p_which) {
	
	if (!region_map)
		return;
	
	
	selected_region=p_which;
	
	update();
	
}
int KeyboardRegionsEditor::get_selected_region() {
	
	return selected_region;
}


void KeyboardRegionsEditor::set_mode(Mode p_mode) {
	
	mode=p_mode;
}
KeyboardRegionsEditor::Mode KeyboardRegionsEditor::get_mode() {
	
	return mode;
}

void KeyboardRegionsEditor::set_region_map(RegionMap *p_region_map) {
	
	bool different=(region_map!=p_region_map);
					
	region_map=p_region_map;
	if (different && region_map) {
		if (region_map->get_stream_size()) 
			selected_region=0;
		else 
			selected_region=-1;
					
		region_selected_signal(selected_region);
	}
	
	update();
	
	
}


KeyboardRegionsEditor::KeyboardRegionsEditor(QWidget *p_parent,const Skin &p_skin) :QWidget(p_parent) {
	
	setFixedSize(p_skin.bg.size());
	
	mode=MODE_SELECT;
	skin=p_skin;
	region_map=NULL;
	selected_region=-1;
	setMouseTracking(true);
	
}


KeyboardRegionsEditor::~KeyboardRegionsEditor()
{
}


}
