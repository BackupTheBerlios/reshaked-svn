//
// C++ Implementation: tab_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tab_box.h"
#include <stdio.h>

namespace GUI {

void TabBox::add_frame(String p_label,Frame *p_child,bool p_at_top) {
	
	tabs->add_tab(p_label,p_at_top);
	stack->add_frame(p_child,p_at_top);
}
		
void TabBox::raise(int p_index) {
	
	stack->raise(p_index);
}
void TabBox::raise_frame(Frame *p_child) {
	
	stack->raise_frame(p_child);
}


void TabBox::skin_changed() {
	
//	printf("Skinchange in tabbox\n");
	set_separation( constant( C_TABBOX_SEPARATION ) );
	stack->set_style( stylebox( SB_TABBOX ), true );
	Container::skin_changed();	
}

void TabBox::set_in_window() {
	
	set_separation( constant( C_TABBOX_SEPARATION ) );
	stack->set_style( stylebox( SB_TABBOX ), true );
	
}
TabBox::TabBox() {
	
	tabs=BoxContainer::add( new TabBar );
	stack=BoxContainer::add( new StackContainer,1 );
	tabs->tab_changed_signal.connect( stack, &StackContainer::raise );
	
}


TabBox::~TabBox()
{
}


}
