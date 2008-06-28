

#include "midi_parameters.h"

namespace ReShaked {



double MidiParameters::Parameter::get() {
	
	
	return val;	
}

void MidiParameters::Parameter::set(double p_val) {
	int new_val=lrint(p_val);
	if (val==new_val && stream)
		return;
	if (new_val<min || new_val>max)
		return;
	
	val=new_val;
	if (val!=CHANGES_DISABLED) {
		
		if (changes->count>=MAX_CHANGED_LIST)
			return; //nothing to do
		changes->list[ changes->count ].control=index;
		changes->list[ changes->count ].value=val;
		changes->count++;
	}
		
}
double MidiParameters::Parameter::get_stepping() {
	
	return 1;
}
double MidiParameters::Parameter::get_min() {
	
	return min;
}
double MidiParameters::Parameter::get_max() {
	
	return max;
}
double MidiParameters::Parameter::get_default() {
	
	
	return default_val;
}
String MidiParameters::Parameter::get_name() {
	
	return "midi_control_"+String::num(index);
}
String MidiParameters::Parameter::get_caption() {
	
	return display;
	
}
String MidiParameters::Parameter::get_suffix() {
	
	return "";
	
}
String MidiParameters::Parameter::get_text_value(double p_for_value,bool p_no_suffix) {
	
	int val=lrint(p_for_value);
			
	if (val==CHANGES_DISABLED) 
		return "Off";
	else
		return String::num(val);
	
}
bool MidiParameters::Parameter::has_text_value() {
	
	return true;
	
}

Property::DisplayMode MidiParameters::Parameter::get_display_mode() {
	
	return DISPLAY_KNOB;
	
}

/* helpers */	
String MidiParameters::Parameter::get_text_value(bool p_no_suffix) {
	
	return get_text_value((double)val);
	
}

/**************************/


int MidiParameters::get_parameter_count() {
	
	return MAX_CONTROLS;	
}
bool MidiParameters::is_parameter_active(int p_param){
	
	ERR_FAIL_INDEX_V(p_param,MAX_CONTROLS,false);
	return parameters[p_param].active;
}
Property *MidiParameters::get_parameter(int p_param){
	
	ERR_FAIL_INDEX_V(p_param,MAX_CONTROLS,NULL);	
	return &parameters[p_param];
}
void MidiParameters::set_parameter_display(int p_param,String p_display){
	ERR_FAIL_INDEX(p_param,MAX_CONTROLS);	
	
	parameters[p_param].display=p_display;
}

/* track parameter changes */
int MidiParameters::get_changes_count(){
	
	return changes.count;
}
const MidiParameters::Changes::Change* MidiParameters::get_changes(){
	
	return changes.list;
}
void MidiParameters::clear_changes(){
	
	changes.count=0;
}

void MidiParameters::send_sound_off() {
	
	if (changes.count>=MAX_CHANGED_LIST)
		return;
	
	changes.list[ changes.count ].control=120;
	changes.list[ changes.count ].value=127;
	changes.count++;
	if (changes.count>=MAX_CHANGED_LIST)
		return;
	
	changes.list[ changes.count ].control=123;
	changes.list[ changes.count ].value=127;
	changes.count++;
	if (changes.count>=MAX_CHANGED_LIST)
		return;
	
}

MidiParameters::MidiParameters(){
	
	for (int i=0;i<MAX_MIDI_CONTROLS;i++) {
		
		if (i<120) {
			parameters[i].display="MIDI Controls/CC " + String::num(i);
			parameters[i].stream=true;
		} else {
			parameters[i].display="MIDI Commands/CMD " + String::num(i);
			parameters[i].stream=false;
		}
		
		parameters[i].val=CHANGES_DISABLED;
		parameters[i].min=CHANGES_DISABLED;
		parameters[i].max=127;
		parameters[i].default_val=CHANGES_DISABLED;
		parameters[i].index=i;
		parameters[i].active=true;
		parameters[i].changes=&changes;
	}
	
	parameters[CONTROL_AFTERTOUCH].display="MIDI Input/Aftertouch";
	parameters[CONTROL_AFTERTOUCH].stream=true;
	parameters[CONTROL_AFTERTOUCH].val=CHANGES_DISABLED;
	parameters[CONTROL_AFTERTOUCH].min=CHANGES_DISABLED;
	parameters[CONTROL_AFTERTOUCH].max=127;
	parameters[CONTROL_AFTERTOUCH].default_val=CHANGES_DISABLED;
	parameters[CONTROL_AFTERTOUCH].index=CONTROL_AFTERTOUCH;
	parameters[CONTROL_AFTERTOUCH].active=true;
	parameters[CONTROL_AFTERTOUCH].changes=&changes;
	
	parameters[CONTROL_PITCH_BEND].display="MIDI Input/Pitch Bend";
	parameters[CONTROL_PITCH_BEND].stream=true;
	parameters[CONTROL_PITCH_BEND].val=0x2000;
	parameters[CONTROL_PITCH_BEND].min=0;
	parameters[CONTROL_PITCH_BEND].max=0x3FFF;
	parameters[CONTROL_PITCH_BEND].default_val=0x2000;
	parameters[CONTROL_PITCH_BEND].index=CONTROL_PITCH_BEND;
	parameters[CONTROL_PITCH_BEND].active=true;
	parameters[CONTROL_PITCH_BEND].changes=&changes;
	
	/* NOW Some defaults */
	
	parameters[1].display="MIDI Controls/01 Modulation (coarse)";
	parameters[1].val=parameters[1].default_val=0;
	
	parameters[2].display="MIDI Controls/02 Breath (coarse)";
	parameters[2].val=parameters[2].default_val=0;
	
	parameters[4].display="MIDI Controls/04 Foot Pedal (coarse)";
	parameters[4].val=parameters[4].default_val=0;
	
	parameters[5].display="MIDI Controls/05 Portamento Time (coarse)";
	parameters[5].val=parameters[5].default_val=0;
	
	parameters[7].display="MIDI Controls/07 Main Volume";
	parameters[7].val=parameters[7].default_val=100;
	
	parameters[8].display="MIDI Controls/08 Balance";
	
	parameters[10].display="MIDI Controls/10 Pan Pos";
	parameters[10].val=parameters[10].default_val=64;
	
	parameters[11].display="MIDI Controls/11 Expression";
	parameters[11].val=parameters[11].default_val=100;
	
	parameters[33].display="MIDI Controls/33 Modulation (fine)";
	parameters[33].val=parameters[33].default_val=0;
	
	parameters[34].display="MIDI Controls/34 Breath (fine)";
	parameters[34].val=parameters[34].default_val=0;
	
	parameters[35].display="MIDI Controls/Breath (fine)";
	parameters[35].val=parameters[35].default_val=0;
	
	parameters[36].display="MIDI Controls/Foot Pedal (fine)";
	parameters[36].val=parameters[36].default_val=0;
			
	parameters[37].display="MIDI Controls/Portamento Time (fine)";
	parameters[37].val=parameters[37].default_val=0;
	
	parameters[65].display="MIDI Controls/67 Portamento (on/off)";
	parameters[65].val=parameters[65].default_val=0;
			
	parameters[67].display="MIDI Controls/67 Soft Pedal";
	parameters[67].val=parameters[67].default_val=0;
	
	parameters[68].display="MIDI Controls/68 Legato Pedal";
	parameters[68].val=parameters[68].default_val=0;
	
	parameters[69].display="MIDI Controls/69 Hold 2 Pedal";
	parameters[69].val=parameters[69].default_val=0;
	
	changes.count=0;
}




};
