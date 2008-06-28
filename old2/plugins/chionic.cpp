//
// C++ Implementation: chionic
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic.h"
//#include "icon_chionic_2.xpm"

#include "dsp/tree_helpers_dsp.h"

namespace ReShaked {


static SoundPlugin* create_chionic(const SoundPluginInfo *p_info,int p_channels) {
	
	/* Do not allow instancing at wrong channel numbers */
	if ( p_channels!=2 && p_channels !=4)
		return NULL;
	
	return new Chionic(p_info,p_channels);
}

const SoundPluginInfo *Chionic::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Chionic 2";
	info.description="Advanced Sampler and Additive Synthesizer";
	info.long_description="Chionic 2 is an advanced sampler and additive synthesizer. It's capable of multi layered voices and many kind of modulations between them, as well as modulating an audio source. It also supports many features intended for producing extremely realistic instruments";
	info.unique_ID="INTERNAL_chionic"; 
	info.category="Synthesizers"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=0; //(const char**)icon_chionic_2_xpm;
	info.creation_func=&create_chionic;
	info.version=1;
	
	return &info;
}


/* Plugs */	

int Chionic::get_input_plug_count()  {
	
	return 1;	
	
}
int Chionic::get_output_plug_count()  {
	
	return 1;
	
}
AudioPlug *Chionic::get_input_plug(int p_index)  {
	
	return input_plug;
	
}
AudioPlug *Chionic::get_output_plug(int p_index)  {
	
	return output_plug;	
	
}


/* Port Info */
int Chionic::get_port_count()  {
	
	return midi_synth->get_input_property_count()+midi_synth->get_output_property_count();

	
}
Property& Chionic::get_port(int p_index)  {
	
	if (p_index<midi_synth->get_input_property_count())
		return *midi_synth->get_input_property(p_index);
	else 
		return *midi_synth->get_output_property(p_index-midi_synth->get_input_property_count());
	
}

SoundPlugin::PortType Chionic::get_port_type(int p_index)  {
	
	if (p_index<midi_synth->get_input_property_count())
		return TYPE_WRITE;
	else 
		return TYPE_READ; 
	
}


/* Setting up */
void Chionic::set_mixing_rate(float p_mixing_rate)  {
	
	midi_synth->set_mix_rate( p_mixing_rate );
	
}

void Chionic::reset() {
	
	midi_synth->reset();	
}

/* Processing */
void Chionic::process(int p_frames)  {
	
	ERR_FAIL_COND(p_frames>INTERNAL_BUFFER_SIZE);
	
	voice_buffers.input=input_plug->get_buffer();
	
	const EventBuffer *ebuff=get_event_buffer();
	
	for (int i=0;i<ebuff->get_event_count();i++) {
		
		const Event *e=ebuff->get_event(i);
				
		if (e->type==Event::TYPE_MIDI) {
			
			const EventMidi &em=e->param.midi;
			switch (em.midi_type) {
				
				case EventMidi::MIDI_NOTE_ON: {
					
					
					midi_synth->control_note_on( em.data.note.note, em.data.note.velocity );
					/*
					printf("NOTE ON %i,%i\n",em.data.note.note,em.data.note.velocity);
					ofs=0;
					freq=440.0*powf(2.0,((float)em.data.note.note-69.0)/12.0);
					amp=DEFAULT_AMP*(float)em.data.note.velocity/127.0;
					float fade=release.get();
					float frames_for_fade=(float)mixing_rate*fade;
					amp_decr=amp/frames_for_fade;
					sust=true;*/
									
						
				} break;
				case EventMidi::MIDI_NOTE_OFF: {
					
					midi_synth->control_note_off( em.data.note.note );
					/*					
					printf("NOTE OFF %i\n",em.data.note.note);
					
					sust=false;
					
					*/
				} break;
				default: {} //dont annoy, gcc
			}
		}
	}
	
	
	
	output_plug->get_buffer()->clear( p_frames );
	
	for (int i=0;i<output_plug->get_buffer()->get_channels();i++) {
		
		midi_synth->set_buffer( i, output_plug->get_buffer()->get_buffer( i ) );
	}
	
	midi_synth->process( p_frames);
	
	
}


