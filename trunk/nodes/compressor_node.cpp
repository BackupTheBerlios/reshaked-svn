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
#include "compressor_node.h"
#include "dsp/formulas.h"

void CompressorNode::mix_rate_changed() {

	compressor->set_mix_rate( get_mix_rate() );
}

AudioNode *CompressorNode::creation_func_peak(int p_channels,const AudioNodeInfo *p_info) {

	return new CompressorNode(p_channels,p_info,false);
}


const AudioNodeInfo *CompressorNode::get_creation_info_peak() {

	static AudioNodeInfo _info;
	_info.caption="Compressor (Peak)";
	_info.short_caption="CompressPk";
	_info.description="Aggressive Compressor";
	_info.unique_ID="INTERNAL_CompressorNodePeak";
	_info.creation_func=&CompressorNode::creation_func_peak;
	_info.icon_string="node_compressor";
	_info.category="Effects";
	_info.custom_channels.push_back(1);
	_info.custom_channels.push_back(2);
	_info.custom_channels.push_back(4);

	return &_info;
}

AudioNode *CompressorNode::creation_func_rms(int p_channels,const AudioNodeInfo *p_info) {

	return new CompressorNode(p_channels,p_info,true);
}


const AudioNodeInfo *CompressorNode::get_creation_info_rms() {

	static AudioNodeInfo _info;
	_info.caption="Compressor (RMS)";
	_info.short_caption="CompressPk";
	_info.description="Music-Oriented Compressor";
	_info.unique_ID="INTERNAL_CompressorNodeRMS";
	_info.creation_func=&CompressorNode::creation_func_rms;
	_info.icon_string="node_compressor";
	_info.category="Effects";
	_info.custom_channels.push_back(1);
	_info.custom_channels.push_back(2);
	_info.custom_channels.push_back(4);

	return &_info;
}

template<class CompType>
void CompressorNode::process(const ProcessInfo& p_info,CompType *p_compressor) {

	float pre_gain_nrg=db_to_energy( pre_gain() );
	float post_gain_nrg=db_to_energy( post_gain() );

	if (get_channels()==1) {
	
	} else if (get_channels()==2) {
	
		float *src_l=get_audio_buffer( PORT_IN, 0, 0 );
		float *src_r=get_audio_buffer( PORT_IN, 0, 1 );
		
		float *side_l=get_audio_buffer( PORT_IN, 1, 0 );
		float *side_r=get_audio_buffer( PORT_IN, 1, 1 );
		
		float *dst_l=get_audio_buffer( PORT_OUT, 0, 0 );
		float *dst_r=get_audio_buffer( PORT_OUT, 0, 1 );
		
		if (mode()==0) { // normal
		
			for(int i=0;i<p_info.audio_buffer_size;i++) {
				
				float l=src_l[i]*pre_gain_nrg;
				float r=src_r[i]*pre_gain_nrg;
				
				float gain = p_compressor->process_2( l,r );
				gain*=post_gain_nrg;
				dst_l[i]=src_l[i]*gain;
				dst_r[i]=src_r[i]*gain;
			}
		} else { // sidechain
		
			for(int i=0;i<p_info.audio_buffer_size;i++) {
				
				float gain = p_compressor->process_2( pre_gain_nrg*side_l[i], pre_gain_nrg*side_r[i] );
				gain*=post_gain_nrg;
				dst_l[i]=src_l[i]*gain;
				dst_r[i]=src_r[i]*gain;
			}
		}
	
	}

}


void CompressorNode::process(const ProcessInfo& p_info) {

	compressor->set_attack( attack()/1000.0 );
	compressor->set_release( release()/1000.0 );
	compressor->set_threshold( threshold() );
	compressor->set_ratio( ratio() );	
	
	if (compressor_peak)
		process(p_info,compressor_peak);
	else if (compressor_rms)
		process(p_info,compressor_rms);

}



CompressorNode::CompressorNode(int p_instanced_channels,const AudioNodeInfo *p_info,bool p_rms) : HL_AudioNode(p_instanced_channels,p_info) {

	if (p_instanced_channels<1)
		p_instanced_channels=1;
	
	if (p_rms) {
	
		compressor_rms = new CompressorRMS;
		compressor=compressor_rms;
	} else {
	
		compressor_peak = new CompressorPeak;
		compressor=compressor_peak;	
	}
	
	add_audio_port("Input",PORT_IN);
	add_audio_port("SideChain",PORT_IN);
	add_audio_port("Compressed",PORT_OUT);	
	

		
	std::vector<String> mode_strings;
	mode_strings.push_back("Normal");
	mode_strings.push_back("SideChain");
	
	mode.set_enum("Mode",mode_strings);
	pre_gain.set_all( 0, -60, 24, 0.1, ControlPort::HINT_RANGE, "PreGain","dB");
	threshold.set_all( 0, -60, 0, 0.1, ControlPort::HINT_RANGE, "Threshold","dB");
	ratio.set_all( 1, 1, 50, 0.1, ControlPort::HINT_RANGE, "Ratio",":1");
	attack.set_all( 100, 1, 800, 1, ControlPort::HINT_RANGE, "Attack","ms");
	release.set_all( 400, 1, 800, 1, ControlPort::HINT_RANGE, "Release","ms");
	post_gain.set_all( 0, -60, 24, 0.1, ControlPort::HINT_RANGE, "PostGain","dB");
	
	add_control_port( PORT_IN, &mode);
	add_control_port( PORT_IN, &pre_gain);
	add_control_port( PORT_IN, &threshold);
	add_control_port( PORT_IN, &ratio);
	add_control_port( PORT_IN, &attack);
	add_control_port( PORT_IN, &release);
	add_control_port( PORT_IN, &post_gain);
	
}


CompressorNode::~CompressorNode() {
		
	delete compressor;
}

