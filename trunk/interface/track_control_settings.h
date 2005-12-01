//
// C++ Interface: track_control_settings
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_CONTROL_SETTINGS_H
#define RESHAKEDTRACK_CONTROL_SETTINGS_H

#include "ui_blocks/helpers.h"
#include <Qt/qgroupbox.h>
#include "ui_blocks/property_editors.h"
#include "engine/editor.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class TrackControlSettings : public QWidget {
	
			    
			    
	PropertyEditKnob *edit_swing;
	PropertyEditKnob *edit_balance;
	Track *track;
	
public:
	
	void set_track(Track *p_track);
	TrackControlSettings(QWidget *p_parent);
	~TrackControlSettings();

};

}

#endif
