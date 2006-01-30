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


Automation::AutomationBlock* Loader::get_shared_automation_block_idx(int p_idx) {
	
	std::map<int,Automation::AutomationBlock*>::iterator I=shared_automation_blocks.find(p_idx);
		
	if (I==shared_automation_blocks.end())
		return NULL;
	
	return I->second;

}


void Loader::load_graph(AudioGraph *p_graph,const std::vector<int> &p_node_remap,TreeLoader *p_loader) {
	
	
}
void Loader::load_track_rack(Track *p_track,TreeLoader *p_loader) {
	
	
	
}


void Loader::load_track(Track *p_track, TreeLoader *p_loader) {
	
	

	/** LOAD STANDARD TRACK INFO */
	p_track->set_name( p_loader->get_string( "name "));

	/** Track Type specific */

	if (dynamic_cast<Track_Pattern*>(p_track)) 
		load_track_pattern(dynamic_cast<Track_Pattern*>(p_track),p_loader);
	
	/** LOAD RACK */
	p_loader->enter("rack");
	load_track_rack(p_track,p_loader);
	p_loader->exit(); //rack


	//for loading automations, everything else bust have been loaded, so, loading them now
		
	/** AUTOMATIONS */
	p_loader->enter("automations");

	for (int i=0;i<p_loader->get_child_count();i++) {
		
		p_loader->enter(p_loader->get_child_name(i));
		
		
		String name=p_loader->get_int("name");
		int prop_idx=-1;
		if (p_loader->get_int("builtin")) { //load builtin property
			
			for (int j=0;j<p_track->get_property_count();j++) {
				
				if (p_track->get_property(j)->get_name()==name && p_track->is_property_builtin(j)) {
					
					prop_idx=j;
					break;
				}
			
			}
			
		} else { //loading plugin property
			
			SoundPlugin *p=p_track->get_plugin(p_loader->get_int("plugin_idx"));
			
			if (p) {
				
				Property *pr=NULL;
				for (int j=0;j<p->get_port_count();j++) {
					
					if (p->get_port(j).get_name()==name) {
						
						pr=&p->get_port(j);
						break;
					}
					
				}
				
				if (pr) {
					
					for (int j=0;j<p_track->get_property_count();j++) {
						
						if (p_track->get_property(j)==pr) {
							
							prop_idx=j;
							break;
						}
					}
				}
			}
		}
		
		if (prop_idx==-1) {
			
			p_loader->exit();
			ERR_CONTINUE(prop_idx==-1);
		}
		
		Automation *a=p_track->get_property_automation(prop_idx);
		/* Dont save unused automations, to save space in file */
		a->set_initial_value( p_loader->get_int("initial") );
		{
			p_loader->enter("blocks");
		
			for (int j=0;j<p_loader->get_child_count();j++) {
		
				p_loader->enter(p_loader->get_child_name(j));
			
				int block_idx=p_loader->get_int("index");
				Tick pos=p_loader->get_int("pos");
				Automation::AutomationBlock *ab=get_shared_automation_block_idx(block_idx);
				if (ab==NULL) {
					
					p_loader->exit();
					ERR_CONTINUE(ab==NULL);
				}
				
				ab=dynamic_cast<Automation::AutomationBlock*>(a->create_link_block( ab )); //make it a link block
				if (ab==NULL) {
					
					p_loader->exit();
					ERR_CONTINUE(ab==NULL);
				}
				
				a->insert_block(ab,pos);
				
				p_loader->exit(); //block_idx
				
			}
			p_loader->exit(); //blocks
						  
		}
		
		p_loader->exit(); //automation_idx
	
	}
	p_loader->exit(); //automations
	
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
		
		int channels=p_loader->get_int( "channels " );
		Track *track=NULL;
		if (p_loader->get_string("type")=="pattern") {
			
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
