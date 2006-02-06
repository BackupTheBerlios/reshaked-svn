//
// C++ Interface: info_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDINFO_EDITOR_H
#define RESHAKEDINFO_EDITOR_H

#include <Qt/qdialog.h>
#include <Qt/qlineedit.h>
#include <Qt/qtextedit.h>
#include "engine/song.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class InfoEditor : public QDialog {
	
	QLineEdit *title;
	QLineEdit *author;
	QTextEdit *notes;
	
public:
	
	QString get_title();
	QString get_author();
	QString get_notes();
	InfoEditor(QWidget *p_parent,Song *p_song);
	~InfoEditor();

};

}

#endif
