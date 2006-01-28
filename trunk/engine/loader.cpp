//
// C++ Implementation: loader
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "loader.h"

namespace ReShaked {


void Loader::load_automation_block(TreeLoader *p_loader) {
	
	Automation::AutomationBlock *a = Automation::create_automation_block();
	
	a->set_length( p_loader->get_int("length" ) );
	
	/* MISSING LFO & INTERPOLATION!! */
	
	p_loader->enter("points");
	
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		float val=p_loader->get_float("val");
		Tick pos=p_loader->get_int("pos");
		float lfo_depth=p_loader->get_float("depth");
		
		
		a->get_data()->insert( pos, Automation::AutomationValue(val,lfo_depth) );
		
		p_loader->exit(); //p_?	
			
		
	}
	p_loader->exit(); //points
	
	int index=p_loader->get_int("index");
	
	shared_automation_blocks[index]=a;
		
}

void Loader::load_pattern_block(TreeLoader *p_loader) {
	
	Track_Pattern::PatternBlock *p = Track_Pattern::create_pattern_block();
	
	p->set_length( p_loader->get_int("length" ) );
	

	p_loader->enter("notes");
	
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		int note=p_loader->get_int("note");
		int volume=p_loader->get_int("vol");
		int column=p_loader->get_int("column");
		Tick pos=p_loader->get_int("pos");
		
		p->get_pattern()->data.insert( Track_Pattern::Position(pos,column), Track_Pattern::Note(note,volume) );
		
		p_loader->exit(); //p_?	
			
	}
	p_loader->exit(); //points
	
	int index=p_loader->get_int("index");
	
	shared_pattern_blocks[index]=p;
	
}



void Loader::load_graph(AudioGraph *p_graph,const std::vector<int> &p_node_remap,TreeLoader *p_loader) {
	
	
}
void Loader::load_track_rack(Track *p_track,TreeLoader *p_loader) {
	
	
	
}


void Loader::load_track(Track *p_track, TreeLoader *p_loader) {
	
	

	/** SAVE STANDARD TRACK INFO */
	p_track->set_name( p_saver->get_string( "name "));

	/** SAVE RACK */
	p_saver->enter("rack");
	load_track_rack(p_track,p_saver);
	p_saver->exit(); //rack

	/** AUTOMATIONS */
	p_saver->enter("automations");

	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		/* FIX AUTOMATIONS! Which property is EACH FOR? , ORDER?! BLEHLER! XOX
		Automation *a=p_track->get_property_automation(i);
		/// Dont save unused automations, to save space in file 
		if ( a->get_block_count()==0 && !p_track->has_property_visible_automation(i))
			continue; 
	
		p_saver->enter("automation_"+String::num(i));
	
		p_saver->add_int("initial",a->get_initial_value());
		{
			p_saver->enter("blocks");
	
			for (int j=0;j<a->get_block_count();j++) {
						
				p_saver->enter("block_"+String::num(j));
				int block_idx=index_automation_block( a->get_block(j) );
				p_saver->add_int("index",block_idx);
				p_saver->add_int("pos",a->get_block_pos(j));
				p_saver->exit(); //block_idx
			
			}
			p_saver->exit(); //blocks
						
		}
		*/	
		p_saver->exit(); //automation_idx
	
	
	}
	p_saver->exit(); //automations

	/** Track Type specific */

	if (dynamic_cast<Track_Pattern*>(p_track)) 
		save_track_pattern(dynamic_cast<Track_Pattern*>(p_track),p_saver);


	
}


void Loader::load_song(Song *p_song,TreeLoader *p_loader) {
	
	/* Stop the song, so properties are restored to their initial values 
	If the song isnt stopped, all properties/controls/etc wouldnt be in their proper value */
	p_song->stop(); 
	shared_automation_blocks.clear();
	shared_pattern_blocks.clear();
	
	
	/** LOAD GLOBAL PROPERTIES */
	
	p_loader->enter("global_properties");
	p_song->get_global_properties().get_swing().set(p_loader->get_float("swing"));
	p_song->get_global_properties().get_tempo().set(p_loader->get_float("tempo"));
	p_song->get_global_properties().get_volume().set(p_loader->get_float("volume"));
	p_loader->exit();
		
	/** LOAD BAR MAP PROPERTIES */
	
	p_loader->enter("bar_map");
	
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		
		p_loader->enter(p_loader->get_child_name(i));
		
		int bar_beat=p_loader->get_int("beat");
		int bar_len=p_loader->get_int("len");
		
		
		p_song->get_bar_map().insert_bar_len_at_beat( bar_beat, bar_len );
		
		p_loader->exit(); 
	}
	
	p_loader->exit(); // bar_map
	
	/** LOAD MARKERS */
	
	p_loader->enter("markers");
	
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		int marker_beat=p_loader->get_int("beat");
		String marker_text=p_loader->get_string("text");
		
		p_song->get_marker_list().insert( marker_beat, marker_text);
		
		p_loader->exit(); 
	}
	
	p_loader->exit(); //markers
	
	/** LOAD AUDIO PORTS CONFIG */
	
	p_loader->enter("audio_ports");
	{
		
		{
			p_loader->enter("input");
			
			p_song->get_audio_port_layout().port_in_info.clear();
					
			for (int i=0;i<p_loader->get_var_count();i++) {
			
				p_song->get_audio_port_layout().port_in_info.push_back(p_loader->get_int(p_loader->get_var_name(i)));
			}
			
			p_loader->exit();
		}
				
		{
			p_loader->enter("output");
			
			p_song->get_audio_port_layout().port_out_info.clear();
			
			for (int i=0;i<p_loader->get_var_count();i++) {
			
				p_song->get_audio_port_layout().port_out_info.push_back(p_loader->get_int(p_loader->get_var_name(i)));
			}
			
			p_loader->exit();
		}
	}			

	p_loader->exit(); //audio_ports
				
	/** Load Automation and Pattern Blocks */

	p_loader->enter("automation_blocks"); //tracks
			
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		load_automation_block(p_loader);
		
		p_loader->exit(); //block_idx
	}
	
	p_loader->exit(); //automation_blocks
	
	p_loader->enter("pattern_blocks"); //tracks
			
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		load_pattern_block(p_loader);
		
		p_loader->exit(); //block_idx
	}
	
	p_loader->exit(); //automation_blocks
	
	
	/** LOAD TRACKS */
	
	p_loader->enter("tracks"); //tracks
	
	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		int channels=p_saver->get_int( "channels " );
		Track *track=NULL;
		if (p_saver->get_string("type")=="pattern") {
			
			track = new Track_Pattern(channels,&p_song->get_global_properties(),&p_song->get_song_playback());
		}

		load_track(track,p_loader);		
		
		p_loader->exit();
		
	}
	p_loader->exit(); //tracks
	
	
	/** LOAD TRACK GRAPH */
	
	{
		
		std::vector<int> node_remap;
		
		node_remap.push_back( p_song->get_track_graph().get_node_index( p_song->get_input_node() ) );
		for (int i=0;i<p_song->get_track_count();i++)
			node_remap.push_back( p_song->get_track_graph().get_node_index( p_song->get_track(i) ) );
		node_remap.push_back( p_song->get_track_graph().get_node_index( p_song->get_output_node() ) );
		
		p_loader->enter("track_graph");
		load_graph( &p_song->get_track_graph(), node_remap, p_loader);
		p_loader->exit();
	
	}
	
	
}


Loader::Loader()
{
}


Loader::~Loader()
{
}


}
