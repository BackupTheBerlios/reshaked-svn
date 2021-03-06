//
// C++ Implementation: chionic_params
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic_params.h"

namespace ReShaked {

ChionicParams::ChionicParams() {
	
	/* Global */
	
	global.volume.send.set_all(0.7,0.0,1.0,1.0,0.01,Property::DISPLAY_SLIDER,"global_volume_send","Send","","Mute");
	global.volume.sens.set_all(0.2,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_volume_sens","Sens");
	
	global.pan.pos.set_all(0.5,0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pan_pos","Position");
	global.pan.depth.set_all(0.5,0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pan_depth","Depth");
	global.pan.pitch_scale.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pan_freq_scale","Pitch Scaling");
	global.pan.pitch_scale_center.config("global_pan_scale_center","Scale Pivot");
	global.pitch.tune_coarse.set_all(0,-24,24,0,1,Property::DISPLAY_SPIN,"global_pitch_tune_coarse","Coarse");
	global.pitch.tune_fine.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pitch_tune_fine","Fine");
	
	/* Layer */
	
	for (int i=0;i<MAX_LAYERS;i++) {
		
		String c="Layer "+String::num(i+1)+"/";
		String n="layer_"+String::num(i+1)+"_";
		
		layer[i].modulation_mode=Layer::MODE_OFF;
		layer[i].regions.blending.enabled=false;
		layer[i].regions.blending.curve.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,n+"region_blend","Region Blend");
		
		Layer::Parameters &p=layer[i].params;
		
		p.volume.send.set_all(0.7,0.0,1.0,0.7,0.01,Property::DISPLAY_SLIDER,n+"volume_send","Send","","Mute");
		p.volume.velocity_range_begin.set_all(0,0,1,0,0.01,Property::DISPLAY_SLIDER,n+"vel_range_begin","Vel. Range Begin");
		p.volume.velocity_range_end.set_all(1,0,1,1,0.01,Property::DISPLAY_SLIDER,n+"vel_range_end","Vel. Range End");
		
		p.volume.velocity_range_blend_expr.set_all(0,0,1,0,1,Property::DISPLAY_CHECKBOX,n+"vel_range_blend_expr","Vel. Range Blend Expression");
		
		p.volume.velocity_range_blend.set_all(0,0,1,0,0.01,Property::DISPLAY_SLIDER,n+"vel_range_blend","Vel. Range Blend");
		
		
		p.volume.pitch_scale.set_all(0,-1.0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"volume_pitch_scale","Pitch Scaling");
		
		p.volume.random.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"volume_random","Random");
		p.volume.envelope.reset(false);
		p.volume.envelope.set_max_value(1.0);
		p.volume.envelope.set_min_value(0.0);
		p.volume.envelope.set_prefix("",2);
		p.volume.envelope.add_node_at_offset(0,1);
		p.volume.envelope.add_node_at_offset(200,1);
		p.volume.lfo.set_rate_unit_size(1000); //msecs
		
		
		
		std::vector<String> filter_types;
		filter_types.push_back("Disabled");
		filter_types.push_back("Lowpass");
		filter_types.push_back("Hipass");
		filter_types.push_back("Bandpass");
		filter_types.push_back("Notch");
		
		p.filter.type.set_all(n+"filter_type","Type",filter_types,0);
		p.filter.cutoff.set_all(1024,50,16000,1024,0.1,Property::DISPLAY_SLIDER,n+"filter_cutoff","Cutoff");
		p.filter.resonance.set_all(0,0,3,0,0.01,Property::DISPLAY_SLIDER,n+"filter_resonance","Resonance");
		
		p.filter.cutoff_velsens.set_all(0,-10.0,10.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_cutoff_velsens","VS Cut","oct");
		p.filter.resonance_velsens.set_all(0,-1.0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_resonance_velsens","VS Reso");
		p.filter.envelope_cutoff.set_max_value(128.0);
		p.filter.envelope_cutoff.set_min_value(-128.0);
		p.filter.envelope_cutoff.set_prefix("smt",1);
		p.filter.lfo_cutoff.set_max_depth(128.0);
		p.filter.lfo_cutoff.set_rate_unit_size(1000); //msecs
		p.filter.lfo_resonance.set_rate_unit_size(1000); //msecs
		
	
		p.filter.freq_tracking.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_freq_track","Pitch Track");
		
		p.filter.random.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_random","Random Cutoff");
		
		p.pan.pos.set_all(0.5,0,1.0,0.5,0.01,Property::DISPLAY_SLIDER,n+"pan_pos","Position");
		p.pan.depth.set_all(0.5,0,1.0,0.5,0.01,Property::DISPLAY_SLIDER,n+"pan_depth","Depth");
		p.pan.random.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"pan_random","Random");
		p.pan.envelope.set_max_value(1.0);
		p.pan.envelope.set_min_value(-1.0);
		p.pan.envelope.set_prefix("pan",2);
		p.pan.lfo.set_rate_unit_size(1000); //msecs
		
		p.pan.envelope_depth.set_max_value(1.0);
		p.pan.envelope_depth.set_min_value(-1.0);
		p.pan.envelope_depth.set_prefix("pan",2);
		p.pan.lfo_depth.set_rate_unit_size(1000); //msecs
		
		p.pitch.tune_coarse.set_all(0,-24,24,0,1,Property::DISPLAY_SPIN,n+"pitch_tune_coarse","Coarse");
		p.pitch.tune_fine.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,n+"pitch_tune_fine","Fine");
		p.pitch.envelope.set_max_value(24.0);
		p.pitch.envelope.set_min_value(-24.0);
		p.pitch.envelope.set_prefix("smt",1);
		p.pitch.lfo.set_rate_unit_size(1000); //msecs
		p.pitch.lfo.set_max_depth(24.0); //msecs
		
		p.time.delay.set_all(0.0,0.0,4.0,0.0,0.001,Property::DISPLAY_SLIDER,n+"time_delay","Delay");
		p.time.random.set_all(0.0,0.0,0.1,0.0,0.1,Property::DISPLAY_SLIDER,n+"time_random","Random");
	}
	
}

void ChionicParams::Layer::copy_from(Layer *p_from) {
	
	
	
}

void ChionicParams::clear_sources() {
	
	for (int i=0;i<global.sources.size();i++) {
		
		global.sources[i].clear();
	}
	
	global.sources.clear();
	
}
ChionicParams::~ChionicParams() {
	
	clear_sources();
}


}
