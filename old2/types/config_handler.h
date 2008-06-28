/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "typedefs.h"
#include <stdio.h>
#include "tree_loader.h"
#include "tree_saver.h"

/**
  *@author Juan Sebastian Linietsky
  */
namespace ReShaked {

class ConfigHandler {


	String header_check;
	
	void remove_trailing_space(std::string*);
	void save_subtree(String p_path,FILE *f, TreeLoader *p_loader);
	
	void add_item(TreeSaver *p_saver,String p_section,String p_key,String p_value);
public:
	void set_header_check(String p_header_check);

	bool save(String p_filename,TreeLoader *p_loader); //read from tree, save to file
	bool load(String p_filename,TreeSaver *p_saver);  //read from file, save to tree

	ConfigHandler();
	~ConfigHandler();
};

}
#endif
