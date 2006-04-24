//
// C++ Interface: blocklist_ui_list
//
// Description:
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_LIST_H
#define RESHAKEDBLOCKLIST_UI_LIST_H

#include "interface/blocklist_ui_base.h"
#include "ui_blocks/helpers.h"
#include <Qt/qscrollarea.h>
#include <Qt/qlayout.h>
#include <Qt/qframe.h>
#include <Qt/qcombobox.h>
#include <Qt/qpushbutton.h>
#include <Qt/qmenubar.h>
#include "editor/editor.h"
#include "interface/rowlist_display.h"
#include "interface/track_top.h"
#include "interface/editor_play_position.h"
#include "ui_blocks/property_editors.h"
#include "engine/property_edit_updater.h"
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_combo.h"
#include "ui_blocks/pixmap_scrollbar.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUIList : public QFrame {

	Q_OBJECT

friend class MainWindow;			
	std::vector<BlockListUI_Base*> block_list_ui_list;
	std::vector<TrackTop*> track_tops;
	std::vector<PropertyEditor*> property_editors;
	std::vector<PropertyEditSliderVU*> slider_vus;
	
	QWidget *hbox;
	QHBoxLayout *hbox_layout;
	QScrollArea *scrollarea;
	PixmapScrollBar *v_scroll;
	PixmapScrollBar *h_scroll;

	RowListDisplay *row_display;
	Editor *editor;
	QComboBox *snap;
	PropertyEditUpdater *property_ui_updater;
	

	PixmapButton *edit_mask;
	PixmapLabel *mask_label;
	PixmapButton *play_cursor;
	PixmapButton *play_block;
	PixmapLabel *edit_button;
	PixmapButton *automation_envelope;
	PixmapButton *automation_stream;
	PixmapCombo *cursor_stepping;
	PixmapButton *midi_in_edit;;
	PixmapButton *poly_input;
	PixmapCombo *snap_config;

	QMenu *edit_menu;
	
	EditorPlayPosition *play_position;
	
	bool scrolling;
	
	void fill_hb_top(QWidget* p_hb_top);
	
	enum EditMenuAction {
		ACTION_SET_SELECTION_BEGIN,
		ACTION_SET_SELECTION_END,
		ACTION_SELECT_COLUMN_ALL,
		ACTION_CLEAR_SELECTION,
		/***/
		ACTION_COPY,
		ACTION_CUT,
		ACTION_PASTE,
		ACTION_PASTE_INSERT,
		ACTION_PASTE_MIX,
		/***/
		ACTION_EDIT_MARKER,
		ACTION_SET_LOOP_BEGIN,
		ACTION_SET_LOOP_END,
		ACTION_SELECTION_TO_LOOP,
		/**/
		ACTION_SELECTION_TO_BLOCK,
		/**/
		ACTION_SELECTION_RAISE_SEMITONE,
		ACTION_SELECTION_LOWER_SEMITONE,
		ACTION_SELECTION_SCALE_VOLUMES,
		ACTION_SELECTION_SET_VOLUMES_TO_MASK,
		/**/
		ACTION_SELECTION_QUANTIZE_UP,
		ACTION_SELECTION_QUANTIZE_NEAREST,
		ACTION_SELECTION_QUANTIZE_DOWN
	};

	
	void property_editor_property_edited(PropertyEditor* p_editor,double p_old_val);
public:	
	static void property_editor_property_edited_callback(void *_this,PropertyEditor* p_editor,double p_old_val);
	
public slots:


	void ensure_cursor_visible();
	void cursor_changed_blocklist();
	void repaint_track_list();
	void update_track_list();
	void vscroll_track_list();
	void repaint_names();
	void update_h_scroll();
	void snap_changed_slot(int p_to_idx);
	void v_scrollbar_changed(int p_scroll);
	void update_blocklist_list(const std::list<int>& p_list);
	
	void play_cursor_slot();
	void play_block_slot();
	
	void show_edit_menu();
	void edit_menu_selected_item(int p_item);
	
	void update_play_position();
	void update_vus();
	
	void update_top_bar();
	
	void h_qscrollbar_range_changed(int p_min,int p_max);
	void h_qscrollbar_changed(int p_val);
	
	void blocklist_ui_under_cursor_request_signal(BlockListUI_Base*p_ui);	
	
	void volume_mask_togled(bool);
	
	void poly_input_toggle(bool p_enabled);
	void midi_in_toggle(bool p_enabled);
	
	void cursor_step_changed(int p_step);
	
	void scale_volume_slot();
	void edit_marker_slot();
	
public:

	BlockListUI_Base *get_blocklist_ui(int p_idx);
	
	BlockListUIList(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_property_ui_updater);
	~BlockListUIList();

};

}

#endif
