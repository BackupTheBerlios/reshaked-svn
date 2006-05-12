//
// C++ Implementation: vst_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "vst_plugin.h"

#include "version.h"
#ifdef VST_ENABLED

#include <math.h>

namespace ReShaked {



double VST_Plugin::Parameter::get() {
	
	return ptrPlug->getParameter(ptrPlug,index);
}
void VST_Plugin::Parameter::set(double p_val){
	
	ptrPlug->setParameter(ptrPlug,index,p_val);
}
double VST_Plugin::Parameter::get_stepping(){
	
	return 0.0001;
}
double VST_Plugin::Parameter::get_min(){
	
	return 0;
}
double VST_Plugin::Parameter::get_max(){
	
	return 1;
}
double VST_Plugin::Parameter::get_default(){
	
	return 0;
}
String VST_Plugin::Parameter::get_name(){
	

	resbuff[0]=0;
	ptrPlug->dispatcher (ptrPlug, effGetParamName, index, 0, resbuff, 0);	
	return resbuff;
	
}
String VST_Plugin::Parameter::get_caption(){
	
	resbuff[0]=0;
	ptrPlug->dispatcher (ptrPlug, effGetParamName, index, 0, resbuff, 0);	
	return resbuff;

}
String VST_Plugin::Parameter::get_postfix(){
	
	resbuff[0]=0;
	ptrPlug->dispatcher (ptrPlug, effGetParamLabel, index, 0, resbuff, 0);	
	return resbuff;
}
String VST_Plugin::Parameter::get_text_value(double p_for_value,bool p_no_postfix){
	
	return "";
}
bool VST_Plugin::Parameter::has_text_value(){
	
	return true;	
}

Property::DisplayMode VST_Plugin::Parameter::get_display_mode(){
	
	return DISPLAY_KNOB;
}

/* helpers */	
String VST_Plugin::Parameter::get_text_value(bool p_no_postfix){

	resbuff[0]=0;
	ptrPlug->dispatcher (ptrPlug, effGetParamDisplay, index, 0, resbuff, 0);	
	String res=resbuff;
	if (!p_no_postfix)
		res+=get_postfix();
	return res;
	
}

		


int VST_Plugin::get_input_plug_count() {
	
	return input_plugs.size();
}
int VST_Plugin::get_output_plug_count() {
	
	return output_plugs.size();
	
	
}
AudioPlug *VST_Plugin::get_input_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,input_plugs.size(),NULL);
	
	return input_plugs[p_index];	
}

AudioPlug *VST_Plugin::get_output_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,output_plugs.size(),NULL);
	
	return output_plugs[p_index];	
}

int VST_Plugin::get_port_count() {
	
	return property_list.size();
}
Property& VST_Plugin::get_port(int p_index) {
	
	static LocalProperty error;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error);

	return *property_list[p_index];
	
}
SoundPlugin::PortType VST_Plugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void VST_Plugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;

	ptrPlug->dispatcher (ptrPlug, effSetSampleRate, 0, 0, 0, p_mixing_rate);

		
}

