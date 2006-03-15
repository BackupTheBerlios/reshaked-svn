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
	
	global.volume.send.set_all(1.0,0.0,1.0,1.0,0.01,Property::DISPLAY_SLIDER,"global_volume_send","Send","","Mute");
	global.volume.sens.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_volume_sens","Sens");
	
	global.pan.pos.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pan_pos","Position");
	global.pan.pitch_scale.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pan_pitch_scale","Pitch/Scale");
	global.pan.pitch_scale_center.set_all(60,0,119,60,1,Property::DISPLAY_SPIN,"global_pan_scale_center","Scale Center");
	
	global.pitch.tune_coarse.set_all(0,-24,24,0,1,Property::DISPLAY_SPIN,"global_pitch_tune_coarse","Coarse");
	global.pitch.tune_fine.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,"global_pitch_tune_fine","Fine");
	
	/* Layer */
	
	for (int i=0;i<MAX_LAYERS;i++) {
		
		String c="Layer "+String::num(i+1)+"/";
		String n="layer_"+String::num(i+1)+"_";
		
		Layer::Parameters &p=layer[i].params;
		
		p.volume.send.set_all(0.7,0.0,1.0,0.7,0.01,Property::DISPLAY_SLIDER,n+"volume_send","Send","","Mute");
		p.volume.velocity_range_begin.set_all(0,0,1,0,0.01,Property::DISPLAY_SLIDER,n+"vel_range_begin","Vel. Range Begin");
		p.volume.velocity_range_end.set_all(1,0,1,1,0.01,Property::DISPLAY_SLIDER,n+"vel_range_end","Vel. Range End");
		
		p.volume.velocity_range_begin_level.set_all(1,0,1,1,0.01,Property::DISPLAY_SLIDER,n+"vel_range_begin_level","Vel. Range Begin");
		
		p.volume.velocity_range_end_level.set_all(1,0,1,1,0.01,Property::DISPLAY_SLIDER,n+"vel_range_end_level","Vel. Range End");
		
		p.volume.pitch_scale.set_all(0,-1.0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"volume_pitch_scale","Pitch Scaling");
		
		p.volume.random.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"volume_random","Random");
		
		std::vector<String> filter_types;
		filter_types.push_back("Disabled");
		filter_types.push_back("Lowpass");
		filter_types.push_back("Hipass");
		filter_types.push_back("Bandpass");
		filter_types.push_back("Notch");
		
		p.filter.type.set_all(n+"filter_type","Type",filter_types,0);
		p.filter.cutoff.set_all(1024,0,16000,1024,0.1,Property::DISPLAY_SLIDER,n+"filter_cutoff","Cutoff");
		p.filter.resonance.set_all(0,0,3,0,0.01,Property::DISPLAY_SLIDER,n+"filter_resonance","Resonance");
		
		p.filter.cutoff_velsens.set_all(0,-1.0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_cutoff_velsens","VS Cut");
		p.filter.resonance_velsens.set_all(0,-1.0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_resonance_velsens","VS Reso");
		
	
		p.filter.freq_tracking.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_freq_track","Pitch Track");
		
		p.filter.random.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"filter_random","Random Cutoff");
		
		p.pan.pos.set_all(0.5,0,1.0,0.5,0.01,Property::DISPLAY_SLIDER,n+"pan_pos","Position");
		p.pan.depth.set_all(0.5,0,1.0,0.5,0.01,Property::DISPLAY_SLIDER,n+"pan_depth","Depth");
		p.pan.random.set_all(0,0,1.0,0,0.01,Property::DISPLAY_SLIDER,n+"pan_random","Random");
		
		p.pitch.tune_coarse.set_all(0,-24,24,0,1,Property::DISPLAY_SPIN,n+"pitch_tune_coarse","Coarse");
		p.pitch.tune_fine.set_all(0.0,-1.0,1.0,0.0,0.01,Property::DISPLAY_SLIDER,n+"pitch_tune_fine","Fine");
		
		p.time.delay.set_all(0.0,0.0,4.0,0.0,0.00001,Property::DISPLAY_SLIDER,n+"time_delay","Delay");
		p.time.random.set_all(0.0,0.0,0.1,0.0,0.01,Property::DISPLAY_SLIDER,n+"time_random","Random");
	}
	
}


ChionicParams::~ChionicParams()
{
}


}
