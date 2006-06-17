//
// C++ Implementation: editor_track
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "editor.h"


namespace ReShaked {


void Editor::track_move_plugin_left(Track *p_track,int p_plugin) {
	
	if (p_plugin==0)
		return;
	d->undo_stream.begin("Move Plugin Left",true);
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_plugin_move_left,p_track,p_plugin));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}
void Editor::track_move_plugin_right(Track *p_track,int p_plugin) {
	
	if ((p_plugin+1)>=p_track->get_plugin_count())
		return;
	d->undo_stream.begin("Move Plugin Right",true);
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_plugin_move_right,p_track,p_plugin));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}


void Editor::add_plugin_to_track(Track *p_track,SoundPlugin *p_plugin,bool p_append_to_end) {
	
	
	
	
	Track::PluginInsertData data;
	data.plugin=p_plugin;
	if (p_plugin->get_info()->is_synth)
		data.pos=0; // if this is a synth, insert at pos 0 
	else
		data.pos=-1; //insert at the end
	
	
	/* For plugins, the automations must be created here, otherwise, no undo/redo is possible */
	
	for (int i=0;i<p_plugin->get_port_count();i++) {
		
		if (p_plugin->get_port_type(i)==SoundPlugin::TYPE_READ)
			continue;
		
		Track::PluginInsertData::AutomationTrack atrack;
		atrack.pos=-1; //insert at end
		atrack.automation = new Track::TrackAutomation(&p_plugin->get_port(i));
		data.automated_tracks.push_back(atrack);
	}
	
	d->undo_stream.begin("Add Plugin - " + p_plugin->get_info()->caption);
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_plugin_add,p_track,data));
	
	/* autoconnect! */
	if (p_append_to_end) {
		AudioGraph *plugin_graph=&p_track->get_plugin_graph();
		AudioNode *playback_node=p_track->get_playback_node();
		int plugin_idx=plugin_graph->get_node_index( p_plugin );
		int playback_idx=plugin_graph->get_node_index( playback_node );
		
		
		if (p_plugin->get_info()->is_synth  && p_plugin->get_output_plug_count()>0) {
		
			if (p_plugin->get_output_plug(0)->get_channels()==playback_node->get_input_plug(0)->get_channels() )
				d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_create,plugin_graph,plugin_idx,0,playback_idx,0));
		} else {
			
			std::list<AudioGraph::Connection> connections_to_erase;
			std::list<AudioGraph::Connection> connections_to_make;
			for (int i=0;i<plugin_graph->get_connection_count();i++) {
				
				AudioGraph::Connection connection=*plugin_graph->get_connection( i );
				
				if (connection.node_to==playback_idx) {
					connections_to_erase.push_back(connection);
					AudioGraph::Connection to_replace=connection;
					to_replace.node_to=plugin_idx;
					connections_to_make.push_back(to_replace);
				}
							
			}
			
			foreach(I,connections_to_erase) {
				
				d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_erase,plugin_graph,I->node_from,I->plug_from,I->node_to,I->plug_to));
				
			}
			
			foreach(I,connections_to_make) {
				
				d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_create,plugin_graph,I->node_from,I->plug_from,I->node_to,I->plug_to));
				
			}
			
			d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_create,plugin_graph,plugin_idx,0,playback_idx,0));
			
		}
	}	
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}
void Editor::remove_plugin_from_track(Track *p_track,int p_which) {
	
	ERR_FAIL_INDEX(p_which,p_track->get_plugin_count());
	
	d->undo_stream.begin("Remove Plugin");
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_plugin_remove,p_track,p_which));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}



void Editor::set_track_name(int p_track,String p_name) {
	
	Track *t=d->song->get_track(p_track);
	ERR_FAIL_COND(t==NULL);
	t->set_name( p_name );
	d->ui_update_notify->track_names_changed();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
}

void Editor::add_track(TrackType p_type,int p_channels,String p_name) {

	
	Track *track;
	switch (p_type) {
		
		case TRACK_TYPE_PATTERN: {
			
			
			track =  new Track_Pattern(p_channels,&d->song->get_global_properties(),&d->song->get_song_playback());
			track->set_name(p_name);

		} break;
	}
	
	if (d->global_edit.current_blocklist<0)
		d->global_edit.current_blocklist=0;
	
	d->undo_stream.begin("Add Track");
	d->undo_stream.add_command(Command3(&commands,&EditorCommands::add_track,track,d->song->get_track_count(),(std::list<AudioGraph::Connection>*)NULL));
	int track_idx=d->song->get_track_graph().get_node_index( track );
	int out_idx=d->song->get_track_graph().get_node_index( d->song->get_output_node() );
	if (track_idx>=0 && out_idx>=0) {
		
		d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_create,&d->song->get_track_graph(),track_idx,0,out_idx,0));
		
	} else {
		ERR_PRINT("NO TRACK IDX or OUT IDX?! o_O");
	}
	
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	/* select the newly added track */
	for (int i=0;i<d->song->get_track_count();i++) {
		
		if (d->song->get_track(i)!=track)
			continue;
		int blocklist=get_track_blocklist( i );
		ERR_FAIL_COND(blocklist==-1);
		d->global_edit.current_blocklist=blocklist;
		enter_blocklist( EditorData::ENTER_LEFT );
	}
}

void Editor::remove_track(int p_which) {
	
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	
	d->undo_stream.begin("Remove Track");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::remove_track,p_which));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}



