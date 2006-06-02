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
String VST_Plugin::Parameter::get_text_value(bool p_no_postfix) l{

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
		
		if (off_all_notes) {
			/* The only reliable way to silence all notes in a VST */
			
			for (int i=0;i<128;i++) {
			
				if (event_idx==MAX_INPUT_EVENTS)
					break;
				VstMidiEvent &vstem=event_array[event_idx];
				
				vstem.deltaFrames=0;
				vstem.midiData[0]=0x80;	//channel 0?
				vstem.midiData[1]=i;
				vstem.midiData[2]=0;
				vstem.midiData[3]=0;
				event_idx++;
								
				
			}
			
			off_all_notes=false;
		}
		
		
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

	if (gain.get()!=0.0) {
		//apply gain
		float linear_gain=powf(10.0,gain.get()/20.0); //convert db -> linear
		
		for (int i=0;i<output_plugs.size();i++) 
			output_plugs[i]->get_buffer()->multiply_by(linear_gain,p_frames);
	}
	
}

VstIntPtr VSTCALLBACK VST_Plugin::host(AEffect *effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt) {
	
	long retval=0;


	VST_Plugin *_this=NULL;
	if (effect && effect->resvd1!=0) {
		//try to convert from resvd1
		_this=FromVstPtr<VST_Plugin>(effect->resvd1);

	}
	//printf("from thread %i\n",GetCurrentThreadId());;
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
//			std::cout << "plug called audioMasterPinConnected" << std::endl;
			
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
			//std::cout << "plug called audioMasterGetTime" << std::endl;
			break;

		case audioMasterProcessEvents:
			//Input Values:
			//<ptr> Pointer to a populated VstEvents structure

			//Return value:
			//0 if error
			//1 if OK
//			std::cout << "plug called audioMasterProcessEvents" << std::endl;
			break;

	
	
		case audioMasterIOChanged:
			//Input Values:
			//None

			//Return Value:
			//0 if error
			//non-zero value if OK
//			std::cout << "plug called audioMasterIOChanged" << std::endl;
			break;

	
		case audioMasterSizeWindow:
			//Input Values:
			//<index> width
			//<value> height

			//Return Value:
			//0 if error
			//non-zero value if OK
//			std::cout << "plug called audioMasterSizeWindow" << std::endl;
			break;

		case audioMasterGetSampleRate:
			//Input Values:
			//None

			//Return Value:
			//not tested, always return 0

			//NB - Host must despatch effSetSampleRate to the plug in response
			//to this call
			//Check despatcher notes for any return codes from effSetSampleRate
			//std::cout << "plug called audioMasterGetSampleRate" << std::endl;
			
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
			//std::cout << "plug called audioMasterGetBlockSize" << std::endl;
			effect->dispatcher(effect,effSetBlockSize,0,128,NULL,0.0f);

			break;

		case audioMasterGetInputLatency:
			//Input Values:
			//None

			//Return Value:
			//input latency (in sampleframes?)
			//std::cout << "plug called audioMasterGetInputLatency" << std::endl;
			break;

		case audioMasterGetOutputLatency:
			//Input Values:
			//None

			//Return Value:
			//output latency (in sampleframes?)
			//std::cout << "plug called audioMasterGetOutputLatency" << std::endl;
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
			//std::cout << "plug called audioMasterGetCurrentProcessLevel" << std::endl;
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
			//std::cout << "plug called audioMasterGetAutomationState" << std::endl;
			break;

		case audioMasterGetVendorString: {
			
			static const char* vendor="Juan Linietsky";
			char *dst=(char*)ptr;
			const char *src=vendor;
			
			while ( *src!=0 ) {
				
				*dst=*src;
				src++;
				dst++;
			};
			
			*dst=0;
				
			//Input Values:
			//<ptr> string (max 64 chars) to be populated

			//Return Value:
			//0 if error
			//non-zero value if OK
			//std::cout << "plug called audioMasterGetVendorString" << std::endl;
		} break;

		case audioMasterGetProductString: {
			
			static const char* product="ReShaked";
			char *dst=(char*)ptr;
			const char *src=product;
			
			while ( *src!=0 ) {
				
				*dst=*src;
				src++;
				dst++;
			};
			
			*dst=0;
			
			//Input Values:
			//<ptr> string (max 64 chars) to be populated

			//Return Value:
			//0 if error
			//non-zero value if OK
			//std::cout << "plug called audioMasterGetProductString" << std::endl;
		}	break;

		 case audioMasterGetVendorVersion:
			//Input Values:
			//None

			//Return Value:
			//Vendor specific host version as integer
			//std::cout << "plug called audioMasterGetVendorVersion" << std::endl;
			break;

		case audioMasterVendorSpecific:
			//Input Values:
			//<index> lArg1
			//<value> lArg2
			//<ptr> ptrArg
			//<opt>	floatArg

			//Return Values:
			//Vendor specific response as integer
			//std::cout << "plug called audioMasterVendorSpecific" << std::endl;
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
			//std::cout << "plug called audioMasterCanDo: "<< (char*)ptr << std::endl;

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
			//std::cout << "plug called audioMasterGetLanguage" << std::endl;
			retval=kVstLangEnglish;
			break;
/*
			MAC SPECIFIC?

		case audioMasterOpenWindow:
			//Input Values:
			//<ptr> pointer to a VstWindow structure

			//Return Value:
			//0 if error
			//else platform specific ptr
			std::cout << "plug called audioMasterOpenWindow" << std::endl;
			break;

		case audioMasterCloseWindow:
			//Input Values:
			//<ptr> pointer to a VstWindow structure

			//Return Value:
			//0 if error
			//Non-zero value if OK
			std::cout << "plug called audioMasterCloseWindow" << std::endl;
			break;
*/
		case audioMasterGetDirectory: {
			
			//std::cout << "plug called audioMasterGetDirectory" << std::endl;
			ERR_FAIL_COND_V(_this==NULL,0);
			char *dir = new char[_this->plugin_dir.length()+1];
			for (int i=0;i<_this->plugin_dir.size();i++) {
				
				dir[i]=(char)_this->plugin_dir[i]; //not perfect but..
			}
			
			dir[_this->plugin_dir.length()]=0;
			return ToVstPtr<char>(dir); // I guess the host will handle it?
			
			//Input Values:
			//None

			//Return Value:
			//0 if error
			//FSSpec on MAC, else char* as integer

			//NB Refers to which directory, exactly?
			
		} break;

		case audioMasterUpdateDisplay:
			//Input Values:
			//None

			//Return Value:
			//Unknown
			//std::cout << "plug called audioMasterUpdateDisplay" << std::endl;
			break;
	}
	
	return retval;
}

