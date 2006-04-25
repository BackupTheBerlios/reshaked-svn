//
// C++ Implementation: sound_plugin_ladspa
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_ladspa.h"

#include "version.h"
#ifdef LADSPA_ENABLED


#include "error_macros.h"
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#define DEFAULT_PORT_FREQ 20000.0

namespace ReShaked {


int SoundPlugin_LADSPA::get_input_plug_count() {
	
	return input_plugs.size();
}
int SoundPlugin_LADSPA::get_output_plug_count() {
	
	return output_plugs.size();
	
}
AudioPlug *SoundPlugin_LADSPA::get_input_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,input_plugs.size(),NULL);
	return input_plugs[p_index];	
}

AudioPlug *SoundPlugin_LADSPA::get_output_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,output_plugs.size(),NULL);
	return output_plugs[p_index];	
}

int SoundPlugin_LADSPA::get_port_count() {
	
	return input_ports.size()+output_ports.size();
}
Property& SoundPlugin_LADSPA::get_port(int p_index) {
	
	static LocalProperty error;
	
	ERR_FAIL_INDEX_V(p_index,get_port_count(),error);
	
	if (p_index<input_ports.size()) {
		
		return input_ports[p_index]->property;
	} else {
		return output_ports[p_index-input_ports.size()]->property;

	}
	
}
SoundPlugin::PortType SoundPlugin_LADSPA::get_port_type(int p_index) {
	
	if (p_index<input_ports.size()) {
		
		return TYPE_WRITE;
	} else {
		
		return TYPE_READ;
	}
	
}

/* Setting up */
void SoundPlugin_LADSPA::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_freq=p_mixing_rate;
	reset();
		
}

/* Processing */
void SoundPlugin_LADSPA::process(int p_frames) {
	
	if (!working)
		return;
	connect_audio_ports(); //must be done always
	
	for (int i=0;i<input_ports.size();i++) {
		
		if (input_ports[i]->use_freq_adj) {
			
			input_ports[i]->value=input_ports[i]->freq_adj*mix_freq/DEFAULT_PORT_FREQ;
		}
	}
	
	for (int i=0;i<instances.size();i++) {
		descriptor->run(instances[i], p_frames);
	}
}


void SoundPlugin_LADSPA::connect_audio_ports() {
	

	int in_count=0,out_count=0;

	for (int i=0;i < (int)descriptor->PortCount;i++) {

		if (LADSPA_IS_PORT_AUDIO(descriptor->PortDescriptors[i])) {
		
			if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[i])) {
				
				if (stereo_mode) {
					
					if (in_count<2) {
						
						AudioPlug *ap=input_plugs[0];
						descriptor->connect_port(instances[0], i, ap->get_buffer()->get_buffer(in_count));								
					}
					
				} else {
					
					ERR_CONTINUE(in_count>=input_plugs.size());
					ERR_CONTINUE(instances.size()!=input_plugs[in_count]->get_channels());
					
					for (int j=0;j<instances.size();j++) {
						
						AudioPlug *ap=input_plugs[in_count];
						descriptor->connect_port(instances[j], i, ap->get_buffer()->get_buffer(j));					
						
					}				
				}
				
				in_count++;
		
			} else if (LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[i])) {
						
				if (stereo_mode) {
				
					if (out_count<2) {
					
						AudioPlug *ap=output_plugs[0];
						descriptor->connect_port(instances[0], i, ap->get_buffer()->get_buffer(out_count));								
					}
				
				} else {
				
					ERR_CONTINUE(out_count>=output_plugs.size());
					ERR_CONTINUE(instances.size()!=output_plugs[out_count]->get_channels());
				
					for (int j=0;j<instances.size();j++) {
					
						AudioPlug *ap=output_plugs[out_count];
						descriptor->connect_port(instances[j], i, ap->get_buffer()->get_buffer(j));					
					
					}				
				}
			
				out_count++;
				
			}	
		}
	}
}

