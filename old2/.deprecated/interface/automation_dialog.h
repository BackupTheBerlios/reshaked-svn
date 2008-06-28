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
#include <Qt/qcombobox.h>
#include <Qt/qslider.h>
#include <Qt/qdialog.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AutomationPopup : public QMenu {
	Q_OBJECT
			    
	class QActionAutomation : public QAction {
				    
		int property_idx;
	public:
		int get_property_idx() { return property_idx; }
		QActionAutomation(QMenu* p_menu,int p_property_idx) : QAction(p_menu) { property_idx=p_property_idx; }; //construct with this parent
				    
	};				    
			    
	QAction *manage;
	Track *track;
signals:
	void attempt_automation_add_signal(int p_property_idx);	
	void attempt_automation_remove_signal(int p_property_idx);	
			
	void editor_requested();
public:
	bool check_action(QAction *p_action);	
	
	void rebuild();
	AutomationPopup(QWidget *p_parent,Track *p_track);
	~AutomationPopup();

};


class AutomationSettingsLFO : public QDialog {

	Q_OBJECT
	QComboBox *wave;
	QSlider *phase;
	QSlider *rate;
	QSlider *depth;
	QSlider *random;
	QSlider *random_seed;
	bool lfo_changed;
	Automation::AutomationData *blockdata;
	
	void set_lfo();
	
private slots:
	
	
	void wave_changed(int);
	void slider_changed(int);
signals:	
	void lfo_changed_signal();
public:	
	bool is_lfo_changed();
	AutomationSettingsLFO(QWidget *p_parent,Automation::AutomationData *p_blockdata);
	
};

}

#endif
