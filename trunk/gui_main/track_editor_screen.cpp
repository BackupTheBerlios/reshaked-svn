//
// C++ Implementation: track_editor_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor_screen.h"
#include "containers/grid_container.h"
#include "track_editor_pattern.h"
#include "widgets/label.h"

#include "gui_common/common_skin.h"
	
class _BlackWidget : public GUI::Widget {
	
	virtual void draw(const GUI::Point& p_global,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

		get_painter()->draw_fill_rect(GUI::Point(),p_size,color(COLOR_TRACK_EDITOR_BG));
	}
public:	
	_BlackWidget()  {
		
		set_bg_on_updates(false);
	}
};
	
void TrackEditorScreen::ensure_cursor_track_visible() {

	if (!hb)
		return;

	GUI::Rect cursor_track_rect;
	TrackEditor * cursor_track;
	
	if ( Editor::get_singleton()->get_cursor_track() < 0 || Editor::get_singleton()->get_cursor_track() >=track_editor_names.size() )
		return;
		
	cursor_track = track_editors[ Editor::get_singleton()->get_cursor_track() ];
	GUI::Widget* track_editor_name = track_editor_names[ Editor::get_singleton()->get_cursor_track() ];
	
	cursor_track_rect = GUI::Rect( cursor_track->get_global_pos(), cursor_track->get_size_cache() );
	
	cursor_track_rect = cursor_track_rect.merge( GUI::Rect( track_editor_name->get_global_pos(), track_editor_name->get_size_cache() ) );
	
	GUI::Rect scrollcontainer_rect=GUI::Rect( scroll->get_global_pos(), scroll->get_size_cache() );
	
	GUI::Rect hb_rect=GUI::Rect( hb->get_global_pos(), hb->get_size_cache() );
	
	if ( cursor_track_rect.pos.x < scrollcontainer_rect.pos.x) {
	
		scroll->get_h_range()->set( cursor_track_rect.pos.x - hb_rect.pos.x );
	}
	
	if ( cursor_track_rect.pos.x+cursor_track_rect.size.x > scrollcontainer_rect.pos.x + scrollcontainer_rect.size.x) {
	
		scroll->get_h_range()->set(cursor_track_rect.pos.x - hb_rect.pos.x - scrollcontainer_rect.size.x + cursor_track_rect.size.x );
	}
	
}
	
void TrackEditorScreen::track_block_changed(Track::Block *p_block) {


	ensure_cursor_track_visible();
	repaint(); // todo optimize later
}

void TrackEditorScreen::track_changed_callback(Track *p_track) {

	for (int i=0;i<track_editors.size();i++) {
	
		if (track_editors[i]->get_track()!=p_track)
			continue;
		track_editors[i]->check_minimum_size();
		track_editors[i]->update();
		
	}
}

void TrackEditorScreen::rebuild_queue() {

	get_window()->get_root()->update_signal.connect( this, &TrackEditorScreen::rebuild );
}

void TrackEditorScreen::rebuild() {

	/* check if it's REALLY necesary to rebuild */
	
	bool all_same=true;
	

	if (hb)
		delete hb;
		
	track_editors.clear();
	track_editor_names.clear();
	
	hb = vb_holder->add( new GUI::HBoxContainer,1 );
	hb->set_separation(0);
	
	for(int i=0;i<editor->get_song()->get_track_count();i++) {
	
		Track *t = editor->get_song()->get_track( i );
		TrackEditor *te=NULL;;
		
		PatternTrack *pt = dynamic_cast<PatternTrack*>(t);
		
		if (pt) {
			te = new TrackEditorPattern(pt);
			
		}
		
		if (!te)
			continue;
		
		TrackEditorTrackName *name = hb->add( new TrackEditorTrackName(te) );
		
		hb->add( te );
		
		track_editors.push_back( te );
		track_editor_names.push_back(name);
		
		
		te->mouse_selecting_signal.connect( this, &TrackEditorScreen::mouse_selection_update_callback );
		
		if (!t->is_collapsed() && te->is_visible() && editor->get_cursor_track()==i)
			te->get_focus();

	}
	
	hb->add( new _BlackWidget, 1 );
	
	ensure_cursor_track_visible();
}