void VST_Plugin::set_property_changed_callback(void *p_userdata,void (*p_property_changed)(void*,int)) {
	
	property_changed=p_property_changed;
	userdata=p_userdata;
}

void VST_Plugin::reset() {
	
	/* People at #musicdsp says this is the semi-standard way to clear up the stuff */
	
	
	//Suspend Plugin
	ptrPlug->dispatcher(ptrPlug,effMainsChanged,0,0,NULL,0.0f);	

	//reset preset.. this maybe helps..
	int current_preset=ptrPlug->dispatcher(ptrPlug,effGetProgram,0,0,0,0.0f);
	ptrPlug->dispatcher(ptrPlug,effSetProgram,0,current_preset,NULL,0.0f);
	//switch the plugin back on (calls Resume)
	ptrPlug->dispatcher(ptrPlug,effMainsChanged,0,1,NULL,0.0f);
	
	if (midi_parameters)
		midi_parameters->send_sound_off();
	
	off_all_notes=true;

}

void VST_Plugin::save(TreeSaver *p_saver) {
	
	if ( !(ptrPlug->flags & effFlagsProgramChunks)) {
		//if chunks are not handled, then save/load the usual way
		SoundPlugin::save(p_saver);
		int current_preset=ptrPlug->dispatcher(ptrPlug,effGetProgram,0,0,0,0.0f);
		p_saver->add_int("__vst_preset",current_preset); //silently add the current preset
		
		/*
		MidiProgramName midi_program;
		memset(&midi_program,0,sizeof(MidiProgramName)); //zero this
		
		int current_prog=ptrPlug->dispatcher(ptrPlug,effGetCurrentMidiProgram,0,0,&midi_program,0.0f);
		printf("current prog is %i\n",current_prog);
		
		memset(&midi_program,0,sizeof(MidiProgramName)); //zero this
		midi_program.thisProgramIndex=current_prog;
		
		int program_is_valid=ptrPlug->dispatcher(ptrPlug,effGetMidiProgramName,0,0,&midi_program,0.0f);
		printf("data - midi program: %i, bank MSB: %i, bank LSB: %i, name is %s - valid is %i \n",midi_program.midiProgram,midi_program.midiBankMsb,midi_program.midiBankLsb,midi_program.name,program_is_valid);
		*/
		return;
	}
		
	/* Save chunk */

	void *data;
	int data_size=ptrPlug->dispatcher (ptrPlug, effGetChunk, 0, 0, &data, 0.0f);
	//I think i should just save the preset, but in every example I saw, they save the whole
	//thing as a preset.. so.. I just do it too!
		
	p_saver->add_raw("vst_chunk",(const unsigned char*)data,data_size);
	
	int current_preset=ptrPlug->dispatcher(ptrPlug,effGetProgram,0,0,0,0.0f);
	p_saver->add_int("vst_preset",current_preset);
	
	/* Save Gain */
	p_saver->add_float("vst_gain",gain.get());
	
	if (!midi_parameters)
		return;
	/* Save midi parameters, if used */
	p_saver->enter("midi_params");
	
	for (int i=0;i<midi_parameters->get_parameter_count();i++) {
		
		Property *p=midi_parameters->get_parameter(i);
		p_saver->add_float(p->get_name(),p->get());
	}
	
	p_saver->exit();
	
		
}
void VST_Plugin::load(TreeLoader *p_saver) {
	
	if ( !(ptrPlug->flags & effFlagsProgramChunks)) {
		//if chunks are not handled, then save/load the usual way
		//also, make sure to set the previous preset so loading is simply overriden
						
		ptrPlug->dispatcher(ptrPlug,effSetProgram,0,p_saver->get_int("__vst_preset"),NULL,0.0f);
		SoundPlugin::load(p_saver);
		return;
	}
	
	/* Load chunk */
	
	if (p_saver->is_var("vst_chunk")) {
		
		int len=p_saver->get_raw_len("vst_chunk");
		ERR_FAIL_COND(len<=0);
		
		unsigned char *data = new unsigned char[len];

		p_saver->get_raw("vst_chunk",data);
		
		ptrPlug->dispatcher (ptrPlug, effSetChunk, 0, len, data, 0.0f);
		
		delete[] data;
	}
		
	ptrPlug->dispatcher(ptrPlug,effSetProgram,0,p_saver->get_int("vst_preset"),NULL,0.0f);
	
	/* Load Gain */
	gain.set( p_saver->get_float("vst_gain") );
	
	if (!midi_parameters)
		return;
	/* Save midi parameters, if used */
	p_saver->enter("midi_params");
	
	for (int i=0;i<midi_parameters->get_parameter_count();i++) {
		
		Property *p=midi_parameters->get_parameter(i);
		if (p_saver->is_var(p->get_name()))
			p->set( p_saver->get_float( p->get_name() ) );

	}
	
	p_saver->exit();
	
}


