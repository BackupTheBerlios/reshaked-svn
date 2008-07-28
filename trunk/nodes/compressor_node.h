//
// C++ Interface: compressor_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPRESSOR_NODE_H
#define COMPRESSOR_NODE_H
#include "engine/hl_audio_node.h"
#include "dsp/compressor.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class CompressorNode : public HL_AudioNode {
	
	HL_EnumControlPort mode;	
	HL_ControlPort pre_gain;
	HL_ControlPort threshold;
	HL_ControlPort ratio;
	HL_ControlPort attack;
	HL_ControlPort release;
	HL_ControlPort post_gain;

	CompressorBase *compressor;
	CompressorPeak *compressor_peak;
	CompressorRMS *compressor_rms;

	template<class CompType>
	void process(const ProcessInfo& p_info,CompType *p_compressor);
	
	virtual void mix_rate_changed(); 
	virtual void process(const ProcessInfo& p_info);

	CompressorNode(int p_instanced_channels,const AudioNodeInfo *p_info,bool p_rms);	
	
	static AudioNode *creation_func_peak(int p_channels,const AudioNodeInfo *p_info);
	static AudioNode *creation_func_rms(int p_channels,const AudioNodeInfo *p_info);
	
public:

	static const AudioNodeInfo *get_creation_info_peak();
	static const AudioNodeInfo *get_creation_info_rms();


	~CompressorNode();

};

#endif
