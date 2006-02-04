//
// C++ Implementation: editor_presets
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "editor.h"
#include "tree_container.h"
#include "tree_loader_disk.h"
#include "loader.h"

namespace ReShaked {






TreeLoaderDisk::ErrorReading Editor::load_plugin_preset(SoundPlugin *p_plugin,String p_filename,Track *p_track) {
	
	
	TreeLoaderDisk tld("RESHAKED_PLUGIN_"+p_plugin->get_info()->unique_ID,0,0);
	
	TreeLoaderDisk::ErrorReading error=tld.open_file( p_filename );
	if (error)
		return error;
	
	d->song->stop(); //stop song
	
	printf("LOADING FILE\n");
	TreeContainer *current = new TreeContainer;
	p_plugin->save( current );
	current->goto_root();
	
	printf("LOADING CONTAINER\n");
	
	TreeContainer *_new = new TreeContainer( &tld );
	_new->goto_root();
	
	tld.close_file();
	
	printf("LOAD OK, SETTING PRESET\n");
	
	d->undo_stream.begin("Load Preset for "+p_plugin->get_info()->caption,true);
	d->undo_stream.add_command(Command4(&commands,&EditorCommands::plugin_load_preset,p_plugin,(TreeLoader*)current,(TreeLoader*)_new,p_track));
	
	
	d->undo_stream.end();
	
	p_plugin->set_current_file(p_filename);

	
	return TreeLoaderDisk::ERROR_NONE;
}

TreeLoaderDisk::ErrorReading Editor::load_track_preset(Track *p_track,String p_filename) {
	
	TreeLoaderDisk tld("RESHAKED_TRACK_RACK",0,0);
	TreeLoaderDisk::ErrorReading error = tld.open_file( p_filename );
	
	if (error)
		return error;
	
	d->song->stop(); //stop song
	
	d->undo_stream.begin("Load Rack");
	
	/* Remove ALL plugins */
	while (p_track->get_plugin_count() )
		remove_plugin_from_track(p_track,0);
		
		
	/* Connections missing (probably between record and play */
	std::list<AudioGraph::Connection> pre_connections; 
	for (int i=0;i<p_track->get_plugin_graph().get_connection_count();i++) {
		
		pre_connections.push_back(*p_track->get_plugin_graph().get_connection(i));
	}
	
	foreach(I,pre_connections) {
		
		connection_erase( &p_track->get_plugin_graph(), I->node_from, I->plug_from, I->node_to,I->plug_to );
	}
	
	/* Load Rack (no undo) */
	
	Loader::load_track_rack( p_track, &tld );
	tld.close_file();
	
	/** Now remove and reinsert the data in the track rack, so it is undo/redo able */
	
	/* Connections */
	
	std::list<AudioGraph::Connection> connections; 
	
	while (p_track->get_plugin_graph().get_connection_count()) {
		
		AudioGraph::Connection c=*p_track->get_plugin_graph().get_connection(0);
		connections.push_back(c);
		p_track->get_plugin_graph().disconnect_plugs( c );
		
	}
	
	std::list<Track::PluginInsertData> plugin_recovery;
	
	while(p_track->get_plugin_count()) {
		

		Track::PluginInsertData recovery;
		p_track->remove_plugin( 0, &recovery );
		recovery.pos=-1; //so they keep initial order
		plugin_recovery.push_back(recovery);
		
	}
			
	foreach(I,plugin_recovery) {
		Track::PluginInsertData recovery=*I;
		d->undo_stream.add_command(Command2(&commands,&EditorCommands::track_plugin_add,p_track,recovery));
	}
	
	foreach(I,connections) {
		
		connection_create(&p_track->get_plugin_graph(),I->node_from,I->plug_from,I->node_to,I->plug_to);
	}
	
	d->undo_stream.end();
	p_track->set_rack_file( p_filename );
	return TreeLoaderDisk::ERROR_NONE;	
}



}