VST_Plugin::VST_Plugin(const SoundPluginInfo *p_info,String p_path,String p_dir,bool p_ports_write_only,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	plugin_dir=p_dir;
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
	
	int in_port_count=ptrPlug->numInputs;
	int out_port_count=ptrPlug->numOutputs;

	/* Create buffers for channels */
	
	if (in_port_count)
		input_buffers = new float*[ptrPlug->numInputs];
	else
		input_buffers=NULL;
	
	if (out_port_count)
		output_buffers = new float*[ptrPlug->numOutputs];
	else
		output_buffers=NULL;
	
	/* Depending on channel amount selected, assign plugs accordingly */
	
	while(in_port_count) {

		if (in_port_count>=p_channels) {
			
			input_plugs.push_back( new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this) );
			in_port_count-=p_channels;
		} else {
			
			input_plugs.push_back( new AudioPlug(in_port_count,AudioPlug::TYPE_INPUT,this) );
			in_port_count=0;
			
		}
	}


	while(out_port_count) {

		if (out_port_count>=p_channels) {
			
			output_plugs.push_back( new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this) );
			out_port_count-=p_channels;
		} else {
			
			output_plugs.push_back( new AudioPlug(out_port_count,AudioPlug::TYPE_OUTPUT,this) );
			out_port_count=0;
			
		}
	}
	
	mix_rate=44100;
	//Suspend Plugin
	ptrPlug->dispatcher(ptrPlug,effMainsChanged,0,0,NULL,0.0f);	
	//set some crap default blocksize(128)
	ptrPlug->dispatcher(ptrPlug,effSetBlockSize,0,128,NULL,0.0f);
	
	//create parameters
	for (int i=0;i<ptrPlug->numParams;i++) {
		if (i>200) //I'm sorry but some plugins (FM7) can go mad with declaring unused params!
			break; 		
		Parameter *p=new Parameter(ptrPlug,i,p_ports_write_only);
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
	off_all_notes=false;
	gain.set_all( get_info()->is_synth?-24:0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "vst_plugin_gain","Gain","dB");
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
