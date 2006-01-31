//
// C++ Implementation: global_properties
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_properties.h"

namespace ReShaked {


LocalProperty &GlobalProperties::get_swing() {
	
	return swing;
}
LocalProperty &GlobalProperties::get_volume() {
	
	return volume;
}
LocalProperty &GlobalProperties::get_tempo() {
	
	return tempo;	
}

void GlobalProperties::clear() {
	
	swing.set(0);
	tempo.set(125);
	volume.set(0);
}
GlobalProperties::GlobalProperties() {
	
	swing.set_all( 0, 0, 100, 0, 1, Property::DISPLAY_KNOB, "global_swing","Global Swing","%","Disabled");
	volume.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "global_volume","Global Volume","dB");
	tempo.set_all( 125.0, 32, 300, 125, 1, Property::DISPLAY_KNOB,"tempo","Tempo (BPM)");
	
}


GlobalProperties::~GlobalProperties()
{
}


}
