//
// C++ Interface: blocklist_ui_base
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_BASE_H
#define RESHAKEDBLOCKLIST_UI_BASE_H

#include <Qt/qwidget.h>
#include "editor/editor.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/

class BlockListUI_Base : public QWidget {
Q_OBJECT			    
	int current_line;
	Editor *_editor;
	bool _mouse_selecting;
protected:	
	
	
	void wheelEvent ( QWheelEvent * e );

	virtual void get_pos_at_pointer(QPoint p_pointer, int *p_blocklist,int *p_column, int *p_row)=0;
	
	void mouse_selection_begin(QPoint p_pos);
	void mouse_selection_update_check();
	void mouse_selection_end();
	
	int get_current_line();
	void set_current_line(int p_line);
	
	virtual void fix_pre_scroll(int p_scroll);
	virtual bool can_scroll();
	
signals: 
	
	void blocklist_ui_under_cursor_request_signal(BlockListUI_Base*p_ui);
	
public:
	void set_blocklist_ui_under_cursor(BlockListUI_Base *p_ui, QPoint p_pos);
	
	void update_viewport_pos(int p_new_line);
	BlockListUI_Base(QWidget *p_parent, Editor *p_editor);

    ~BlockListUI_Base();

};

}

#endif
