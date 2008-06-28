//
// C++ Implementation: tree_helpers_dsp
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_helpers_dsp.h"
#include "sample.h"
#include "lfo.h"
#include "envelope.h"
namespace ReShaked {

bool load_sample_from_tree(TreeLoader *p_tree,Sample *p_sample) {
	
	
	int len=p_tree->get_int("length");
	int chans=p_tree->get_int("channels");
	
//	printf("sample has %i len, %i channels\n",len,chans);
	
	p_sample->create( chans, len );
	for (int i=0;i<chans;i++) {
		String buffer_str="buffer_"+String::num(i);
		if (p_tree->get_float_array_len( buffer_str )!=len)
			continue;
		p_tree->get_float_array(buffer_str,p_sample->get_buffer_w(i));
	}
	
	p_sample->set_base_freq( p_tree->get_float( "base_freq" ) );
	
	switch ( p_tree->get_int("loop_type") ) {
		
		case 0: p_sample->set_loop_type( Sample::LOOP_NONE ); break;
		case 1: p_sample->set_loop_type( Sample::LOOP_FORWARD ); break;
		case 2: p_sample->set_loop_type( Sample::LOOP_BIDI ); break;
	}
	
	p_sample->set_loop_begin( p_tree->get_int("loop_begin") );
	p_sample->set_loop_end( p_tree->get_int("loop_end") );
	
	return false;
}

void save_sample_to_tree(TreeSaver *p_tree,Sample *p_sample) {
	
	p_tree->add_string("format","raw" );
	

	p_tree->add_float("base_freq",p_sample->get_base_freq());
		
	p_tree->add_int("loop_type",p_sample->get_loop_type());
	p_tree->add_int("loop_begin",p_sample->get_loop_begin());
	p_tree->add_int("loop_end",p_sample->get_loop_end());
	
	p_tree->add_int("length",p_sample->get_length());
	
	p_tree->add_int("channels",p_sample->get_channels());
	
	for (int i=0;i<p_sample->get_channels();i++) {
		
		p_tree->add_float_array("buffer_"+String::num(i),p_sample->get_buffer(i),p_sample->get_length());
	}
	
}

bool load_lfo_from_tree(TreeLoader *p_tree,LFO *p_lfo) {
	
	p_lfo->set_delay( p_tree->get_float( "delay")  );	
	p_lfo->set_rate( p_tree->get_float( "rate" ) );	
	p_lfo->set_depth( p_tree->get_float( "depth" ) );	
	p_lfo->set_phase( p_tree->get_float( "phase" ) );		
	p_lfo->set_random_depth( p_tree->get_float( "random_depth") );	
	p_lfo->set_random_seed( p_tree->get_float( "random_seed") );	
	
	switch( p_tree->get_int( "mode") ) {
		
		case LFO::MODE_SINE: p_lfo->set_mode( LFO::MODE_SINE ); break;
		case LFO::MODE_SAW_UP: p_lfo->set_mode( LFO::MODE_SAW_UP ); break;
		case LFO::MODE_SAW_DOWN: p_lfo->set_mode( LFO::MODE_SAW_DOWN ); break;
		case LFO::MODE_SQUARE: p_lfo->set_mode( LFO::MODE_SQUARE ); break;
		
	}
	
	
}
void save_lfo_to_tree(TreeSaver *p_tree,LFO *p_lfo) {
	
	p_tree->add_float( "delay", p_lfo->get_delay() );	
	p_tree->add_float( "rate", p_lfo->get_rate() );	
	p_tree->add_float( "depth", p_lfo->get_depth() );	
	p_tree->add_float( "phase", p_lfo->get_phase() );		
	p_tree->add_float( "random_depth", p_lfo->get_random_depth() );	
	p_tree->add_float( "random_seed", p_lfo->get_random_seed() );	
	p_tree->add_int( "mode", p_lfo->get_mode() );	

	
	
}

bool save_envelope_to_tree(TreeSaver *p_tree,Envelope *p_envelope) {
	
	p_tree->add_int("enabled",p_envelope->is_enabled());
	p_tree->add_int("loop_enabled",p_envelope->is_loop_enabled());
	p_tree->add_int("loop_begin",p_envelope->get_loop_begin());
	p_tree->add_int("loop_end",p_envelope->get_loop_end());
	p_tree->add_int("sustain_loop_enabled",p_envelope->is_sustain_loop_enabled());
	p_tree->add_int("sustain_loop_begin",p_envelope->get_sustain_loop_begin());
	p_tree->add_int("sustain_loop_end",p_envelope->get_sustain_loop_end());
	p_tree->add_int("cubic",p_envelope->is_cubic());
	p_tree->enter("nodes");
	
	for (int i=0;i<p_envelope->get_node_count();i++) {
		
		p_tree->enter("node_"+String::num(i+1));
		p_tree->add_int("offset",p_envelope->get_node_offset(i) );
		p_tree->add_float("value",p_envelope->get_node_value(i) );
		p_tree->exit();
	}
	
	p_tree->exit();
	
}
void load_envelope_from_tree(TreeLoader *p_tree,Envelope *p_envelope) {
	
	p_envelope->reset(false);
	
	p_envelope->set_enabled(p_tree->get_int("enabled"));
	p_envelope->set_cubic(p_tree->get_int("cubic"));
	
	p_tree->enter("nodes");
	
	for (int i=0;i<p_tree->get_child_count();i++) {
		
		p_tree->enter(p_tree->get_child_name(i));
		
		int offset=p_tree->get_int("offset");
		float value=p_tree->get_float("value");
		
		p_envelope->add_node_at_offset( offset,value);
		
		p_tree->exit();
	}
	
	p_tree->exit();
	
	p_envelope->set_loop_enabled(p_tree->get_int("loop_enabled"));
	p_envelope->set_loop_begin(p_tree->get_int("loop_begin"));
	p_envelope->set_loop_end(p_tree->get_int("loop_end"));
	p_envelope->set_sustain_loop_enabled(p_tree->get_int("sustain_loop_enabled"));
	p_envelope->set_sustain_loop_begin(p_tree->get_int("sustain_loop_begin"));
	p_envelope->set_sustain_loop_end(p_tree->get_int("sustain_loop_end"));
	
	
}

}