void SoundPlugin_LADSPA::reset() {
	
	printf("RESET called\n");

	if (!working)
		return;

	/* Reset exiting handles (instances) */
	
	for (int i=0;i<instances.size();i++) {
		
		if (instances[i]) {
		
			if (descriptor->deactivate) {
				descriptor->deactivate(instances[i]);
			}
			//cleanup
			if (descriptor->cleanup)
				descriptor->cleanup(instances[i]);
			//recreate
		}
		instances[i]=descriptor->instantiate(descriptor,(int)mix_freq);		

	}

	

	int inport_count=0,outport_count=0;
	printf("Relink to properties\n");
	/* If you dont connect all the input/outputs to something, the plugin will CRASH!!!" */

	for (int i=0;i < (int)descriptor->PortCount;i++) {

		if (LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[i])) {
//			const LADSPA_PortRangeHint *hints = descriptor->PortRangeHints;
			if (LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[i])) {
				
				
				
				for (int j=0;j<instances.size();j++) {
					
					int idx=outport_count*instances.size()+j;
					ERR_CONTINUE(idx<0 || idx>=output_ports.size());
							
					descriptor->connect_port(instances[j], i, &output_ports[idx]->value);

				}

				outport_count++;
				
			} else if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[i])) {

				for (int j=0;j<instances.size();j++) {
					
					int idx=inport_count;
					if (idx<0 || idx>=input_ports.size()) {
						
						printf("idx %i / %i\n",idx,input_ports.size());
					}
					ERR_CONTINUE(idx<0 || idx>=input_ports.size());
							
					descriptor->connect_port(instances[j], i, &input_ports[idx]->value);

				}
				
				inport_count++;
				
			}
		} 
	}

	for (int i=0;i<instances.size();i++) {
		if (descriptor->activate)
			descriptor->activate( instances[i] );
	}
}

