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

namespace ReShaked {

/**
	@author red <red@killy>
*/

class BlockListUI_Base : public QWidget {
			    
	int current_line;
protected:	
	
	int get_current_line();
	void set_current_line(int p_line);
	
	virtual void fix_pre_scroll(int p_scroll);
	virtual bool can_scroll();
	
public:
	
	void update_viewport_pos(int p_new_line);
	BlockListUI_Base(QWidget *p_parent);

    ~BlockListUI_Base();

};

}

#endif