/* Processing */
void VST_Plugin::process(int p_frames) {
	
	if (!enabled)
		return;
	/* Configure Input Buffers */

	if (get_info()->is_synth) {
		
		const EventBuffer *ebuff=get_event_buffer();
		int event_idx=0;
		
		/* First try the midi events that come from our MidiParams */
		const MidiParameters::Changes::Change *changes=midi_parameters->get_changes();
		for (int i=0;i<midi_parameters->get_changes_count();i++) {
			
			if (event_idx==MAX_INPUT_EVENTS)
				break;
			VstMidiEvent &vstem=event_array[event_idx];
			
			if (changes[i].control<128) {
				
				vstem.deltaFrames=0;
				vstem.midiData[0]=0xB0;	//channel 0?
				vstem.midiData[1]=changes[i].control;
				vstem.midiData[2]=changes[i].value;
				vstem.midiData[3]=0;
				event_idx++;
								
			} else 	if (changes[i].control==MidiParameters::CONTROL_AFTERTOUCH) {

				vstem.deltaFrames=0;
				vstem.midiData[0]=0xD0;	//channel 0?
				vstem.midiData[1]=changes[i].value;
				vstem.midiData[2]=0;
				vstem.midiData[3]=0;
				event_idx++;
				
			} else 	if (changes[i].control==MidiParameters::CONTROL_PITCH_BEND) {
				
				vstem.deltaFrames=0;
				vstem.midiData[0]=0xE0;	//channel 0?
				vstem.midiData[1]=changes[i].value&0x7F;
				vstem.midiData[2]=changes[i].value>>7;
				vstem.midiData[3]=0;
				event_idx++;
			}
		}
		
		midi_parameters->clear_changes();
		/* Then do the input midi events */
		for (int i=0;i<ebuff->get_event_count();i++) {
		
			const Event *e=ebuff->get_event(i);
				
			
			if (e->type==Event::TYPE_MIDI) {
			
				if (event_idx==MAX_INPUT_EVENTS)
					break;
				
				const EventMidi &em=e->param.midi;
				VstMidiEvent &vstem=event_array[event_idx];
				switch (em.midi_type) {
					case EventMidi::MIDI_NOTE_ON: {
					
						vstem.deltaFrames=e->frame_offset;
						vstem.midiData[0]=0x90;	//channel 0?
						vstem.midiData[1]=em.data.note.note;
						vstem.midiData[2]=em.data.note.velocity;
						vstem.midiData[3]=0;
						event_idx++;
					} break;
					case EventMidi::MIDI_NOTE_OFF: {
					
						vstem.deltaFrames=e->frame_offset;
						vstem.midiData[0]=0x80;	//channel 0?
						vstem.midiData[1]=em.data.note.note;
						vstem.midiData[2]=em.data.note.velocity;
						vstem.midiData[3]=0;
						event_idx++;
					} break;
					case EventMidi::MIDI_CONTROLLER: {
					
						vstem.deltaFrames=e->frame_offset;
						vstem.midiData[0]=0xB0;	//channel 0?
						vstem.midiData[1]=em.data.control.index;
						vstem.midiData[2]=em.data.control.parameter;
						vstem.midiData[3]=0;
						event_idx++;
						
					} break;
					case EventMidi::MIDI_PITCH_BEND: {
					
						vstem.deltaFrames=e->frame_offset;
						vstem.midiData[0]=0xE0;	//channel 0?
						vstem.midiData[1]=em.data.pitch_bend.bend&0x7F;
						vstem.midiData[2]=em.data.pitch_bend.bend>>7;
						vstem.midiData[3]=0;
						event_idx++;
					} break;
					default: {} //dont annoy, gcc
				}
			}
		}
		
		event_pointers->numEvents=event_idx; //set how many events
		
		//eat all of it, fucker, I dont care if you dont want any more
		ptrPlug->dispatcher(ptrPlug,effProcessEvents,0,0,event_pointers,0.0f);
		
	}
	
	int abuff_idx=0;
	for (int i=0;i<input_plugs.size();i++) {
		
		for (int j=0;j<input_plugs[i]->get_channels();j++) {
			
			input_buffers[abuff_idx]=input_plugs[i]->get_buffer()->get_buffer(j);
			abuff_idx++;
		}
	}
		
	/* Configure Output Buffers */
	abuff_idx=0;
	for (int i=0;i<output_plugs.size();i++) {
		
		output_plugs[i]->get_buffer()->clear(p_frames);
		for (int j=0;j<output_plugs[i]->get_channels();j++) {
			
			output_buffers[abuff_idx]=output_plugs[i]->get_buffer()->get_buffer(j);
			abuff_idx++;
		}
	}
		
	ptrPlug->processReplacing(ptrPlug,input_buffers,output_buffers,p_frames);

	
}

