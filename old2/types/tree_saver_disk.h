//
// C++ Interface: tree_saver_disk
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTREE_SAVER_DISK_H
#define RESHAKEDTREE_SAVER_DISK_H


#include "tree_saver.h"
#include <list>
#include "tree_saver_loader_disk_shared.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class TreeSaverDisk : public TreeSaver{

	
	
	void write_byte(unsigned char p_byte);
	void write_short(unsigned short p_short);
	void write_int(unsigned int p_int);
	void write_sint(int p_int);
	void write_string(String p_string);
	void write_float(float p_float);
	
	std::list<String> path;
	
	FILE *f;

	String mime;
	int version;
public:
	
	void enter(String p_dir);
	void exit();
	String get_path();
	void goto_root();
	
	void add_int(String p_name,int p_int);
	void add_int_array(String p_name,const int *p_arr,int p_len);
	void add_float(String p_name,float p_float);
	void add_float_array(String p_name,const float *p_arr,int p_len);
	void add_string(String p_name,String p_string);
	void add_raw(String p_name,const unsigned char *p_raw,int p_bytes);
	
	bool open_file(String p_file);
	void close_file();
	
	TreeSaverDisk(String p_mime,int p_version);
	~TreeSaverDisk();

};

}

#endif
