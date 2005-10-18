//
// C++ Interface: snap_toolbar
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcombobox.h>

#ifndef EDIT_TOOLBAR_H
#define EDIT_TOOLBAR_H


namespace ReShaked {

class EditToolbar : public QToolBar {

	Q_OBJECT
	
	enum ComboItems {
		ITEM_1,
		ITEM_2,
		ITEM_3,
		ITEM_4,
		ITEM_6,
		ITEM_8,
		ITEM_12,
		ITEM_16,
		ITEM_24,
		ITEM_32,
		ITEM_48,
		ITEM_64,
		ITEM_CUSTOM,
	};
		
	static int snap_val[ITEM_CUSTOM];
		
	QSpinBox *octave_spin;
	
	QComboBox *snap_combo;
	
	int current_snap;

protected slots:
		
	void snapSelectedSlot(int p_idx);			
signals:
	
	void snapSelectedSignal(int p_snap);
			
public:
	
	EditToolbar(QMainWindow *p_parent);
};


};


#endif