void TrackEditorScreen::mouse_selection_update_callback(GUI::Point p_pos) {

	for (int i=0;i<track_editors.size();i++) {
	
		if (track_editors[i]->get_track()->is_collapsed())
			continue;
		GUI::Rect rect = GUI::Rect(track_editors[i]->get_global_pos(), track_editors[i]->get_size_cache() );
		
		if (p_pos.x>= rect.pos.x && p_pos.x<=rect.pos.x+rect.size.x) {
			// only do care about x
		
			track_editors[i]->mouse_select_notify( p_pos - rect.pos );
		}
	}
	
}
void TrackEditorScreen::block_changed_callback(Track::Block *p_block) {

	repaint();
}

void TrackEditorScreen::repaint() {

	for (int i=0;i<track_editors.size();i++) {
		track_editors[i]->update();
		track_editor_names[i]->update();
	}
		
	barbeat->update();
}

void TrackEditorScreen::cursor_track_changed_callback(int p_from,int p_to) {

	if (p_from<0 || p_from>=track_editors.size())
		return;
	if (p_to<0 || p_to>=track_editors.size())
		return;
	
	track_editors[p_from]->update();
	track_editors[p_to]->get_focus();
	track_editors[p_to]->update();

	ensure_cursor_track_visible();
}

void TrackEditorScreen::notify_enter() {

	for(int i=0;i<editor->get_song()->get_track_count();i++) {
	
		
		if (track_editors[i]->is_visible() && editor->get_cursor_track()==i)
			track_editors[i]->get_focus();

	}

}

TrackEditorScreen::TrackEditorScreen(Editor *p_editor,GUI_UpdateNotify *p_update_notify) {

	editor=p_editor;
	
	top = add( new TrackEditorTop(p_update_notify) );
	
	GUI::HBoxContainer *hbc = add( new GUI::HBoxContainer, 1 );
	
	hbc->set_separation(0);
	barbeat = hbc->add( new TrackEditorBarBeat );
	
	GUI::GridContainer *gc = hbc->add( new GUI::GridContainer(2),1 );
	scroll = gc->add( new GUI::ScrollContainer, true, true );
	gc->add( new GUI::Widget, false, true ); 
	GUI::ScrollBar *hsbar = gc->add( new GUI::HScrollBar, true, false );
	hsbar->set_range( scroll->get_h_range() );
	hsbar->set_auto_hide( true );
	
	
	vb_holder = scroll->set( new GUI::VBoxContainer );
	scroll->set_expand_v(true);
	scroll->set_expand_h(true);
	scroll->set_scroll_v(false);
	scroll->set_scroll_h(true);
	
	hb=NULL;
	
	p_update_notify->track_list_changed_signal.connect( this, &TrackEditorScreen::rebuild_queue );
	p_update_notify->cursor_track_changed_signal.connect( this, &TrackEditorScreen::cursor_track_changed_callback );
	p_update_notify->window_snap_changed_signal.connect( this, &TrackEditorScreen::repaint );
	p_update_notify->window_offset_changed_signal.connect( this, &TrackEditorScreen::repaint );
	p_update_notify->track_list_repaint_signal.connect( this, &TrackEditorScreen::repaint );
	p_update_notify->selection_changed_signal.connect( this, &TrackEditorScreen::repaint );
	p_update_notify->track_block_changed_signal.connect( this, &TrackEditorScreen::block_changed_callback );
	p_update_notify->track_changed_signal.connect( this, &TrackEditorScreen::track_changed_callback );
	
	rebuild();
}


TrackEditorScreen::~TrackEditorScreen() {
}


