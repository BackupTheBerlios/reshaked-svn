//
// C++ Implementation: snap_toolbar
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//



#include "edit_toolbar.h"
#include <qinputdialog.h>

namespace ReShaked {

int EditToolbar::snap_val[ITEM_CUSTOM] = {
	1,
	2,
	3,
	4,
	6,
	8,
	12,
	16,
	24,
	32,
	48,
	64
};

void EditToolbar::snapSelectedSlot(int p_idx) {
	
	if (p_idx<0)
		return;
	
	int new_snap=1;
	
	if (p_idx<ITEM_CUSTOM) {
		
		
		new_snap=snap_val[p_idx];
		
		
	} else {
		bool ok;
		new_snap = QInputDialog::getInteger ( "Snap Setting", "Enter new Snap Value:",current_snap,1,64,1,&ok,this); if (!ok)
			return;		

		snap_combo->lineEdit()->setText(QString::number(new_snap));			
	}
	
	current_snap=new_snap;
	snapSelectedSignal(new_snap);
	
}

EditToolbar::EditToolbar(QMainWindow *p_parent) : QToolBar(p_parent) {
	
	
	setLabel("Editing Options");
	

	new QLabel("Octave: ",this);
	octave_spin = new QSpinBox(0,9,1,this);
	octave_spin->setValue(4);
	octave_spin->setFocusPolicy(NoFocus);
	new QLabel("  Snap: ",this);
	snap_combo = new QComboBox(true,this);
	for (int i=0;i<ITEM_CUSTOM;i++) {
		
		snap_combo->insertItem(QString::number(snap_val[i]));			
	}
	
	snap_combo->insertItem("Custom...");
	snap_combo->lineEdit()->setReadOnly(true);
	
	QObject::connect(snap_combo,SIGNAL(activated(int)),this,SLOT(snapSelectedSlot(int)));
	
	current_snap=4;
	snap_combo->setFocusPolicy(NoFocus);
	snap_combo->lineEdit()->setFocusPolicy(NoFocus);
	
}



};