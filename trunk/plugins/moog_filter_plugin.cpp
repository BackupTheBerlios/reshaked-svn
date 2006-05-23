//
// C++ Implementation: moog_filter_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "moog_filter_plugin.h"

#include <math.h>
#include "pixmaps/icon_moog_svf.xpm"

namespace ReShaked {

static SoundPlugin* create_moog(const SoundPluginInfo *p_info,int p_channels) {
	
	return new MoogFilterPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *MoogFilterPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Moog SVF";
	info.description="State Variable VCF";
	info.long_description="Emulation of a vintage VCF (Voltage Controlled Filter) with warm resonance and progressive Lowpass/Bandpass/Highpass transition.";
	info.unique_ID="INTERNAL_moog_svf"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_moog_svf_xpm;
	info.creation_func=&create_moog;
	info.version=1;	
	return &info;
}


int MoogFilterPlugin::get_input_plug_count() {
	
	return 1;
}
int MoogFilterPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *MoogFilterPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *MoogFilterPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int MoogFilterPlugin::get_port_count() {
	
	return property_list.size();
}
Property& MoogFilterPlugin::get_port(int p_index) {
	
	
	static LocalProperty error_prop;
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error_prop);
	
	return *property_list[p_index];
	
}
SoundPlugin::PortType MoogFilterPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void MoogFilterPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
}
void MoogFilterPlugin::reset() {
	
	for (int i=0;i<get_channels_created();i++)
		h[i].clear();

}
/* Processing */
void MoogFilterPlugin::process(int p_frames) {
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
// Moog 24 dB/oct resonant lowpass VCF
// References: CSound source code, Stilson/Smith CCRMA paper.
// Modified by paul.kellett@maxim.abel.co.uk July 2000

	float f, p, q;             //filter coefficients
	float t1, t2;              //temporary buffers

// Set coefficients given frequency & resonance [0.0...1.0]

	float frequency=2*cutoff.get()/mix_rate;
	
	q = 1.0f - frequency;
	p = frequency + 0.8f * frequency * q;
	f = p + p - 1.0f;
	q = resonance .get()* (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
	
	float lpgain=(type.get()<0.5)?(1.0-(type.get()*2.0)):0;
	float bpgain=(type.get()<0.5)?(type.get()*2.0):((1.0-type.get())*2.0);
	float hpgain=(type.get()<0.5)?0:((type.get()-0.5)*2.0);
	

	for (int i=0;i<get_channels_created();i++) {
	
		float b0=h[i].b0;
		float b1=h[i].b1;
		float b2=h[i].b2;
		float b3=h[i].b3;
		float b4=h[i].b4; //history for each 1pole
		
		float *src=input_plug->get_buffer()->get_buffer(i);
		float *dst=output_plug->get_buffer()->get_buffer(i);
	
		for (int j=0;j<p_frames;j++) {
	
			float in=src[j];
			in -= q * b4;                          //feedback
			t1 = b1;  b1 = (in + b0) * p - b1 * f;
			t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
			t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
			b4 = (b3 + t1) * p - b4 * f;
			b4 = b4 - b4 * b4 * b4 * 0.166667f;    //clipping
			b0 = in;
			
			float res=b4*lpgain; //lpass
			res+= ((src[j] - 3.0f * (b3 - b4))-b4)*hpgain; //hpass
			res+=3.0f * (b3 - b4) * bpgain; //bpass
			dst[j]=res;
		}
		
		h[i].b0=undenormalise(b0);
		h[i].b1=undenormalise(b1);
		h[i].b2=undenormalise(b2);
		h[i].b3=undenormalise(b3);
		h[i].b4=undenormalise(b4);
	}

// Lowpass  output:  b4
// Highpass output:  in - b4;
// Bandpass output:  3.0f * (b3 - b4);	
	
}


MoogFilterPlugin::MoogFilterPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	
	h = new History[p_channels];
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	cutoff.set_all( 400, 0.1, 16000, 0.1, 0.1, Property::DISPLAY_SLIDER, "cutoff","Cutoff","hz");
	cutoff.set_quad_coeff( true );
	resonance.set_all( 0, 0, 1, 0, 0.01, Property::DISPLAY_SLIDER, "resonance","Resonance","");
	type.set_all( 0, 0, 1, 0, 0.01, Property::DISPLAY_SLIDER, "mode","Mode","","LPF","HPF");
	
	property_list.push_back(&cutoff);
	property_list.push_back(&resonance);
	property_list.push_back(&type);
	mix_rate=44100;
	reset();	
}


MoogFilterPlugin::~MoogFilterPlugin() { 
	
	delete input_plug;
	delete output_plug;
	delete [] h;
}


}