void Chionic::save(TreeSaver *p_saver) {
	
	SoundPlugin::save(p_saver);
	
	
	/** SOURCES **/
	
	p_saver->enter("sources");
	for (int i=0;i<params.global.sources.size();i++) {
		
		ChionicParams::Source &s=params.global.sources[i];
		
		p_saver->enter("source_"+String::num(i+1));
		
		p_saver->add_string( "type" , s.type==ChionicParams::Source::TYPE_SAMPLE?"sample":"osc");
		p_saver->add_string( "name" , s.name );
		if (s.type==ChionicParams::Source::TYPE_SAMPLE) {
			
			p_saver->enter("sample");
			save_sample_to_tree(p_saver, s.sample );
			p_saver->exit();
			
		} else {
			
			p_saver->enter("osc");
			p_saver->add_float_array( "shape", s.osc->get_osc(0), Oscillator::BASE_SIZE );
			p_saver->exit();
			
		}		
		
		p_saver->exit();
	}
	
	p_saver->exit();
	
	/** GLOBAL **/
	
	p_saver->enter("global");
	
	ChionicParams::Global &g=params.global;
	
	p_saver->add_float( "volume_send", g.volume.send.get() );
	p_saver->add_float( "volume_sens", g.volume.sens.get() );
	p_saver->add_float( "pitch_coarse", g.pitch.tune_coarse.get() );
	p_saver->add_float( "pitch_fine", g.pitch.tune_fine.get() );
	p_saver->add_float( "pan_pos", g.pan.pos.get() );
	p_saver->add_float( "pan_depth", g.pan.depth.get() );
	p_saver->add_float( "pan_pitch_scale_center", g.pan.pitch_scale_center.get() );
	p_saver->add_float( "pan_pitch_scale", g.pan.pitch_scale.get() );
	
	{
		p_saver->enter("modulation_sources");
		for (int i=0;i<(ChionicParams::MAX_LAYERS+1);i++) {
			
			p_saver->enter("source_"+String::num(i+1));
			
			for (int j=0;j<g.modulation_source[i].read_from.size();j++) {
			
				p_saver->add_int("from_"+String(j+1),g.modulation_source[i].read_from[j]);
				
			}
			p_saver->exit();
		}
		p_saver->exit();
	}
	
	p_saver->exit();
	
	/** LAYERS **/
	
	p_saver->enter("layers");
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		p_saver->enter("layer_"+String::num(i+1));
		
		ChionicParams::Layer &l=params.layer[i];
		
		{ /** REGIONS **/
			
			p_saver->add_int("region_blending_enabled",l.regions.blending.enabled);
			p_saver->add_float("region_blending_curve",l.regions.blending.curve.get());
			
			p_saver->enter("regions");
			
			for (int j=0;j<l.regions.map.get_stream_size();j++) {
				
				p_saver->enter("region_"+String::num(j+1));
				p_saver->add_int("from_note",l.regions.map.get_index_pos(j));
				p_saver->add_int("base_note",l.regions.map.get_index_value(j).base_note);
				p_saver->add_int("source_idx",l.regions.map.get_index_value(j).source_idx);
				p_saver->exit();
				
			}
			p_saver->exit();
		}
		
		
		p_saver->add_float("volume_send",l.params.volume.send.get());
		p_saver->add_float("volume_velocity_range_end",l.params.volume.velocity_range_end.get());
		p_saver->add_float("volume_velocity_range_begin",l.params.volume.velocity_range_begin.get());
		p_saver->add_float("volume_velocity_range_blend",l.params.volume.velocity_range_blend.get());
		p_saver->add_float("volume_velocity_range_blend_expr",l.params.volume.velocity_range_blend_expr.get());
		p_saver->add_float("volume_velocity_random",l.params.volume.random.get());
		p_saver->add_float("volume_velocity_pitch_scale",l.params.volume.pitch_scale.get());
		
		{	
			p_saver->enter("volume_lfo");
			save_lfo_to_tree(p_saver,&l.params.volume.lfo);
			p_saver->exit();
		}
		{	
			p_saver->enter("volume_envelope");
			save_envelope_to_tree(p_saver,&l.params.volume.envelope);
			p_saver->exit();
		}
		
		
		p_saver->add_float("pan_pos",l.params.pan.pos.get());
		p_saver->add_float("pan_depth",l.params.pan.depth.get());
		p_saver->add_float("pan_random",l.params.pan.random.get());
		
		{	
			p_saver->enter("pan_lfo");
			save_lfo_to_tree(p_saver,&l.params.pan.lfo);
			p_saver->exit();
		}
		{	
			p_saver->enter("pan_depth_lfo");
			save_lfo_to_tree(p_saver,&l.params.pan.lfo_depth);
			p_saver->exit();
		}
		{	
			p_saver->enter("pan_envelope");
			save_envelope_to_tree(p_saver,&l.params.pan.envelope);
			p_saver->exit();
		}
		{	
			p_saver->enter("pan_depth_envelope");
			save_envelope_to_tree(p_saver,&l.params.pan.envelope_depth);
			p_saver->exit();
		}
		
		p_saver->add_float("pitch_coarse",l.params.pitch.tune_coarse.get());
		p_saver->add_float("pitch_fine",l.params.pitch.tune_fine.get());
		
		{	
			p_saver->enter("pitch_lfo");
			save_lfo_to_tree(p_saver,&l.params.pitch.lfo);
			p_saver->exit();
		}
		{	
			p_saver->enter("pitch_envelope");
			save_envelope_to_tree(p_saver,&l.params.pitch.envelope);
			p_saver->exit();
		}
		
		p_saver->add_int("filter_type",l.params.filter.type.get_current());
		p_saver->add_float("filter_cutoff",l.params.filter.cutoff.get());
		p_saver->add_float("filter_resonance",l.params.filter.resonance.get());
		p_saver->add_float("filter_cutoff_velsens",l.params.filter.cutoff_velsens.get());
		p_saver->add_float("filter_resonance_velsens",l.params.filter.resonance_velsens.get());
		p_saver->add_float("filter_freq_tracking",l.params.filter.freq_tracking.get());
		p_saver->add_float("filter_random",l.params.filter.random.get());
		{	
			p_saver->enter("filter_cutoff_lfo");
			save_lfo_to_tree(p_saver,&l.params.filter.lfo_cutoff);
			p_saver->exit();
		}
		{	
			p_saver->enter("filter_resonance_lfo");
			save_lfo_to_tree(p_saver,&l.params.filter.lfo_resonance);
			p_saver->exit();
		}
		{	
			p_saver->enter("filter_cutoff_envelope");
			save_envelope_to_tree(p_saver,&l.params.filter.envelope_cutoff);
			p_saver->exit();
		}
		{	
			p_saver->enter("filter_resonance_envelope");
			save_envelope_to_tree(p_saver,&l.params.filter.envelope_resonance);
			p_saver->exit();
		}
		
		p_saver->add_float("time_delay",l.params.time.delay.get());
		p_saver->add_float("time_random",l.params.time.random.get());
		
		p_saver->add_int("modulation_mode",l.modulation_mode);
		p_saver->exit();
		
		
	}
	p_saver->exit();
	
}
void Chionic::load(TreeLoader *p_loader) {
	
	
	SoundPlugin::load(p_loader);
	
	
	/** SOURCES **/
	
	p_loader->enter("sources");
	
	params.clear_sources();
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		
		p_loader->enter(p_loader->get_child_name(i));
		
		ChionicParams::Source s;
		if ( p_loader->get_string("type")=="sample" ) { //is sample!
			
			s.sample = new Sample;
			
			p_loader->enter("sample");
			load_sample_from_tree(p_loader, s.sample );
			p_loader->exit();
			
			
		} else { //is osc!
			
			p_loader->enter("osc");
			if (p_loader->get_float_array_len( "shape" )==Oscillator::BASE_SIZE) {
				
				p_loader->get_float_array("shape",s.osc->get_osc_w());
				s.osc->update_submaps();
				
			} else {
				
				ERR_PRINT("Osc sample mismatch!");
			}
			p_loader->exit();
			
		}
		
		s.name=p_loader->get_string("name");
		
		params.global.sources.push_back(s);
		p_loader->exit();
	}
	
	p_loader->exit();
	
	/** GLOBAL **/
	
	p_loader->enter("global");
	
	ChionicParams::Global &g=params.global;
	
	g.volume.send.set( p_loader->get_float( "volume_send") );
	g.volume.sens.set(p_loader->get_float( "volume_sens") );
	g.pitch.tune_coarse.set( p_loader->get_float( "pitch_coarse" ));
	g.pitch.tune_fine.set(p_loader->get_float( "pitch_fine") );
	g.pan.pos.set(p_loader->get_float( "pan_pos") );
	g.pan.depth.set( p_loader->get_float( "pan_depth") );
	g.pan.pitch_scale_center.set( p_loader->get_float( "pan_pitch_scale_center") );
	g.pan.pitch_scale.set( p_loader->get_float( "pan_pitch_scale") );
	
	{
		p_loader->enter("modulation_sources");
		

		for (int i=0;i<p_loader->get_child_count();i++) {
			
			p_loader->enter(p_loader->get_child_name(i));
			
			ERR_CONTINUE(i>=(ChionicParams::MAX_LAYERS+1));
				
			g.modulation_source[i].read_from.clear();
			
			
			for (int j=0;j<p_loader->get_var_count();j++) {
			
				g.modulation_source[i].read_from.push_back( p_loader->get_int( p_loader->get_var_name(j) ) );
				
			}
			p_loader->exit();
		}
		p_loader->exit();
	}
	
	p_loader->exit();
	
	/** LAYERS **/
	
	p_loader->enter("layers");
	
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		ERR_CONTINUE( i>=ChionicParams::MAX_LAYERS );
		
		p_loader->enter( p_loader->get_child_name(i) );
		
		ChionicParams::Layer &l=params.layer[i];
		
		{ /** REGIONS **/
			
			l.regions.blending.enabled=p_loader->get_int("region_blending_enabled");
			l.regions.blending.curve.set(p_loader->get_float("region_blending_curve"));
			
			p_loader->enter("regions");
			
			l.regions.map.clear();
			
			for (int j=0;j<p_loader->get_child_count();j++) {
				
				p_loader->enter(p_loader->get_child_name(j));
				
				int from_note=p_loader->get_int("from_note");
				int base_note=p_loader->get_int("base_note");
				int source_idx=p_loader->get_int("source_idx");
				
				Region r;
				r.base_note=base_note;
				r.source_idx=source_idx;
				
				l.regions.map.insert(from_note,r);
						
				p_loader->exit();
				
			}
			p_loader->exit();
		}
		
		
		l.params.volume.send.set(p_loader->get_float("volume_send"));
		l.params.volume.velocity_range_end.set(p_loader->get_float("volume_velocity_range_end"));
		l.params.volume.velocity_range_begin.set(p_loader->get_float("volume_velocity_range_begin"));
		l.params.volume.velocity_range_blend.set(p_loader->get_float("volume_velocity_range_blend"));
		l.params.volume.velocity_range_blend_expr.set(p_loader->get_float("volume_velocity_range_blend_expr"));
		l.params.volume.random.set(p_loader->get_float("volume_velocity_random"));
		l.params.volume.pitch_scale.set(p_loader->get_float("volume_velocity_pitch_scale"));
		
		{	
			p_loader->enter("volume_lfo");
			load_lfo_from_tree(p_loader,&l.params.volume.lfo);
			p_loader->exit();
		}
		{	
			p_loader->enter("volume_envelope");
			load_envelope_from_tree(p_loader,&l.params.volume.envelope);
			p_loader->exit();
		}
		
		
		l.params.pan.pos.set(p_loader->get_float("pan_pos"));
		l.params.pan.depth.set(p_loader->get_float("pan_depth"));
		l.params.pan.random.set(p_loader->get_float("pan_random"));
		
		{	
			p_loader->enter("pan_lfo");
			load_lfo_from_tree(p_loader,&l.params.pan.lfo);
			p_loader->exit();
		}
		{	
			p_loader->enter("pan_depth_lfo");
			load_lfo_from_tree(p_loader,&l.params.pan.lfo_depth);
			p_loader->exit();
		}
		{	
			p_loader->enter("pan_envelope");
			load_envelope_from_tree(p_loader,&l.params.pan.envelope);
			p_loader->exit();
		}
		{	
			p_loader->enter("pan_depth_envelope");
			load_envelope_from_tree(p_loader,&l.params.pan.envelope_depth);
			p_loader->exit();
		}
		
		l.params.pitch.tune_coarse.set(p_loader->get_float("pitch_coarse"));
		l.params.pitch.tune_fine.set(p_loader->get_float("pitch_fine"));
		
		{	
			p_loader->enter("pitch_lfo");
			load_lfo_from_tree(p_loader,&l.params.pitch.lfo);
			p_loader->exit();
		}
		{	
			p_loader->enter("pitch_envelope");
			load_envelope_from_tree(p_loader,&l.params.pitch.envelope);
			p_loader->exit();
		}
		
		l.params.filter.type.set(p_loader->get_int("filter_type"));
		l.params.filter.cutoff.set(p_loader->get_float("filter_cutoff"));
		l.params.filter.resonance.set(p_loader->get_float("filter_resonance"));
		l.params.filter.cutoff_velsens.set(p_loader->get_float("filter_cutoff_velsens"));
		l.params.filter.resonance_velsens.set(p_loader->get_float("filter_resonance_velsens"));
		l.params.filter.freq_tracking.set(p_loader->get_float("filter_freq_tracking"));
		l.params.filter.random.set(p_loader->get_float("filter_random"));
		{	
			p_loader->enter("filter_cutoff_lfo");
			load_lfo_from_tree(p_loader,&l.params.filter.lfo_cutoff);
			p_loader->exit();
		}
		{	
			p_loader->enter("filter_resonance_lfo");
			load_lfo_from_tree(p_loader,&l.params.filter.lfo_resonance);
			p_loader->exit();
		}
		{	
			p_loader->enter("filter_cutoff_envelope");
			load_envelope_from_tree(p_loader,&l.params.filter.envelope_cutoff);
			p_loader->exit();
		}
		{	
			p_loader->enter("filter_resonance_envelope");
			load_envelope_from_tree(p_loader,&l.params.filter.envelope_resonance);
			p_loader->exit();
		}
		
		l.params.time.delay.set(p_loader->get_float("time_delay"));
		l.params.time.random.set(p_loader->get_float("time_random"));
		
		switch( p_loader->get_int("modulation_mode") ) {
			
			case ChionicParams::Layer::MODE_ON_ADD: l.modulation_mode=ChionicParams::Layer::MODE_ON_ADD; break;
			case ChionicParams::Layer::MODE_RING: l.modulation_mode=ChionicParams::Layer::MODE_RING; break;
			case ChionicParams::Layer::MODE_FM: l.modulation_mode=ChionicParams::Layer::MODE_FM; break;
			default: l.modulation_mode=ChionicParams::Layer::MODE_OFF; break;
		}

		p_loader->exit();
		
		
	}
	p_loader->exit();
	
	preset_loaded=true;	
}

bool Chionic::check_if_preset_was_loaded() {
	
	bool loaded=preset_loaded;
	preset_loaded=false;
	return loaded;
	
}

ChionicParams* Chionic::get_params() {
	
	return &params;
}
Chionic::Chionic(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		voice_buffers.voice[i].buffer.setup(p_channels,INTERNAL_BUFFER_SIZE);
	}
		
	std::vector<MidiSynth::Voice*> voices;
	for (int i=0;i<32;i++) { //32 voices!
		
		voices.push_back( new ChionicVoice(&params,&voice_buffers) );
		
	}
	
	midi_synth = new MidiSynth(p_channels,voices);

	preset_loaded=false;
	
	
}
Chionic::~Chionic()  {
	
	
	
}


}
