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
	
	switch (p_action) {
		case PLAY_FROM_CURSOR: {
			
		} break;
		case PLAY_BLOCK_LOOPED: {
			
		} break;
		case SELECTION_BEGIN: {
			
		} break;
		case SELECTION_END: {
			
			editor->selection_begin();

		} break;
		case SELECTION_BLOCK_COLUMN: {
			
			editor->selection_end();
			
		} break;
		case SELECTION_CLEAR: {
			
			editor->disable_selection();

			
		} break;
		case SELECTION_CREATE_BLOCKS: {
			editor->selection_create_block();

		} break;
		case SELECTION_MAKE_LOOP: {
			
			editor->selection_to_loop();

		} break;
		case SELECTION_SCALE_VOLUMES: {
			
			scale_volumes();
		} break;
		case SELECTION_APPLY_VOLUME_MASK: {
			
			editor->selection_set_volumes_to_mask();
			
		} break;
		case SELECTION_QUANTIZE_UP: {
			
			editor->selection_quantize(Editor::QUANTIZE_UP);
			
		} break;
		case SELECTION_QUANTIZE_NEAREST: {
			
			editor->selection_quantize(Editor::QUANTIZE_NEAREST);
			
		} break;
		case SELECTION_QUANTIZE_DOWN: {
			
			editor->selection_quantize(Editor::QUANTIZE_DOWN);
			
		} break;
		case EDIT_COPY: {
			
			editor->selection_copy();
			
		} break;
		case EDIT_CUT: {
			
			editor->selection_zap();
			
		} break;
		case EDIT_PASTE: {
			
			editor->selection_paste();
			
		} break;
		case EDIT_PASTE_INSERT: {
			
			editor->selection_paste_insert();
			
		} break;
		case EDIT_PASTE_MIX: {
			
			editor->selection_paste_mix();
			
		} break;
		case EDIT_MARKER: {
			
			edit_marker_at_cursor();
				
		} break;
		case EDIT_BAR_LENTH: {
			
		} break;
		case EDIT_SET_LOOP_BEGIN: {
			
			editor->set_loop_begin_at_cursor();			
		} break;
		case EDIT_SET_LOOP_END: {
			
			editor->set_loop_end_at_cursor();
			
		} break;
		case EDIT_TRANSPOSE_UP_SEMITONE: {
			
			editor->selection_cursor_transpose_up();
			
		} break;
		case EDIT_TRANSPOSE_DOWN_SEMITONE: {
			
			editor->selection_cursor_transpose_down();
			
		} break;
		case EDIT_TRANSPOSE_UP_OCTAVE: {
			
		} break;
		case EDIT_TRANSPOSE_DOWN_OCTAVE: {
			
		} break;
	}	
}

void EditViewToolbar::cursor_set_step_callback(int p_step) {
	
	if (updating)
		return;
	editor->cursor_set_step(p_step);
}
void EditViewToolbar::volume_set_mask_callback(double p_mask) {
	
	if (updating)
		return;
	editor->set_volume_mask( (int)p_mask );
}


void EditViewToolbar::snap_changed_callback(int p_to_idx) {
	
	if (p_to_idx<0 || p_to_idx>=MAX_DIVISORS)
		return;
	
	editor->set_snap(divisors[p_to_idx]);
	
}

void EditViewToolbar::scale_volumes() {
	
	input_volume_scale->show("Scale Volumes by %",0,1000,1,last_scale);
	
	
}
void EditViewToolbar::edit_marker_at_cursor() {
	
	int beat=editor->get_cursor().get_beat();
	int marker_idx=editor->get_song()->get_marker_list().get_exact_index( beat );
	String current_text= (marker_idx>=0)?editor->get_song()->get_marker_list().get_index_value_w( marker_idx):"";
	
	input_marker->show("Insert Marker",current_text);
	
}
void EditViewToolbar::change_bar_len_at_cursor() {
	
	
}


void EditViewToolbar::update_components() {
	
	updating=true;
	cursor_step->select( editor->cursor_get_step() );
	volume_mask->get_range()->set( editor->get_volume_mask() );
	updating=false;
	
}

