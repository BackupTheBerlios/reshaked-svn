//
// C++ Implementation: audio_graph_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_graph_screen.h"
#include "widgets/bitmap_button.h"
#include "gui_common/common_skin.h"
#include "containers/center_container.h"
#include "containers/grid_container.h"

static void create_buttons(GUI::BoxContainer *p_container,GUI::ButtonGroup *p_group,int p_offset,int p_count) {

	GUI::HBoxContainer *hbc = p_container->add( new GUI::HBoxContainer );

	for(int i=0;i<p_count;i++) {
		
		GUI::BitmapButton *bb = hbc->add( new GUI::BitmapButton );
		
		bb->add_bitmap_override( GUI::BITMAP_BUTTON_NORMAL, BITMAP_LAYER_BUTTON_0 + i + p_offset );
		bb->add_bitmap_override( GUI::BITMAP_BUTTON_HOVER, BITMAP_LAYER_BUTTON_0_HOVER + i + p_offset);
		bb->add_bitmap_override( GUI::BITMAP_BUTTON_PRESSED, BITMAP_LAYER_BUTTON_0_PRESSED + i + p_offset);
		p_group->add_button( bb );
			
	};
	
	hbc->set_separation(0);

}

AudioGraphScreen::AudioGraphScreen(GUI_UpdateNotify *p_update_notify,Song *p_song) {

	song=p_song;

	GUI::HBoxContainer *hbc = add( new GUI::CenterContainer )->set(new GUI::HBoxContainer );
		
	GUI::BitmapButton *bb_all = new GUI::BitmapButton;
		
	bb_all->add_bitmap_override( GUI::BITMAP_BUTTON_NORMAL, BITMAP_LAYER_BUTTON_ALL );
	bb_all->add_bitmap_override( GUI::BITMAP_BUTTON_HOVER, BITMAP_LAYER_BUTTON_ALL_HOVER  );
	bb_all->add_bitmap_override( GUI::BITMAP_BUTTON_PRESSED, BITMAP_LAYER_BUTTON_ALL_PRESSED );
	
	layer_group.add_button( bb_all );
		
	GUI::VBoxContainer *vbc1 = hbc->add( new GUI::VBoxContainer );
	vbc1->set_separation(0);
		
	create_buttons(vbc1,&layer_group,0,4);
	create_buttons(vbc1,&layer_group,4,4);
	
	hbc->add( bb_all );
	
	GUI::VBoxContainer *vbc2 = hbc->add( new GUI::VBoxContainer );
	vbc2->set_separation(0);
	
	create_buttons(vbc2,&layer_group,8,4);
	create_buttons(vbc2,&layer_group,12,4);
	
	hbc->set_separation(0);
		

	scroll_box = add( new GUI::ScrollBox, 1 );
	audio_graph_widget = scroll_box->set( new AudioGraphWidget(p_update_notify,p_song) );
	scroll_box->set_expand_h(true);
	scroll_box->set_expand_v(true);
	scroll_box->set_scroll_h(true);
	scroll_box->set_scroll_v(true);	
	
}


AudioGraphScreen::~AudioGraphScreen()
{
}


