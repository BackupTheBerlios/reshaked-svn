//
// C++ Implementation: midi_driver
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_driver.h"

namespace ReShaked {

MidiDeviceDescription *MidiDriver::get_output_device_description(int p_output) {
	
	ERR_FAIL_INDEX_V(p_output,MAX_OUTPUTS,NULL);
	return &output_device_description[p_output];	

}

MidiDriver::MidiDriver() {
}


MidiDriver::~MidiDriver()
{
}


}