VstIntPtr VSTCALLBACK VST_Plugin::host(AEffect *effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt) {
	
	long retval=0;

	printf("call to host, effect is %p\n",effect);
	VST_Plugin *_this=NULL;
	if (effect && effect->resvd1!=0) {
		printf("try to convert from resvd1\n");
		_this=FromVstPtr<VST_Plugin>(effect->resvd1);
		printf("convert ok\n");
	}
	printf("from thread %i\n",GetCurrentThreadId());;
	switch (opcode)
	{
		case audioMasterVersion:
			retval=2;
			break;

		case audioMasterAutomate:
			//Input values:
			//<index> parameter that has changed
			//<opt> new value

			//Return value:
			//not tested, always return 0

			//NB - this is called when the plug calls
			//setParameterAutomated
			if (_this && _this->property_changed) {
				if (index<0 || index>=_this->param_list.size())
					return 0;
				for (int i=0;i<_this->property_list.size();i++) {
					
					if (_this->property_list[i]==_this->param_list[index]) {
						_this->property_changed(_this->userdata,i);
						
					}
				}
			}
					
			break;

		case audioMasterCurrentId:
			//Input values:
			//none

			//Return Value
			//the unique id of a plug that's currently loading
			//zero is a default value and can be safely returned if not known

			break;

		case audioMasterIdle:
			//Input values:
			//none

			//Return Value
			//not tested, always return 0
			Sleep(1);
			//NB - idle routine should also call effEditIdle for all open editors
			break;

		case audioMasterPinConnected:
			//Input values:
			//<index> pin to be checked
			//<value> 0=input pin, non-zero value=output pin

			//Return values:
			//0=true, non-zero=false
//			cout << "plug called audioMasterPinConnected" << endl;
			break;

		//VST 2.0 opcodes

		case audioMasterGetTime:
			//Input Values:
			//<value> should contain a mask indicating which fields are required
			//...from the following list?
			//kVstNanosValid
			//kVstPpqPosValid
			//kVstTempoValid
			//kVstBarsValid
			//kVstCyclePosValid
			//kVstTimeSigValid
			//kVstSmpteValid
			//kVstClockValid

			//Return Value:
			//ptr to populated const VstTimeInfo structure (or 0 if not supported)

			//NB - this structure will have to be held in memory for long enough
			//for the plug to safely make use of it
			//cout << "plug called audioMasterGetTime" << endl;
			break;

		case audioMasterProcessEvents:
			//Input Values:
			//<ptr> Pointer to a populated VstEvents structure

			//Return value:
			//0 if error
			//1 if OK
//			cout << "plug called audioMasterProcessEvents" << endl;
			break;

	
	
		case audioMasterIOChanged:
			//Input Values:
			//None

			//Return Value:
			//0 if error
			//non-zero value if OK
//			cout << "plug called audioMasterIOChanged" << endl;
			break;

	
		case audioMasterSizeWindow:
			//Input Values:
			//<index> width
			//<value> height

			//Return Value:
			//0 if error
			//non-zero value if OK
//			cout << "plug called audioMasterSizeWindow" << endl;
			break;

		case audioMasterGetSampleRate:
			//Input Values:
			//None

			//Return Value:
			//not tested, always return 0

			//NB - Host must despatch effSetSampleRate to the plug in response
			//to this call
			//Check despatcher notes for any return codes from effSetSampleRate
			//cout << "plug called audioMasterGetSampleRate" << endl;
			
			if (_this)
				effect->dispatcher (effect, effSetSampleRate, 0, 0, 0, _this->mix_rate);
			break;

		case audioMasterGetBlockSize:
			//Input Values:
			//None

			//Return Value:
			//not tested, always return 0

			//NB - Host must despatch effSetBlockSize to the plug in response
			//to this call
			//Check despatcher notes for any return codes from effSetBlockSize
			//cout << "plug called audioMasterGetBlockSize" << endl;
			effect->dispatcher(effect,effSetBlockSize,0,128,NULL,0.0f);

			break;

		case audioMasterGetInputLatency:
			//Input Values:
			//None

			//Return Value:
			//input latency (in sampleframes?)
			//cout << "plug called audioMasterGetInputLatency" << endl;
			break;

		case audioMasterGetOutputLatency:
			//Input Values:
			//None

			//Return Value:
			//output latency (in sampleframes?)
			//cout << "plug called audioMasterGetOutputLatency" << endl;
			break;
		case audioMasterGetCurrentProcessLevel:
			//Input Values:
			//None

			//Return Value:
			//0: not supported,
			//1: currently in user thread (gui)
			//2: currently in audio thread (where process is called)
			//3: currently in 'sequencer' thread (midi, timer etc)
			//4: currently offline processing and thus in user thread
			//other: not defined, but probably pre-empting user thread.
			retval=2;
			//cout << "plug called audioMasterGetCurrentProcessLevel" << endl;
			break;

		case audioMasterGetAutomationState:
			//Input Values:
			//None

			//Return Value:
			//0: not supported
			//1: off
			//2:read
			//3:write
			//4:read/write
			//cout << "plug called audioMasterGetAutomationState" << endl;
			break;

		case audioMasterGetVendorString:
			//Input Values:
			//<ptr> string (max 64 chars) to be populated

			//Return Value:
			//0 if error
			//non-zero value if OK
			//cout << "plug called audioMasterGetVendorString" << endl;
			break;

		case audioMasterGetProductString:
			//Input Values:
			//<ptr> string (max 64 chars) to be populated

			//Return Value:
			//0 if error
			//non-zero value if OK
			//cout << "plug called audioMasterGetProductString" << endl;
			break;

		case audioMasterGetVendorVersion:
			//Input Values:
			//None

			//Return Value:
			//Vendor specific host version as integer
			//cout << "plug called audioMasterGetVendorVersion" << endl;
			break;

		case audioMasterVendorSpecific:
			//Input Values:
			//<index> lArg1
			//<value> lArg2
			//<ptr> ptrArg
			//<opt>	floatArg

			//Return Values:
			//Vendor specific response as integer
			//cout << "plug called audioMasterVendorSpecific" << endl;
			break;

	
		case audioMasterCanDo:
			//Input Values:
			//<ptr> predefined "canDo" string

			//Return Value:
			//0 = Not Supported
			//non-zero value if host supports that feature

			//NB - Possible Can Do strings are:
			//"sendVstEvents",
			//"sendVstMidiEvent",
			//"sendVstTimeInfo",
			//"receiveVstEvents",
			//"receiveVstMidiEvent",
			//"receiveVstTimeInfo",
			//"reportConnectionChanges",
			//"acceptIOChanges",
			//"sizeWindow",
			//"asyncProcessing",
			//"offline",
			//"supplyIdle",
			//"supportShell"
			//cout << "plug called audioMasterCanDo" << endl;

			if (strcmp((char*)ptr,"sendVstEvents")==0 ||
						 strcmp((char*)ptr,"sendVstMidiEvent")==0 ||
						 strcmp((char*)ptr,"supplyIdle")==0)
			{
				retval=1;
			}
			else
			{
				retval=0;
			}

			break;

		case audioMasterGetLanguage:
			//Input Values:
			//None

			//Return Value:
			//kVstLangEnglish
			//kVstLangGerman
			//kVstLangFrench
			//kVstLangItalian
			//kVstLangSpanish
			//kVstLangJapanese
			//cout << "plug called audioMasterGetLanguage" << endl;
			retval=1;
			break;
/*
			MAC SPECIFIC?

		case audioMasterOpenWindow:
			//Input Values:
			//<ptr> pointer to a VstWindow structure

			//Return Value:
			//0 if error
			//else platform specific ptr
			cout << "plug called audioMasterOpenWindow" << endl;
			break;

		case audioMasterCloseWindow:
			//Input Values:
			//<ptr> pointer to a VstWindow structure

			//Return Value:
			//0 if error
			//Non-zero value if OK
			cout << "plug called audioMasterCloseWindow" << endl;
			break;
*/
		case audioMasterGetDirectory:
			//Input Values:
			//None

			//Return Value:
			//0 if error
			//FSSpec on MAC, else char* as integer

			//NB Refers to which directory, exactly?
			//cout << "plug called audioMasterGetDirectory" << endl;
			break;

		case audioMasterUpdateDisplay:
			//Input Values:
			//None

			//Return Value:
			//Unknown
			//cout << "plug called audioMasterUpdateDisplay" << endl;
			break;
	}
	
	return retval;
}

