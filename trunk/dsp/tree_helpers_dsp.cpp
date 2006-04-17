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
namespace ReShaked {

bool load_sample_from_tree(TreeLoader *p_tree,Sample *p_sample) {
	
	
	int len=p_tree->get_int("length");
	int chans=p_tree->get_int("channels");
	
	printf("sample has %i len, %i channels\n",len,chans);
	
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


}
