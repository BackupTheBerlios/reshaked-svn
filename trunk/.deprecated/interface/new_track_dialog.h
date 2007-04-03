//
// C++ Interface: new_track_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDNEW_TRACK_DIALOG_H
#define RESHAKEDNEW_TRACK_DIALOG_H

#include <Qt/qlineedit.h>
#include <Qt/qcombobox.h>
#include <Qt/qdialog.h>
#include <Qt/qpushbutton.h>
#include "ui_blocks/helpers.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class NewTrackDialog : public QDialog {
			    
			    
			    
	static int track_idx;
	static int default_channels;
			    
	QLineEdit *line_edit;
	QComboBox *combo_channels;
	QComboBox *combo_type;
	QPushButton *acceptb;
	
	
	virtual void accept();		    
public:
	
	
	QString get_name();
	int get_channels();
	
	NewTrackDialog(QWidget *p_parent);
	~NewTrackDialog();

};

}

#endif