void VST_Plugin::set_property_changed_callback(void *p_userdata,void (*p_property_changed)(void*,int)) {
	
	property_changed=p_property_changed;
	userdata=p_userdata;
}

VST_Plugin::VST_Plugin(const SoundPluginInfo *p_info,String p_path,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	userdata=NULL;
	property_changed=NULL;
	
	mix_rate=44100;
	enabled=true;
	if (p_channels<1)
		p_channels=1;
	
	printf("Begin plugin instancing\n");
	//input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	//output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	libhandle=LoadLibrary(p_path.utf8().get_data());
	if (libhandle==NULL) {
		WARN_PRINT("Can't open library");
		enabled=false;
		return;
	}
	printf("Library open OK\n");
		
	AEffect* (__cdecl* getNewPlugInstance)(audioMasterCallback);
	getNewPlugInstance=(AEffect*(__cdecl*)(audioMasterCallback))GetProcAddress(libhandle, "main");

	if (getNewPlugInstance==NULL) {
		WARN_PRINT("Can't find symbol 'main'");
		FreeLibrary(libhandle);
		enabled=false;
		return;
	}
	printf("Get instancing func OK\n");
	
	ptrPlug=getNewPlugInstance(&VST_Plugin::host);

	if (ptrPlug==NULL) {
		WARN_PRINT("Can't instance plugin.");
		enabled=false;
		FreeLibrary(libhandle);
		return;
	}
	printf("Call instancing func OK\n");
	
	if (ptrPlug->magic!=kEffectMagic) {
			
		WARN_PRINT("Can't instance plugin, corrupted");
		FreeLibrary(libhandle);
		enabled=false;
		return;
			
	}	
	
	printf("plugin instanced OK\n");
	
	//Open Plugin
	ptrPlug->dispatcher(ptrPlug,effOpen,0,0,NULL,0.0f);
	
	ptrPlug->resvd1=ToVstPtr<VST_Plugin>(this);
	
	int in_port_count=ptrPlug->numInputs/p_channels;
	int out_port_count=ptrPlug->numOutputs/p_channels;
	printf("inputs: %i, outputs:%i\n",in_port_count,out_port_count);
	if (in_port_count)
		input_buffers = new float*[p_channels*in_port_count];
	else
		input_buffers=NULL;
	
	if (out_port_count)
		output_buffers = new float*[p_channels*out_port_count];
	else
		output_buffers=NULL;
	

	for (int i=0;i<in_port_count;i++) {
		
		input_plugs.push_back( new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this) );
	}
	
	for (int i=0;i<out_port_count;i++) {
		
		output_plugs.push_back( new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this) );
	}
	
	mix_rate=44100;
	//Suspend Plugin
	ptrPlug->dispatcher(ptrPlug,effMainsChanged,0,0,NULL,0.0f);	
	//set some crap default blocksize(128)
	ptrPlug->dispatcher(ptrPlug,effSetBlockSize,0,128,NULL,0.0f);
	
	//create parameters
	for (int i=0;i<ptrPlug->numParams;i++) {
		
		Parameter *p=new Parameter(ptrPlug,i);
		property_list.push_back( p );
		param_list.push_back(p);
	}
			
	
	
	/* HANDLE EVENTS ARRAY */
	if (get_info()->is_synth) {
	
		event_array = new VstMidiEvent[MAX_INPUT_EVENTS];
		int array_size=sizeof(VstInt32)+sizeof(VstIntPtr)+sizeof(VstEvent*)*MAX_INPUT_EVENTS;
		event_pointers = (VstEvents*) new char[array_size];
		event_pointers->numEvents=0;
		event_pointers->reserved=0;
		
		for (int i=0;i<MAX_INPUT_EVENTS;i++) {
			
			event_array[i].type=kVstMidiType;
			event_array[i].byteSize=24;
			event_array[i].deltaFrames=0;
			event_array[i].flags=0;			///< @see VstMidiEventFlags
			event_array[i].noteLength=0;	///< (in sample frames) of entire note, if available, 
			event_array[i].noteOffset=0;	///< offset (in sample frames) into note from note 
			event_array[i].midiData[0]=0;
			event_array[i].midiData[1]=0;
			event_array[i].midiData[2]=0;
			event_array[i].midiData[3]=0;
			event_array[i].detune=0;
			event_array[i].noteOffVelocity=0;
			event_array[i].reserved1=0;
			event_pointers->events[i]=(VstEvent*)&event_array[i];
		}
		
		midi_parameters = new MidiParameters;
		for (int i=0;i<midi_parameters->get_parameter_count();i++)
			property_list.insert(property_list.begin(), midi_parameters->get_parameter(i) );
	} else {
		
		event_array=NULL;
		event_pointers=NULL;
		midi_parameters=NULL;
	}
	
	gain.set_all( get_info()->is_synth?-20:0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "vst_plugin_gain","Gain","dB");
	property_list.push_back(&gain);
	//switch the plugin back on (calls Resume)
	ptrPlug->dispatcher(ptrPlug,effMainsChanged,0,1,NULL,0.0f);

	
}


VST_Plugin::~VST_Plugin() { 
	
	if (!enabled)
		return;
	
	//Suspend Plugin
	ptrPlug->dispatcher(ptrPlug,effMainsChanged,0,0,NULL,0.0f);	
	//Free Plugin
	ptrPlug->dispatcher(ptrPlug,effClose,0,0,NULL,0.0f);
	//Close Library
	FreeLibrary(libhandle);
	
	/* free plugs */
	for (int i=0;i<input_plugs.size();i++) 
		delete input_plugs[i];
	
	for (int i=0;i<output_plugs.size();i++) 
		delete output_plugs[i];
	
	/* free property parameters */
	for (int i=0;i<param_list.size();i++) 
		delete param_list[i];
	
	/* free Buffers */
	if (input_buffers)
		delete[] input_buffers;
	
	if (output_buffers)
		delete[] output_buffers;
	
	if (get_info()->is_synth) {
		
		delete[] event_array;
		delete[] (char*)event_pointers; //since i had to custom-instance for custom-eventcount
		delete midi_parameters;
	}
}


}

#endif
