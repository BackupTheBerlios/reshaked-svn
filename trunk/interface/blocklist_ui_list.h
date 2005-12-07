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
#include "editor/editor.h"
#include "interface/rowlist_display.h"
#include "interface/track_top.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUIList : public QFrame {

	Q_OBJECT

	std::vector<BlockListUI_Base*> block_list_ui_list;
	std::vector<TrackTop*> track_tops;
	
	QWidget *hbox;
	QHBoxLayout *hbox_layout;
	QScrollArea *scrollarea;
	QScrollBar *v_scroll;

	RowListDisplay *row_display;
	Editor *editor;
	QComboBox *snap;
	bool scrolling;

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
	
public:

	BlockListUIList(QWidget *p_parent,Editor *p_editor);
	~BlockListUIList();

};

}

#endif
