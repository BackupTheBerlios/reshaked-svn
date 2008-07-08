//
// C++ Implementation: default_paths
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "default_paths.h"
#include "base/file_system.h"

String DefaultPaths::base_path;


String DefaultPaths::get_presets_path() {

	return base_path+"/presets";
}
String DefaultPaths::get_config_path() {

	return base_path+"/reshaked.cfg";
}
String DefaultPaths::get_theme_path() {
	
	return base_path+"/themes";
}

void DefaultPaths::ensure_node_preset_dir_exists(String p_ID) {

	GUI::FileSystem *fs = GUI::FileSystem::instance_func();
	String preset_path =get_presets_path()+"/"+p_ID; 
	if (fs->change_dir(preset_path))
		fs->make_dir(preset_path);
		
	delete fs;

}

void DefaultPaths::set_base_path(String p_path) {

	printf("attempting to change path to %s\n",p_path.ascii().get_data());
	GUI::FileSystem *fs = GUI::FileSystem::instance_func();
	
	if (fs->change_dir(p_path)) { // failed
	
		fs->make_dir( p_path );
	}
	
	ERR_FAIL_COND( fs->change_dir(p_path) ); // eek can't change dir
	
	if (fs->change_dir("presets")) { // failed
	
		fs->make_dir( "presets" );		
	} else {
	
		fs->change_dir("..");
	}
	
	if (fs->change_dir("themes")) { // failed
	
		fs->make_dir( "themes" );		
	} else {
	
		fs->change_dir("..");
	}
	
	base_path=p_path;
	
	delete fs;
}