//
// C++ Implementation: edit_view_toolbar
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "edit_view_toolbar.h"

#include "widgets/button.h"
#include "widgets/separator.h"
#include "widgets/icon.h"
#include "gui_custom/pixmap_data.h"
#include "containers/center_container.h"

namespace ReShaked {

	
void EditViewToolbar::action_callback(int p_action) {
	
	
}

void EditViewToolbar::set_in_window() {
		
	Button *b;
	b = add( new MenuButton( PixmapData::get_pixmap( PIXMAP_ICON_PLAY_FROM_CURSOR ) ) ); 
	b->pressed_signal.connect( Method( Method1<int>( this, &EditViewToolbar::action_callback ), (int)PLAY_FROM_CURSOR ) );
	        
	b = add( new MenuButton( PixmapData::get_pixmap( PIXMAP_ICON_PLAY_CURRENT_BLOCK ) ) ); 
	b->pressed_signal.connect( Method( Method1<int>( this, &EditViewToolbar::action_callback ), (int)PLAY_BLOCK_LOOPED ) );
	
	add( new VSeparator );
	
	HBoxContainer *hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	selection_menu = hb->add( new MenuBox("Selection" ) );
	
	selection_menu->add_item("Block Begin",SELECTION_BEGIN);
	selection_menu->add_item("Block end",SELECTION_END);
	selection_menu->add_item("Block/Column",SELECTION_BLOCK_COLUMN);
	selection_menu->add_item("Clear",SELECTION_CLEAR);
	selection_menu->add_separator();
	selection_menu->add_item("Create Block",SELECTION_CREATE_BLOCKS);
	selection_menu->add_item("Set to Loop Begin/End",SELECTION_MAKE_LOOP);
	selection_menu->add_separator();
	selection_menu->add_item("Scale Volumes",SELECTION_SCALE_VOLUMES);
	selection_menu->add_item("Apply Volume Mask",SELECTION_APPLY_VOLUME_MASK);
	selection_menu->add_separator();
	selection_menu->add_item("Quantize Up",SELECTION_QUANTIZE_UP);
	selection_menu->add_item("Quantize Nearest",SELECTION_QUANTIZE_NEAREST);
	selection_menu->add_item("Quantize Down",SELECTION_QUANTIZE_DOWN);
	
	edit_menu = hb->add( new MenuBox("Edit" ) );
	
	
	edit_menu->add_item("Copy",EDIT_COPY);
	edit_menu->add_item("Cut",EDIT_CUT);
	edit_menu->add_item("Paste",EDIT_PASTE);
	edit_menu->add_item("Paste Insert",EDIT_PASTE_INSERT);
	edit_menu->add_item("Paste Mix",EDIT_PASTE_MIX);
	edit_menu->add_separator();
	edit_menu->add_item("Marker",EDIT_MARKER);
	edit_menu->add_item("Bar Length",EDIT_BAR_LENTH);
	edit_menu->add_separator();
	edit_menu->add_item("Set Loop Begin",EDIT_SET_LOOP_BEGIN);
	edit_menu->add_item("Set Loop End",EDIT_SET_LOOP_END);
	edit_menu->add_separator();
	edit_menu->add_item("Transpose Up Semitone",EDIT_TRANSPOSE_UP_SEMITONE);
	edit_menu->add_item("Transpose Down Semitone",EDIT_TRANSPOSE_DOWN_SEMITONE);
	edit_menu->add_item("Transpose Up Octave",EDIT_TRANSPOSE_UP_OCTAVE);
	edit_menu->add_item("Transpose Down Octave",EDIT_TRANSPOSE_DOWN_OCTAVE);
	                      
	add( new VSeparator );
	
	hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_AUDIO ) ) );
	
	volume_mask = hb->add( new SpinBox );
	volume_mask->get_range()->set_min(0);
	volume_mask->get_range()->set_max(99);
	volume_mask->get_range()->set_step(1);
	
	add( new VSeparator );
	
	hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_CURSOR_STEP ) ) );
	
	cursor_step = hb->add( new ComboBox );
	
	for (int i=0;i<10;i++)
		cursor_step->add_string( String::num(i) );
		
	add( new VSeparator );
	
	hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_MIDI_IN ) ) );
	
	midi_mode = hb->add( new ComboBox );
	
	midi_mode->add_string("Disabled");
	midi_mode->add_string("Mono Mode");
	midi_mode->add_string("Poly Mode");
		
	add( new VSeparator );
	
	add( new Widget, 1);
	
	
	hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	midi_mode = hb->add( new ComboBox );
	midi_mode->set_minimum_size( Size( 120, 0 ));
	midi_mode->add_string("1/1 of Beat ");
	midi_mode->add_string("1/2 of Beat ");
	midi_mode->add_string("1/3 of Beat ");
	midi_mode->add_string("1/4 of Beat ");
	midi_mode->add_string("1/6 of Beat ");
	midi_mode->add_string("1/8 of Beat ");
	midi_mode->add_string("1/12 of Beat ");
	midi_mode->add_string("1/16 of Beat ");
	midi_mode->add_string("1/24 of Beat ");
	midi_mode->add_string("1/32 of Beat ");
	midi_mode->add_string("1/48 of Beat ");
	midi_mode->add_string("1/64 of Beat ");
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_ZOOM ) ) );
	
}
	
EditViewToolbar::EditViewToolbar(GUI_UpdateNotify *p_notifier,Editor *p_editor)
{
	editor=p_editor;
}


EditViewToolbar::~EditViewToolbar()
{
}


}
