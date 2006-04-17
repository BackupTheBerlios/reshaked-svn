//
// C++ Implementation: track_control_settings
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_control_settings.h"
#include <Qt/qlayout.h>

namespace ReShaked {


void TrackControlSettings::set_track(Track *p_track) {
	
	track=p_track;
	//edit_swing->set_property( & p_track->swing() );
	//edit_balance->set_property( & p_track->balance() );

}

	
TrackControlSettings::TrackControlSettings(QWidget *p_parent) :QWidget(p_parent)
{
	QGridLayout *gl = new QGridLayout(this);
	setLayout(gl);
	/*
	edit_swing= new PropertyEditKnob(this);
	gl->addWidget(edit_swing,0,0);
	edit_balance =  new PropertyEditKnob(this);
	gl->addWidget(edit_balance,1,0);
	
	layout()->setMargin(0);
	layout()->setSpacing(2);
	*/
	
}


TrackControlSettings::~TrackControlSettings()
{
}


}
