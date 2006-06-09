//
// C++ Implementation: global_view_cursor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_view_cursor.h"
#include <Qt/qframe.h>
namespace ReShaked {

void GlobalViewCursor::add_button(GlobalView::EditMode p_mode,PixmapsList p_pixmap_fg,PixmapsList p_pixmap_bg,QString p_hint) {

	PixmapButton::Skin skin(GET_QPIXMAP(p_pixmap_fg),GET_QPIXMAP(p_pixmap_bg));
	mode_button[p_mode]= new PixmapButton(this,skin,PixmapButton::TYPE_TOGGLE);
	connect_bind_int(mode_button[p_mode],SIGNAL(mouse_pressed_signal()),this,SLOT(mode_selected(int)),p_mode);
	mode_button[p_mode]->setToolTip(p_hint);

}

void GlobalViewCursor::mode_selected(int p_mode) {
	
	for (int i=0;i<MAX_MODES;i++) {
		
		if (i==p_mode) 
			mode_button[i]->set_pressed(true);
		else
			mode_button[i]->set_pressed(false);
	}
	mode=(GlobalView::EditMode)p_mode;
	edit_mode_changed_signal( mode );
}

GlobalViewCursor::GlobalViewCursor(QWidget *p_parent) : CHBox(p_parent) {
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__LEFT));
	add_button(GlobalView::EDIT_MODE_SELECT,THEME_GLOBAL_TOOLBAR__SELECT,THEME_GLOBAL_TOOLBAR__SELECT_ACTIVE,"Select Blocks (Click)\nAdd Blocks (Ctrl+Click and Motion)\nMove Blocks (Drag)\nCopy Blocks (Shift+Drag)\nCopy-Link Blocks (Shift+Ctrl+Drag)");
	add_button(GlobalView::EDIT_MODE_ADD_BLOCK,THEME_GLOBAL_TOOLBAR__ADD,THEME_GLOBAL_TOOLBAR__ADD_ACTIVE,"Add Blocks");
	add_button(GlobalView::EDIT_MODE_COPY_BLOCK,THEME_GLOBAL_TOOLBAR__COPY,THEME_GLOBAL_TOOLBAR__COPY_ACTIVE,"Copy Blocks");
	add_button(GlobalView::EDIT_MODE_COPY_LINK_BLOCK,THEME_GLOBAL_TOOLBAR__COPY_LINK,THEME_GLOBAL_TOOLBAR__COPY_LINK_ACTIVE,"Copy Link Blocks");
	delete_selected=new PixmapButton(this,PixmapButton::Skin( GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__ERASER),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__ERASER_PUSHED)));;
	QObject::connect(delete_selected,SIGNAL(mouse_pressed_signal()),this,SIGNAL(delete_clicked_signal()));
	delete_selected->setToolTip("Erase Selected Blocks (Delete)");
		
	select_linked=new PixmapButton(this,PixmapButton::Skin( GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__SELECT_LINKED),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__SELECT_LINKED_PUSHED)));;
	QObject::connect(select_linked,SIGNAL(mouse_pressed_signal()),this,SIGNAL(select_linked_signal()));
	select_linked->setToolTip("Select Linked Blocks\n to a Selected Block");
	unlink_selected=new PixmapButton(this,PixmapButton::Skin( GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__UNLINK_SELECTED),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__UNLINK_SELECTED_PUSHED)));;
	QObject::connect(unlink_selected,SIGNAL(mouse_pressed_signal()),this,SIGNAL(unlink_selected_signal()));
	unlink_selected->setToolTip("Unlink Selected Blocks");
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__SEPARATOR));
	
	
	PixmapButton *set_repeat=new PixmapButton(this,PixmapButton::Skin( GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__BLOCK_SET_REPEAT),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__BLOCK_SET_REPEAT_PUSHED)));;
	QObject::connect(set_repeat,SIGNAL(mouse_pressed_signal()),this,SIGNAL(repeat_set_signal()));
	set_repeat->setToolTip("Set Repeat on Selected Blocks");
	
	PixmapButton *unset_repeat=new PixmapButton(this,PixmapButton::Skin( GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__BLOCK_UNSET_REPEAT),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__BLOCK_UNSET_REPEAT_PUSHED)));;
	QObject::connect(unset_repeat,SIGNAL(mouse_pressed_signal()),this,SIGNAL(repeat_unset_signal()));
	unset_repeat->setToolTip("Clear Repeat on Selected Blocks");
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__HSPACER),PixmapLabel::EXPAND_TILE_H);
	
	
	zoom = new PixmapSlider(this,PixmapSlider::Skin(GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__ZOOM_BG),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__ZOOM_FG),GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__ZOOM_GRABBER)),PixmapSlider::TYPE_HORIZONTAL,1,1);
	QObject::connect(zoom,SIGNAL(value_changed_signal( float )),this,SIGNAL(zoom_changed(float)));
	zoom->setToolTip("Zoom View");
	new PixmapLabel(this,GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__ZOOM_ICON));
	new PixmapLabel(this,GET_QPIXMAP(THEME_GLOBAL_TOOLBAR__RIGHT));
	
	layout()->setMargin(0);
	layout()->setSpacing(0);
	mode_selected(0);
	setBackgroundRole(QPalette::NoRole);
	zoom->set_value( 0.35 );
	zoom_changed( 0.35);

}


GlobalViewCursor::~GlobalViewCursor()
{
}


}
