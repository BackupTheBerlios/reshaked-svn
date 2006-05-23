//
// C++ Implementation: compressor_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "compressor_plugin.h"

#include <math.h>
#include "dsp/formulas.h"

#include "pixmaps/icon_compressor.xpm"

namespace ReShaked {

static SoundPlugin* create_compressor(const SoundPluginInfo *p_info,int p_channels) {
	
	return new CompressorPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *CompressorPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Compressor";
	info.description="Compressor with SideChain";
	info.long_description="Envelope-less compressor that keeps signal level changes below a specific threshold. Alternatively supports a SideChain, which allows to apply the compressor effect from a source to another. Unlike the limiter, the focus of this effect is to be produce nicer sound, at the cost of being less strict.";
	info.unique_ID="INTERNAL_compressor"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_compressor_xpm;
	info.creation_func=&create_compressor;
	info.version=1;	
	return &info;
}


int CompressorPlugin::get_input_plug_count() {
	
	return 2;
}
int CompressorPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *CompressorPlugin::get_input_plug(int p_index) {
	
	if (p_index==0)
		return input_plug;	
	else
		return sidechain_plug;
}

AudioPlug *CompressorPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int CompressorPlugin::get_port_count() {
	
	return property_list.size();
}
Property& CompressorPlugin::get_port(int p_index) {
	
	static LocalProperty error_prop;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error_prop);
	
	return *property_list[p_index];
}
SoundPlugin::PortType CompressorPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void CompressorPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();
}

void CompressorPlugin::reset() {
	
	if (avg_buffer)
		delete avg_buffer;
	
	avg_size=lrint((float)MAX_AVG_BUFFER_MS/1000.0*mix_rate);
	
	avg_buffer= new float[avg_size];
	
	for (int i=0;i<avg_size;i++)
		avg_buffer[i]=0;
	avg_pos=0;
	avg=0;	
	current_avg_len=-1; //make sure it's updated
	
}

void CompressorPlugin::process_1(int p_frames) {
	
	
	
}
void CompressorPlugin::process_2(int p_frames) {
	
	float *src_l=input_plug->get_buffer()->get_buffer( 0 );
	float *src_r=input_plug->get_buffer()->get_buffer( 1 );
	
	float *side_l=sidechain_plug->get_buffer()->get_buffer( 0 );
	float *side_r=sidechain_plug->get_buffer()->get_buffer( 1 );
	
	float *dst_l=output_plug->get_buffer()->get_buffer( 0 );
	float *dst_r=output_plug->get_buffer()->get_buffer( 1 );
	
	float pregain_f = db_to_energy( pre_gain.get() );
	float postgain_f = db_to_energy( post_gain.get() );
	
	float threshold_f = db_to_energy( threshold.get() );
	
	float ratio_f = ratio.get();
	
	bool apply_to_sidechain=(mode.get()>0);
	
	for (int i=0;i<p_frames;i++) {
		
		float l=src_l[i]*pregain_f;
		float r=src_r[i]*pregain_f;
		
		float frame_nrg=(fabsf(l)+fabsf(r))/2.0; //as said, nice sound over strictness
		
		//will the error round itself? I can't tell :( but when you stop, this gets reset anyway
		avg-=(double)avg_buffer[avg_pos];
		avg+=(double)frame_nrg;
		undenormalise(avg);
		avg_buffer[avg_pos]=frame_nrg;
		if ( (++avg_pos)>current_avg_len)
			avg_pos=0;
		
		float signal_nrg=(float)avg/(float)current_avg_len;
		
		if (avg==0)
			avg=0;
		
		float ratio_adjust=1.0;
		
		if (threshold_f<signal_nrg) {
		
			
			// ratio between the energy we have and the enrgy we want, this is the hard-compression
			float signal_to_threshold_ratio=threshold_f/signal_nrg;
			//the difference between the hard-compression and no-compression
			float difference=1.0-signal_to_threshold_ratio;
			
			difference/=ratio_f; //apply the compression ratio to this difference
			
			ratio_adjust=signal_to_threshold_ratio+difference; //add the difference as the ratio
		
		}
		
		ratio_adjust*=postgain_f;
		
		if (apply_to_sidechain) {
			
			dst_l[i]=ratio_adjust*side_l[i];
			dst_r[i]=ratio_adjust*side_r[i];
			
		} else {
			
			dst_l[i]=ratio_adjust*l;
			dst_r[i]=ratio_adjust*r;
			
		}
		
	}
	
}
void CompressorPlugin::process_4(int p_frames) {
	
	
}


/* Processing */
void CompressorPlugin::process(int p_frames) {
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
	int desired_avg_len=lrint((response.get()/1000.0)*mix_rate);
	
	/* make sure it's no greater than buffer size */
	if (desired_avg_len>avg_size)
		desired_avg_len=avg_size;
	
	if (desired_avg_len!=current_avg_len) {
		
		/* Reset average buffer, this will corrupt the sound but there's nothing I can do */
		current_avg_len=desired_avg_len;
		for (int i=0;i<current_avg_len;i++)
			avg_buffer[i]=0;
		avg=0;
		avg_pos=0;
		
	}
	
	if (get_channels_created()==1)
		process_1(p_frames);
	else if (get_channels_created()==2)
		process_2(p_frames);
	else if (get_channels_created()==4)
		process_4(p_frames);
	
}


CompressorPlugin::CompressorPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	avg_buffer=NULL;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	sidechain_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	sidechain_plug->set_name("Side");
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	std::vector<String> mode_strings;
	mode_strings.push_back("Normal");
	mode_strings.push_back("Side");
	
	mode.set_all( "mode","Mode",mode_strings );
	pre_gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "pre_gain","PreGain","dB");
	threshold.set_all( 0, -60, 0, 0, 0.1, Property::DISPLAY_SLIDER, "threshold","Threshold","dB");
	ratio.set_all( 1, 1, 50, 1, 0.1, Property::DISPLAY_SLIDER, "ratio","Ratio",":1");
	response.set_all( 20, 1, MAX_AVG_BUFFER_MS, 1, 0.1, Property::DISPLAY_SLIDER, "response","Response","ms");
	post_gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "post_gain","PostGain","dB");
	
	property_list.push_back(&mode);
	property_list.push_back(&pre_gain);
	property_list.push_back(&threshold);
	property_list.push_back(&ratio);
	property_list.push_back(&response);
	property_list.push_back(&post_gain);
	
	set_mixing_rate( 44100 );
	
}


CompressorPlugin::~CompressorPlugin() { 
	
	delete input_plug;
	delete output_plug;
	delete sidechain_plug;
	if (avg_buffer)
		delete[] avg_buffer;
}


}
