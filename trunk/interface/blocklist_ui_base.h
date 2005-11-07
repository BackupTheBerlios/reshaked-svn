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
public:
    BlockListUI_Base(QWidget *p_parent);

    ~BlockListUI_Base();

};

}

#endif
