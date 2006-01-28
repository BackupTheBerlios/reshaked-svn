//
// C++ Interface: tree_loader
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTREE_LOADER_H
#define RESHAKEDTREE_LOADER_H

#include "rstring.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class TreeLoader{
public:
	
	virtual void enter(String p_dir)=0;
	virtual void exit()=0;
	virtual String get_path()=0;
	
	virtual int get_int(String p_name)=0;
	virtual void get_int_array(String p_name,int *p_arr)=0;
	virtual int get_int_array_len(String p_name)=0;
	virtual float get_float(String p_name)=0;
	virtual void get_float_array(String p_name,float*)=0;
	virtual int get_float_array_len(String p_name)=0;
	virtual String get_string(String p_name)=0;
	virtual void get_raw(String p_name,unsigned char *p_raw)=0;
	virtual int get_raw_len(String p_name)=0;
	
	virtual int get_var_count()=0;
	virtual String get_var_name(int i)=0;
	
	virtual int get_child_count()=0;
	virtual String get_child_name(int i)=0;
	
	
	TreeLoader();
	virtual ~TreeLoader();

};

}

#endif
