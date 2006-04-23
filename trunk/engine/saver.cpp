//
// C++ Implementation: saver
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "saver.h"
#include "version.h"

namespace ReShaked {


void Saver::save_graph(AudioGraph *p_graph,const std::vector<int> &p_node_remap,TreeSaver *p_saver) {

	
	ERR_FAIL_COND(p_node_remap.size()!=p_graph->get_node_count());

	for (int i=0;i<p_graph->get_connection_count();i++) {
		
		p_saver->enter("c_"+String::num(i));
		
		AudioGraph::Connection c=*p_graph->get_connection(i);
		p_saver->add_int("node_from",p_node_remap[c.node_from]);
		p_saver->add_int("plug_from",c.plug_from);
		p_saver->add_int("node_to",p_node_remap[c.node_to]);
		p_saver->add_int("plug_to",c.plug_to);
		
		p_saver->exit();
		
	}

	
	
}


void Saver::save_track_rack(Track *p_track,TreeSaver *p_saver) {
	
	
	/** PLUGIN GRAPH */
	
	
	p_saver->add_int("track_channels",p_track->get_channels());
	
	p_saver->enter("plugin_graph");
	std::vector<int> node_remap;
	
	
	for (int i=0;i<p_track->get_plugin_graph().get_node_count();i++) {
		
		AudioNode *n=p_track->get_plugin_graph().get_node(i);
		
		if (n==p_track->get_record_node())
			node_remap.push_back(0);
		else if (n==p_track->get_playback_node())
			node_remap.push_back(p_track->get_plugin_graph().get_node_count()-1);
		else {
			
			for (int j=0;j<p_track->get_plugin_count();j++) {
				
				if (n==p_track->get_plugin(j)) {
					node_remap.push_back(j+1);
					break;
				}
			}
			
		}
	}
		
	save_graph( &p_track->get_plugin_graph(), node_remap, p_saver );
	
	p_saver->exit(); //plugin_graph
	
	/** PLUGINS */
	
	p_saver->enter("plugins");
	
	for (int i=0;i<p_track->get_plugin_count();i++) {
		
		SoundPlugin *p=p_track->get_plugin(i);
		p_saver->enter("plugin_"+String::num(i));
		
		p_saver->add_string("ID",p->get_info()->unique_ID);
		p_saver->add_int("duplicate",p->get_duplicate());
		p_saver->add_int("skips_processing",p->skips_processing());
		if (p->get_info()->can_custom_channels)
			p_saver->add_int("channels_created",p->get_channels_created());
		else
			p_saver->add_int("channels_created",-1);
		
		{ //plugin data
			p_saver->enter("data");
			
			p->save(p_saver); //plugin handles its own saving
			
			p_saver->exit(); //data
		}
		p_saver->exit(); //plugin
		
	}
	
	p_saver->exit(); //plugins
	
	
}


int Saver::index_automation_block(Automation::AutomationBlock *p_block) {
	
	for (int i=0;i<shared_automation_blocks.size();i++) {
		
		if (p_block->get_data()==shared_automation_blocks[i]->get_data())
			return i;
		
	}
	
	/* evidently, we dont have the data for this one! */
	
	shared_automation_blocks.push_back( p_block );
	
	return shared_automation_blocks.size()-1;
}

int Saver::index_pattern_block(Track_Pattern::PatternBlock *p_block) {
	
	for (int i=0;i<shared_pattern_blocks.size();i++) {
		
		if (p_block->get_pattern()==shared_pattern_blocks[i]->get_pattern())
			return i;
		
	}
	
	/* evidently, we dont have the data for this one! */
	
	shared_pattern_blocks.push_back( p_block );
	
	return shared_pattern_blocks.size()-1;
}

void Saver::save_track_pattern(Track_Pattern *p_pattern_track,TreeSaver *p_saver) {
	
	p_saver->enter("track_pattern");
	
	
	p_saver->add_float( "swing", p_pattern_track->swing().get() );
	p_saver->add_float( "volume", p_pattern_track->volume().get() );
	
	{
		p_saver->enter("blocks");
		
	
		for (int i=0;i<p_pattern_track->get_block_count();i++) {
							
			p_saver->enter("block_"+String::num(i));
			int block_idx=index_pattern_block( p_pattern_track->get_block(i) );
			p_saver->add_int("index",block_idx);
			p_saver->add_int("pos",p_pattern_track->get_block_pos(i));
			p_saver->exit(); //block_idx
					
		}
		p_saver->exit(); //blocks
	}	
	
	p_saver->exit(); //track_pattern
	
}

void Saver::save_track(Track *p_track,TreeSaver *p_saver) {
	
	/** SAVE STANDARD TRACK INFO */
	p_saver->add_string( "name" , p_track->get_name() );
	p_saver->add_int( "channels ", p_track->get_channels() );
	p_saver->add_string("type",p_track->get_type_name());
	p_saver->add_int("mute",p_track->is_mute());
	
	/** SAVE RACK */
	p_saver->enter("rack");
	save_track_rack(p_track,p_saver);
	p_saver->exit(); //rack
	
	/** AUTOMATIONS */
	p_saver->enter("automations");
	
	for (int i=0;i<p_track->get_property_count();i++) {
		
		
		Automation *a=p_track->get_property_automation(i);
		/* Dont save unused automations, to save space in file */
		if ( a->get_block_count()==0 && !p_track->has_property_visible_automation(i))
			continue; 
		
		bool builtin=p_track->is_property_builtin(i);
		int plugin_idx=-1;
		
		if (!builtin) { //belongs to a plugin
			
			plugin_idx=p_track->find_plugin_idx_for_property(a->get_property());
			if (plugin_idx==-1) {
				
				printf("Failed to find plugin for property %s\n",a->get_property()->get_caption().ascii().get_data() );
			}
			ERR_CONTINUE(plugin_idx==-1);
		}
		
		
		
		p_saver->enter("automation_"+String::num(i));
		
		p_saver->add_int("property_index",i); //original index
		
		p_saver->add_int("display_size",a->get_display_size());
		p_saver->add_int("visible",p_track->has_property_visible_automation(i));
		p_saver->add_int("initial",a->get_initial_value());
		p_saver->add_int("builtin",builtin); //belongs to plugin or not
		p_saver->add_int("swing_follow",a->is_swing_follow_enabled());
		
		p_saver->add_string("name",a->get_property()->get_name());
		if (!builtin) 
			p_saver->add_int("plugin_idx",plugin_idx); //plugin it belongs to
	
		
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
		
		p_saver->exit(); //automation_idx
		
		
	}
	p_saver->exit(); //automations
	
	/** Track Type specific */
	
	if (dynamic_cast<Track_Pattern*>(p_track)) 
		save_track_pattern(dynamic_cast<Track_Pattern*>(p_track),p_saver);
	
}

void Saver::save_automation_block(Automation::AutomationBlock *p_block,TreeSaver *p_saver) {
	
	p_saver->add_int("length",p_block->get_length());
	
	/* LFO */
	p_saver->add_int("lfo_delay", p_block->get_data()->get_lfo().get_delay() );
	p_saver->add_int("lfo_mode", p_block->get_data()->get_lfo().get_mode() );
	p_saver->add_float("lfo_rate", p_block->get_data()->get_lfo().get_rate() );
	p_saver->add_float("lfo_depth", p_block->get_data()->get_lfo().get_depth() );
	p_saver->add_float("lfo_phase", p_block->get_data()->get_lfo().get_phase() );
	p_saver->add_float("lfo_random_depth", p_block->get_data()->get_lfo().get_random_depth() );
	p_saver->add_float("lfo_random_seed", p_block->get_data()->get_lfo().get_random_seed() );
	
	/* Interpolation */
	
	p_saver->add_int("interpolation_mode", p_block->get_data()->get_interpolation() );
	
	p_saver->enter("points");
	for (int i=0;i<p_block->get_data()->get_stream_size();i++) {
		
		float val=p_block->get_data()->get_index_value(i).value;
		Tick pos=p_block->get_data()->get_index_pos(i);
		float lfo_depth=p_block->get_data()->get_index_value(i).lfo_depth;
		
		p_saver->enter("p_"+String::num(i));
		p_saver->add_float("val",val);
		p_saver->add_float("depth",lfo_depth);
		p_saver->add_int("pos",pos);
		p_saver->exit(); //p_?	
			
		
	}
	p_saver->exit(); //points
	
}
void Saver::save_pattern_block(Track_Pattern::PatternBlock *p_block,TreeSaver *p_saver) {
	

	p_saver->add_int("length",p_block->get_length());
	

	p_saver->enter("notes");
	for (int i=0;i<p_block->get_note_count();i++) {
		
		int note=p_block->get_note(i).note;
		int volume=p_block->get_note(i).volume;
		int column=p_block->get_note_pos(i).column;
		Tick pos=p_block->get_note_pos(i).tick;
		
		p_saver->enter("n_"+String::num(i));
		p_saver->add_int("note",note);
		p_saver->add_int("vol",volume);
		p_saver->add_int("column",column);
		p_saver->add_int("pos",pos);
		p_saver->exit(); //p_?	
			
	}
	p_saver->exit(); //points

	
}


void Saver::save_song(Song *p_song,TreeSaver *p_saver) {
	
	/* Stop the song, so properties are restored to their initial values 
	If the song isnt stopped, all properties/controls/etc wouldnt be in their proper value */
	p_song->stop(); 
	shared_automation_blocks.clear();
	shared_pattern_blocks.clear();
	
	/** GLOBAL SONG INFO */
	
	p_saver->enter("info");
	
	p_saver->add_string("title",p_song->get_info_title());
	p_saver->add_string("author",p_song->get_info_author());
	p_saver->add_string("notes",p_song->get_info_notes());
	p_saver->add_string("version_saved",VERSION_STRING);
	
	p_saver->exit(); //info
	/** SAVE GLOBAL PROPERTIES */
	
	p_saver->enter("global_properties");
	p_saver->add_float("swing",p_song->get_global_properties().get_swing().get());
	p_saver->add_float("tempo",p_song->get_global_properties().get_tempo().get());
	p_saver->add_float("volume",p_song->get_global_properties().get_volume().get());
	p_saver->exit();
		
	/** SAVE BAR MAP PROPERTIES */
	
	p_saver->enter("bar_map");
	
	for (int i=0;i<p_song->get_bar_map().get_bar_idx_count();i++) {
		
		p_saver->enter("bar_"+String::num(i));
		
		int bar_beat=p_song->get_bar_map().get_bar_idx_pos(i);
		int bar_len=p_song->get_bar_map().get_bar_idx(i);
		
		p_saver->add_int("beat",bar_beat);
		p_saver->add_int("len",bar_len);
		
		p_saver->exit(); 
	}
	
	p_saver->exit(); // bar_map
	
	/** SAVE MARKERS */
	
	p_saver->enter("markers");
	
	for (int i=0;i<p_song->get_marker_list().get_stream_size();i++) {
		
		p_saver->enter("marker_"+String::num(i));
		
		int marker_beat=p_song->get_marker_list().get_index_pos(i);
		String marker_text=p_song->get_marker_list().get_index_value(i);
		
		p_saver->add_int("beat",marker_beat);
		p_saver->add_string("text",marker_text);
		
		p_saver->exit(); 
	}
	
	p_saver->exit(); //markers
	
	/** SAVE AUDIO PORTS CONFIG */
	
	p_saver->enter("audio_ports");
	{
		
		p_saver->enter("input");
		
		for (int i=0;i<p_song->get_audio_port_layout().port_in_info.size();i++) {
			
			p_saver->add_int("in_"+String::num(i),p_song->get_audio_port_layout().port_in_info[i]);
		}
		
		p_saver->exit();
		
		p_saver->enter("output");
		
		for (int i=0;i<p_song->get_audio_port_layout().port_out_info.size();i++) {
			
			p_saver->add_int("out_"+String::num(i),p_song->get_audio_port_layout().port_out_info[i]);
		}
		
		p_saver->exit();
	}			

	p_saver->exit(); //audio_ports
				
	
	/** SAVE TRACKS */
	
	p_saver->enter("tracks"); //tracks
	
	for (int i=0;i<p_song->get_track_count();i++) {
		
		p_saver->enter("track_"+String::num(i));
		
		save_track(p_song->get_track(i),p_saver);		
		
		p_saver->exit();
		
	}
	p_saver->exit(); //tracks
	
	
	/** SAVE TRACK GRAPH */
	
	{
		
		std::vector<int> node_remap;
		for (int i=0;i<p_song->get_track_graph().get_node_count();i++) {
			
			AudioNode *n=p_song->get_track_graph().get_node(i);
			if (n==p_song->get_input_node()) {
				
				node_remap.push_back(0);
			} else if (n==p_song->get_output_node()) {
				
				node_remap.push_back(p_song->get_track_graph().get_node_count()-1);
			} else {
				
				for (int j=0;j<p_song->get_track_count();j++) {
					
					if (p_song->get_track(j)==n) {
						
						node_remap.push_back(j+1);
						break;
						
					}
				}
				
			}
		
		}
		
		p_saver->enter("track_graph");
		save_graph( &p_song->get_track_graph(), node_remap, p_saver);
		p_saver->exit();
	
	}
	/** Save Automation and Pattern Blocks */

	p_saver->enter("automation_blocks"); //tracks
			
	for (int i=0;i<shared_automation_blocks.size();i++) {
		
		p_saver->enter("block_"+String::num(i)); //tracks
		
		p_saver->add_int("index",i);
		save_automation_block( shared_automation_blocks[i], p_saver);
		
		p_saver->exit(); //block_idx
	}
	
	p_saver->exit(); //automation_blocks
	
	p_saver->enter("pattern_blocks"); //tracks
			
	for (int i=0;i<shared_pattern_blocks.size();i++) {
		
		p_saver->enter("block_"+String::num(i)); //tracks
		
		p_saver->add_int("index",i);
		save_pattern_block( shared_pattern_blocks[i], p_saver);
		
		p_saver->exit(); //block_idx
	}
	
	p_saver->exit(); //pattern_blocks
	
}

Saver::Saver()
{
}


Saver::~Saver()
{
}


}
