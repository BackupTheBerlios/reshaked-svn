//
// C++ Interface: blocklist_separator
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_SEPARATOR_H
#define RESHAKEDBLOCKLIST_SEPARATOR_H

#include <Qt/qwidget.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockList_Separator : public QWidget {
			    
	enum {
		WIDTH=16,
		OFFSET=32,
	};
	
	void paintEvent(QPaintEvent *pe);
	
	QString string;
public:
	
	BlockList_Separator(QWidget *p_parent,QString p_string);
	~BlockList_Separator();

};

}

#endif
