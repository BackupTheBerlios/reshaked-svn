//
// C++ Implementation: editor_play_position
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "editor_play_position.h"
#include "ui_blocks/visual_settings.h"

namespace ReShaked {


int EditorPlayPosition::get_playback_snap_pos() {
	
	if (editor->get_song()->get_song_playback().get_status()!=SongPlayback::STATUS_PLAY) {
		
		return -1;
	}
	
	return editor->get_cursor().ticks_to_snap( editor->get_song()->get_song_playback().get_current_tick_from() );	
}
bool EditorPlayPosition::is_playback_visible() {
	
		
	int pos=get_playback_snap_pos()-editor->get_cursor().get_window_offset();
	if (pos<0)
		return false;
	if (pos>editor->get_cursor().get_window_size())
		return false;
	return true;
}

void EditorPlayPosition::check_pos_changed() {
	
	if (last_drawn_play_pos==get_playback_snap_pos())
		return;
	
	if (!is_playback_visible() && !last_draw_play_visible)
		return;
	
		update();
	
}

void EditorPlayPosition::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	
	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int visible_rows=height()/rowsize;
	
	int loop_marker_height=GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_BEGIN).height();
	int loop_begin_pos=(editor->get_song()->get_loop_begin()*editor->get_cursor().get_snap()-editor->get_cursor().get_window_offset())*rowsize;
	int loop_end_pos=(editor->get_song()->get_loop_end()*editor->get_cursor().get_snap()-editor->get_cursor().get_window_offset())*rowsize;
	
	if (loop_begin_pos<(-loop_marker_height))
		loop_begin_pos=-loop_marker_height;
	if (loop_end_pos>(height()+loop_marker_height))
		loop_end_pos=height()+loop_marker_height;
	
	p.fillRect(0,loop_begin_pos,width(),loop_end_pos-loop_begin_pos,QColor(50,50,50));
	
	p.drawPixmap(0,loop_begin_pos,GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_BEGIN));
	p.drawPixmap(0,loop_end_pos-loop_marker_height,GET_QPIXMAP(ICON_EDIT_VIEW_ROW_LOOP_END));
	

	last_draw_play_visible=is_playback_visible();
	int visible_pos=get_playback_snap_pos()-editor->get_cursor().get_window_offset();
	last_drawn_play_pos=visible_pos;
	
	if (!last_draw_play_visible)
		return;
	
	p.drawPixmap(0,visible_pos*rowsize,GET_QPIXMAP( ICON_EDIT_VIEW_ROW_PLAYED ) );

}

EditorPlayPosition::EditorPlayPosition(QWidget*p_parent,Editor *p_editor) : QWidget(p_parent)
{
	setFixedWidth( GET_QPIXMAP( ICON_EDIT_VIEW_ROW_PLAYED ).width() );
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	setBackgroundRole(QPalette::NoRole);
	editor=p_editor;
	last_drawn_play_pos=-1;
	last_draw_play_visible=true;
	setToolTip("Playback/Loop");
}


EditorPlayPosition::~EditorPlayPosition()
{
}


}
