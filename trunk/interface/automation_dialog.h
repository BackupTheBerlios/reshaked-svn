//
// C++ Interface: automation_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUTOMATION_DIALOG_H
#define RESHAKEDAUTOMATION_DIALOG_H


#include "engine/track.h"
#include <Qt/qmenu.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AutomationPopup : public QMenu {
	Q_OBJECT
			    
	class QActionAutomation : public QAction {
				    
		String path;
	public:
		String get_path() { return path; }
		QActionAutomation(QMenu* p_menu,String p_path) : QAction(p_menu) { path=p_path; }; //construct with this parent
				    
	};				    
			    
	
	Track *track;
signals:
	void attempt_automation_add_signal(String p_path);	
	void attempt_automation_remove_signal(String p_path);	
			
public:
	bool check_action(QAction *p_action);	
	
	void rebuild();
	AutomationPopup(QWidget *p_parent,Track *p_track);
	~AutomationPopup();

};

}

#endif