SoundPlugin_LADSPA::SoundPlugin_LADSPA(const SoundPluginInfo *p_info,String p_path,int p_index,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	
	if (p_channels<1)
		p_channels=1;
	
	library=NULL;
	working=false;
	library = dlopen(p_path.utf8().get_data(), RTLD_LAZY);
	ERR_FAIL_COND(library == NULL);

	
	LADSPA_Descriptor_Function descriptor_fn;
	descriptor_fn = (LADSPA_Descriptor_Function)dlsym(library, "ladspa_descriptor");
	if (descriptor_fn == NULL)
		descriptor_fn = (LADSPA_Descriptor_Function)dlsym(library, "_ladspa_descriptor"); //little hax0r

	if (descriptor_fn == NULL) {
		dlclose(library);
	}	
	
	ERR_FAIL_COND(descriptor_fn == NULL);

	descriptor= descriptor_fn(p_index);
	
	if (descriptor == NULL) { //wrong p_index?
		dlclose(library);
	}	
	
	ERR_FAIL_COND(descriptor == NULL);
	
	int out_count=0,in_count=0;

	for (int i=0; i < (int)descriptor->PortCount; i++) {
	
	
		if (LADSPA_IS_PORT_AUDIO(descriptor->PortDescriptors[i])) {

			if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[i]))
				in_count++;
			if (LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[i]))
				out_count++;
		}
	} 
	
	
	

	/* I ASSUME plugin is stereo... (only one instance needed) */
	
	stereo_mode=(in_count == 2 && out_count == 2);
	
	if (in_count>2 || out_count>2)
		p_channels=1; //ignore requested channels and just instance individual ones. 
	
	if (p_channels!=2 && stereo_mode) {
		
		dlclose(library);
		ERR_FAIL_COND(p_channels!=2 && stereo_mode); //buggo
	}
	
	if (stereo_mode) {
		   
		input_plugs.push_back( new AudioPlug(2,AudioPlug::TYPE_INPUT,this) );
		output_plugs.push_back( new AudioPlug(2,AudioPlug::TYPE_OUTPUT,this) );
		
		instances.resize(1);
	} else {
		
		for (int i=0;i<in_count;i++) {
			
			input_plugs.push_back( new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this) );
		}
		
		for (int i=0;i<out_count;i++) {
			
			output_plugs.push_back( new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this) );
		}
		
		instances.resize(p_channels);
	}
	
	for (int i=0;i<instances.size();i++) {
		
		instances[i]=NULL;
	}
	
	/* now that we know how many instances, create ports */
	
	for (int i=0; i<(int)descriptor->PortCount; i++) {

		const LADSPA_PortRangeHint *hints = descriptor->PortRangeHints;

		if (LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[i])) {

			bool gay_sample_rate=false;
			if (LADSPA_IS_HINT_SAMPLE_RATE(hints[i].HintDescriptor))
				gay_sample_rate=true;

			String port_name=descriptor->PortNames[i];
			String port_var_name="descr_" + String::num(i);
			float min,max,def;
			bool is_int=false;
			bool is_toggle=false;
			float step;
		


			if (LADSPA_IS_HINT_BOUNDED_BELOW(hints[i].HintDescriptor)) {
				min= hints[i].LowerBound;
			} else {
				min= -9999.0f;
			}

			if (LADSPA_IS_HINT_BOUNDED_ABOVE(hints[i].HintDescriptor)) {
				max= hints[i].UpperBound;
			} else {
				max= 9999.0f;
			}



			/* infinity */

			if (LADSPA_IS_HINT_INTEGER(hints[i].HintDescriptor)) {
				is_int=true;
			}

			if (LADSPA_IS_HINT_DEFAULT_MINIMUM(hints[i].HintDescriptor)) {

				def=min;
			} else if (LADSPA_IS_HINT_DEFAULT_LOW(hints[i].HintDescriptor)) {

				def=min + (max-min) * 0.25;
			} else if (LADSPA_IS_HINT_DEFAULT_HIGH(hints[i].HintDescriptor)) {

				def=min + (max-min) * 0.75;
			} else if (LADSPA_IS_HINT_DEFAULT_MAXIMUM(hints[i].HintDescriptor)) {

				def=max;
			} else if (LADSPA_IS_HINT_DEFAULT_0(hints[i].HintDescriptor)) {

				def=0.0f;
			} else if (LADSPA_IS_HINT_DEFAULT_1(hints[i].HintDescriptor)) {

				def=1.0f;
			} else if (LADSPA_IS_HINT_DEFAULT_100(hints[i].HintDescriptor)) {

				def=100.0f;
			} else if (LADSPA_IS_HINT_DEFAULT_440(hints[i].HintDescriptor)) {

				def=440.0f;
			} else if (LADSPA_IS_HINT_INTEGER(hints[i].HintDescriptor)) {

				def=min;
			} else if (max >= 0.0f && min <= 0.0f) {

				def = 0.0f;
			} else { //middle by default...
				if (min<0.0f)
					def=0.0f;
				else
					def=min + (max-min) * 0.5;
			}

			
			if (gay_sample_rate) {
				
				min*=DEFAULT_PORT_FREQ;
				max*=DEFAULT_PORT_FREQ;
				def*=DEFAULT_PORT_FREQ;
			}
			
			if (is_int) {
				
				step=1;
			} else if (fabsf(max)>1000.0 || fabsf(min)>1000.0) {
			
				step=0.1;
			} else {
				
				step=0.01;
			}
			
			if (LADSPA_IS_HINT_TOGGLED(hints[i].HintDescriptor)) {

				min=0;
				max=1;
				def=0;
				step=1;
			}
			
			
			if (LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[i])) {
				
				if (instances.size()==1) {
					
					SharedProperty *prop = new SharedProperty;
			
					prop->property.config(port_var_name,port_name,&prop->value,min,max,step,def);
				
					output_ports.push_back(prop);
				} else {
					for (int j=0;j<instances.size();j++) {
						
						SharedProperty *prop = new SharedProperty;
				
						prop->property.config(port_var_name+"_" +String::num(j+1),port_name+" " +String::num(j+1),&prop->value,min,max,step,def);
	
						output_ports.push_back(prop);
					}
				}				
			} else {
				SharedProperty *prop = new SharedProperty;
			
				if (gay_sample_rate) {
					prop->use_freq_adj=true;

					prop->property.config(port_var_name,port_name,&prop->freq_adj,min,max,step,def);
				} else
					prop->property.config(port_var_name,port_name,&prop->value,min,max,step,def);
				
				input_ports.push_back(prop);
			}

		} 
	} 
	
	reset();
	
	working=true;

	mix_freq=44100;
	
}


SoundPlugin_LADSPA::~SoundPlugin_LADSPA(){ 
	
	/* Delete instances */
	for (int i=0;i<instances.size();i++) {
		
		if (!instances[i])
			continue;
		
		if (descriptor->deactivate) {
			descriptor->deactivate(instances[i]);
		}
		descriptor->cleanup(instances[i]);
	}	
	
	/* ports/plugs */
	for (int i=0;i<input_ports.size();i++) 
		delete input_ports[i];
	
	for (int i=0;i<input_plugs.size();i++) 
		delete input_plugs[i];
	
	for (int i=0;i<output_ports.size();i++) 
		delete output_ports[i];
	
	for (int i=0;i<output_plugs.size();i++) 
		delete output_plugs[i];
	
	dlclose(library);	
}


}

#endif
