//
// C++ Interface: default_paths
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEFAULT_PATHS_H
#define DEFAULT_PATHS_H
#include "typedefs.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class DefaultPaths{

	static String base_path;
public:
	
	static String get_presets_path();
	static String get_config_path();
	static String get_theme_path();
	
	static void ensure_node_preset_dir_exists(String p_ID);
	static void set_base_path(String p_path);

};

#endif
