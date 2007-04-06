//
// C++ Implementation: distortion_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "distortion_plugin.h"
#include "dsp/formulas.h"
#include "icon_distortion.xpm"
namespace ReShaked {

static SoundPlugin* create_distortion(const SoundPluginInfo *p_info,int p_channels) {
	
	return new DistortionPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *DistortionPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Distortion";
	info.description="This is Work In Progres...";
	info.long_description="Adjust panning or balance for stereo and quad signals.";
	info.unique_ID="INTERNAL_distortion"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_distortion_xpm;
	info.creation_func=&create_distortion;
	info.version=1;	
	return &info;
}


int DistortionPlugin::get_input_plug_count() {
	
	return 1;
}
int DistortionPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *DistortionPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *DistortionPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int DistortionPlugin::get_port_count() {
	
	return property_list.size();
}
Property& DistortionPlugin::get_port(int p_index) {
	
	static LocalProperty error_prop;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error_prop);
	
	return *property_list[p_index];	
}
SoundPlugin::PortType DistortionPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void DistortionPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
}



/* Processing */


void DistortionPlugin::process(int p_frames) {
	
	if (p_frames>MAX_LEN)
		return;
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
	
	//float lpf_c=expf(-2.0*M_PI*pre_lpf.get()/(mix_rate*(float)OVERSAMPLE)); 
	float lpf_c=expf(-2.0*M_PI*pre_lpf.get()/(mix_rate)); 
	float lpf_ic=1.0-lpf_c;
	
	int typei=lrint(type.get());
	float drive_f=drive.get();
	float pregain_f=db_to_energy(pre_gain.get());
	
	
	float clip_db=db_to_energy( -drive_f*60 );
	
	float atan_mult=pow(10,drive_f*drive_f*3.0)-1.0+0.001;
	float atan_div=1.0/(atanf(atan_mult)*(1.0+drive_f*8));
	
	for (int j=0;j<get_channels_created();j++) {
		
		float *src=input_plug->get_buffer()->get_buffer( j );
		float *ovs=buff.get_buffer( j );
		float *dst=output_plug->get_buffer()->get_buffer( j );
		
		/* First, Oversample */
		/*
		for (int i=0;i<p_frames;i++) {
		
			int ofs=i*OVERSAMPLE;
			float val=src[i];
			float preval=pre[j];
			float diff=val-preval;

			
			ovs[ofs]=preval+diff*(1.0/(float)OVERSAMPLE)*1.0;
			ovs[ofs+1]=preval+diff*(1.0/(float)OVERSAMPLE)*2.0;
			ovs[ofs+2]=preval+diff*(1.0/(float)OVERSAMPLE)*3.0;
			ovs[ofs+3]=preval+diff*(1.0/(float)OVERSAMPLE)*4.0;
			ovs[ofs+4]=preval+diff*(1.0/(float)OVERSAMPLE)*5.0;
			ovs[ofs+5]=preval+diff*(1.0/(float)OVERSAMPLE)*6.0;
			ovs[ofs+6]=preval+diff*(1.0/(float)OVERSAMPLE)*7.0;
			ovs[ofs+7]=preval+diff*(1.0/(float)OVERSAMPLE)*8.0;
			
			pre[j]=val;
		
	} */
	
		
		for (int i=0;i<p_frames;i++) {
			
		
	
			float out=undenormalise(src[i]*lpf_ic+lpf_c*h[j]);
			h[j]=out;
			float a=out;
			float ha=src[i]-out; //high freqs
			a*=pregain_f;
					
			switch (typei) {
				
				case TYPE_CLIP: {
							
					if (a>clip_db)
						a=clip_db;
					else if (a<(-clip_db))
						a=-clip_db;
					
				} break;
				case TYPE_ATAN: {
							
	
					a=atanf(a*atan_mult)*atan_div;
					
				} break;
				case TYPE_DECIMATE: {
					a = a*(fabsf(a) + drive_f)/(a*a + (drive_f-1)*fabsf(a) + 1)	;
							
				} break;
				case TYPE_OVERDRIVE: {
				
						
					const double x = a * 0.686306;
					const double z = 1 + exp (sqrt (fabs (x)) * -0.75);
					a = (expf(x) - expf(-x * z)) / (expf(x) + expf(-x));	
				} break;
				case TYPE_WAVESHAPE_1: {
					float x = a;
					float k= 2*drive_f/(1-drive_f);

					a = (1.0+k)*x/(1.0+k*fabsf(x));


				} break;
			}
			
			dst[i]=a+ha;
			
		}
		/*
		for (int i=0;i<p_frames;i++) {
		
			int ofs=i*OVERSAMPLE;

			float r=	ovs[ofs]+
					ovs[ofs+1]+
					ovs[ofs+2]+
					ovs[ofs+3]+
					ovs[ofs+4]+
					ovs[ofs+5]+
					ovs[ofs+6]+
					ovs[ofs+7] ;
			r/=(float)OVERSAMPLE;
			
			dst[i]=r;
		
	}		*/
	}
	
}


DistortionPlugin::DistortionPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	
	pre_gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "pre_gain","Pre Gain","dB");
	
	std::vector<String> type_list;
	type_list.push_back("Clip");
	type_list.push_back("Atan");
	type_list.push_back("Decimate");
	type_list.push_back("Overdrive");
	type_list.push_back("Waveshape 1");
	type.set_all( "type","Type",type_list );
	
	pre_lpf.set_all( 1000, 0.1, 12000, 1000, 0.1, Property::DISPLAY_SLIDER, "pre_lpf","Pre-LPF","hz");
	pre_lpf.set_quad_coeff( true );
	
	drive.set_all( 0, 0, 1, 0, 0.01, Property::DISPLAY_SLIDER, "drive","Drive");
	
	
	property_list.push_back(&pre_gain);
	property_list.push_back(&type);
	property_list.push_back(&pre_lpf);
	property_list.push_back(&drive);
	
	pre= new float[p_channels];
	h= new float[p_channels];
	for (int i=0;i<p_channels;i++) {
		pre[i]=0;
		h[i]=0;
	}
	
	buff.setup( p_channels, MAX_LEN*OVERSAMPLE );
	mix_rate=44100;
}


DistortionPlugin::~DistortionPlugin() { 
	
	delete input_plug;
	delete output_plug;
	delete[] pre;
	delete[] h;
	
}


}