void EditViewToolbar::marker_changed_callback(String p_text) {
	
	int beat=editor->get_cursor().get_beat();
	int marker_idx=editor->get_song()->get_marker_list().get_exact_index( beat );
	
	if (p_text=="") {
		if (marker_idx>=0)
			editor->marker_remove( beat );
	} else 
		editor->marker_set( beat, p_text );
			
	
}
void EditViewToolbar::volume_scale_callback(double p_val) {
	
	editor->selection_volume_scale((int)p_val);	
	last_scale=p_val;
}
void EditViewToolbar::bar_len_changed_callback(double p_val) {
	
	//void bar_length_set(int p_at_beat,int p_len);
	
}


void EditViewToolbar::set_in_window() {
		
	Button *b;
	b = add( new MenuButton( PixmapData::get_pixmap( PIXMAP_ICON_PLAY_FROM_CURSOR ) ) ); 
	b->pressed_signal.connect( Method( Method1<int>( this, &EditViewToolbar::action_callback ), (int)PLAY_FROM_CURSOR ) );
	b->set_focus_mode( FOCUS_NONE );
	        
	b = add( new MenuButton( PixmapData::get_pixmap( PIXMAP_ICON_PLAY_CURRENT_BLOCK ) ) ); 
	b->pressed_signal.connect( Method( Method1<int>( this, &EditViewToolbar::action_callback ), (int)PLAY_BLOCK_LOOPED ) );
	b->set_focus_mode( FOCUS_NONE );
	
	add( new VSeparator );
	
	HBoxContainer *hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	selection_menu = hb->add( new MenuBox("Selection" ) );
	selection_menu->set_focus_mode( FOCUS_NONE );
	
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
	selection_menu->item_selected_signal.connect(this,&EditViewToolbar::action_callback);
	
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
	edit_menu->item_selected_signal.connect(this,&EditViewToolbar::action_callback);
	                      
	edit_menu->set_focus_mode( FOCUS_NONE );
	
	add( new VSeparator );
	
	hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_AUDIO ) ) );
	
	volume_mask = hb->add( new SpinBox );
	volume_mask->get_range()->set_min(0);
	volume_mask->get_range()->set_max(99);
	volume_mask->get_range()->set_step(1);
	volume_mask->get_range()->set( editor->get_volume_mask() );
	volume_mask->get_range()->value_changed_signal.connect( this, &EditViewToolbar::volume_set_mask_callback );
	
	add( new VSeparator );
	
	hb = add( new CenterContainer() )->set( new HBoxContainer );
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_CURSOR_STEP ) ) );
	
	cursor_step = hb->add( new ComboBox );
	
	for (int i=0;i<10;i++)
		cursor_step->add_string( String::num(i) );
	
	cursor_step->select( editor->cursor_get_step() );
	cursor_step->selected_signal.connect( this, &EditViewToolbar::cursor_set_step_callback );
	
		
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
	
	snap = hb->add( new ComboBox );
	snap->set_minimum_size( Size( 120, 0 ));
	snap->add_string("1/1 of Beat ");
	snap->add_string("1/2 of Beat ");
	snap->add_string("1/3 of Beat ");
	snap->add_string("1/4 of Beat ");
	snap->add_string("1/6 of Beat ");
	snap->add_string("1/8 of Beat ");
	snap->add_string("1/12 of Beat ");
	snap->add_string("1/16 of Beat ");
	snap->add_string("1/24 of Beat ");
	snap->add_string("1/32 of Beat ");
	snap->add_string("1/48 of Beat ");
	snap->add_string("1/64 of Beat ");
	snap->select(3);
	snap->selected_signal.connect( this, &EditViewToolbar::snap_changed_callback );
	
	hb->add( new Icon( PixmapData::get_pixmap( PIXMAP_ICON_ZOOM ) ) );
	
	input_bar_len = new NumericInputDialog( get_window() );
	input_bar_len->entered_number_signal.connect(this,&EditViewToolbar::bar_len_changed_callback);
	input_volume_scale = new NumericInputDialog( get_window() );
	input_volume_scale->entered_number_signal.connect(this,&EditViewToolbar::volume_scale_callback);
	input_marker = new StringInputDialog( get_window() );
	input_marker->entered_string_signal.connect(this,&EditViewToolbar::marker_changed_callback);
		
	
	updating=false;
}
	
EditViewToolbar::EditViewToolbar(GUI_UpdateNotify *p_notifier,Editor *p_editor)
{
	editor=p_editor;
	last_scale=100;
}


EditViewToolbar::~EditViewToolbar()
{
}


}