void Editor::move_track_left(int p_which) {
	
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	if (p_which==0)
		return;
	
	d->undo_stream.begin("Track Move Left",true);
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::track_move_left,p_which));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}
void Editor::move_track_right(int p_which) {
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	
	if (p_which==(d->song->get_track_count()-1))
		return;
	
	d->undo_stream.begin("Track Move Right",true);
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::track_move_right,p_which));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}


AudioGraph::ConnectError Editor::connection_create(AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to) {
	
	d->undo_stream.begin("Connect Plugs");
	d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_create,p_graph,p_node_from,p_plug_from,p_node_to,p_plug_to));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	return p_graph->get_last_conect_error();
}
void Editor::connection_erase(AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to) {
	
	d->undo_stream.begin("Disconnect Plugs");
	d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_erase,p_graph,p_node_from,p_plug_from,p_node_to,p_plug_to));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}

void Editor::property_changed(Property * p_property,double p_old_value,Track *p_hint_track) {
	
//	printf("%lls changed from %g to %g\n",p_property->get_caption().c_str(),p_old_value,p_property->get());
	
	Automation *a=NULL;
	if (p_hint_track) {
		
		for (int i=0;i<p_hint_track->get_property_count();i++) {
			
			if (p_hint_track->get_property(i)==p_property) {
				
				a=p_hint_track->get_property_automation(i);
				break;
			}
		}
	}
	
	static Property *last_property=NULL;
	bool can_collapse=last_property==p_property;
	last_property=p_property;
	
	if (a) { //property belongs to an automation, so set initial value or record it
		
		if (d->song->get_song_playback().is_recording_automations() && d->song->get_song_playback().get_status()==SongPlayback::STATUS_PLAY) {
			/* Record automation */
		
			Tick tick=d->song->get_song_playback().get_playback_pos_tick();
			add_automation_point( a, tick, p_property->get_coeff_value(), 0, can_collapse );
			
		} else {
			if (p_property->get()==a->get_initial_value())
				return; //no point
			d->undo_stream.begin(p_property->get_caption() + " Changed",can_collapse);
			d->undo_stream.add_command(Command2(&commands,&EditorCommands::automation_initial_value_changed ,a,p_property->get()));
			d->undo_stream.end();
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		}		
	} else {
		
		if (p_property->get()==p_old_value)
			return; //no point
		d->undo_stream.begin("Property Changed",can_collapse);
		d->undo_stream.add_command(Command3(&commands,&EditorCommands::property_value_changed,p_property,p_old_value,p_property->get()));
		d->undo_stream.end();
		d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
	}
}

void Editor::set_plugin_skips_processing(SoundPlugin *p_plugin, bool p_skips) {
	
	d->undo_stream.begin(p_skips?(p_plugin->get_info()->caption+" Skips Processing"):(p_plugin->get_info()->caption+" Resumes Processing"));
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::plugin_set_skip,p_plugin,p_skips));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}

void Editor::set_plugin_preset_name(SoundPlugin *p_plugin,String p_name) {
	
	d->undo_stream.begin(p_plugin->get_info()->caption+" Renamed");
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::plugin_set_name,p_plugin,p_name));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}
void Editor::set_track_mute(Track *p_track,bool p_mute) {
	
	if (p_track->is_mute()==p_mute)
		return; //dont bother!
	
	d->undo_stream.begin(String(p_mute?"Mute ":"Unmute ")+p_track->get_name());
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_mute,p_track,p_mute));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}
void Editor::set_track_solo(int p_track_idx) {
	
	/* check if it's soloed */
	
	bool unsolo=true;
	
	for (int i=0;i<d->song->get_track_count();i++) {
			
		if ((i==p_track_idx && d->song->get_track(i)->is_mute()) || (i!=p_track_idx && !d->song->get_track(i)->is_mute()) ) {
			
			unsolo=false;
		}
	}
	begin_meta_undo_block(unsolo?"Un-Solo":"Solo");
	if (!unsolo) { //not unsoloing, then solo
		
		for (int i=0;i<d->song->get_track_count();i++) {
		
			set_track_mute(d->song->get_track(i),i!=p_track_idx);
		}
	} else {
		
		for (int i=0;i<d->song->get_track_count();i++) {
		
			set_track_mute(d->song->get_track(i),false);
		}
		
	}
	end_meta_undo_block();
}

void Editor::track_automation_move_left(Track *p_track,int p_property) {
	
	int curr=p_property-1;
	
	while (curr>=0) {
		
		if (p_track->has_property_visible_automation( curr ) )
			break;
		curr--;
	}
	
	if (curr<0)
		return; //couldnt find anything
	
	d->undo_stream.begin("Move Automation Left",true);
	d->undo_stream.add_command(Command3(&commands,&EditorCommands::track_swap_properties,p_track,p_property,curr));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	

}

void Editor::track_automation_move_right(Track *p_track,int p_property) {
	
	int curr=p_property+1;
	
	int max=p_track->get_property_count();
	
	while (curr<max) {
		
		if (p_track->has_property_visible_automation( curr ) )
			break;
		curr++;
	}
	
	if (curr>=max)
		return; //couldnt find anything
	
	d->undo_stream.begin("Move Automation Right",true);
	d->undo_stream.add_command(Command3(&commands,&EditorCommands::track_swap_properties,p_track,p_property,curr));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}
	

} //end of namespace
