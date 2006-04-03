//
// C++ Interface: keyboard_regions_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDKEYBOARD_REGIONS_EDITOR_H
#define RESHAKEDKEYBOARD_REGIONS_EDITOR_H


#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include "dsp/region.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class KeyboardRegionsEditor : public QWidget{
	
	Q_OBJECT
public:		
	enum {
			
		MAX_NOTES=120
	};
	
	enum Mode {
		MODE_SELECT,
		MODE_ADD,
		MODE_REMOVE
	};
	
	struct Skin {
		
		QPixmap bg;
		QColor region;
		QColor region_selected;
		QColor border;
	};
private:
	
	Mode mode;
	Skin skin;
	
	RegionMap *region_map;
	
	
	void paintEvent(QPaintEvent *e);
	
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void leaveEvent ( QEvent * );
	int selected_region;
	
signals:
			
	void region_selected_signal(int p_which);
	void mouse_over_note_signal(int p_note);
		
public:

	void set_selected_region(int p_which);
	int get_selected_region();
	
	void set_mode(Mode p_mode);
	Mode get_mode();
	
	void set_region_map(RegionMap *p_region_map);
	
	KeyboardRegionsEditor(QWidget *p_parent,const Skin &p_skin);
	~KeyboardRegionsEditor();

};

}

#endif
