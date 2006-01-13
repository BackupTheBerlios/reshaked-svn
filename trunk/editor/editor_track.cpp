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

void Editor::add_plugin_to_track(Track *p_track,SoundPlugin *p_plugin) {
	
	
	
	
	Track::PluginInsertData data;
	data.plugin=p_plugin;
	data.pos=-1;
	
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
	d->undo_stream.end();
	
}
void Editor::remove_plugin_from_track(Track *p_track,int p_which) {
	
	ERR_FAIL_INDEX(p_which,p_track->get_plugin_count());
	
	d->undo_stream.begin("Remove Plugin");
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_plugin_remove,p_track,p_which));
	d->undo_stream.end();
	
}



void Editor::set_track_name(int p_track,String p_name) {
	
	Track *t=d->song->get_track(p_track);
	ERR_FAIL_COND(t==NULL);
	t->set_name( p_name );
	d->ui_update_notify->track_names_changed();
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
	
}



void Editor::move_track_left(int p_which) {
	
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	if (p_which==0)
		return;
	
	d->undo_stream.begin("Track Move Left");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::track_move_left,p_which));
	d->undo_stream.end();
	
}
void Editor::move_track_right(int p_which) {
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	
	if (p_which==(d->song->get_track_count()-1))
		return;
	
	d->undo_stream.begin("Track Move Right");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::track_move_right,p_which));
	d->undo_stream.end();
	
	
}


AudioGraph::ConnectError Editor::connection_create(AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to) {
	
	d->undo_stream.begin("Connect Plugs");
	d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_create,p_graph,p_node_from,p_plug_from,p_node_to,p_plug_to));
	d->undo_stream.end();
	
	return p_graph->get_last_conect_error();
}
void Editor::connection_erase(AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to) {
	
	d->undo_stream.begin("Disconnect Plugs");
	d->undo_stream.add_command(Command5(&commands,&EditorCommands::connection_erase,p_graph,p_node_from,p_plug_from,p_node_to,p_plug_to));
	d->undo_stream.end();
	
	
}

void Editor::property_changed(Property * p_property,float p_old_value,Track *p_hint_track) {
	
	printf("%lls changed from %g to %g\n",p_property->get_caption().c_str(),p_old_value,p_property->get());
	
}

} //end of namespace
