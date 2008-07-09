//
// C++ Interface: tree_saver_xml
//
// Description: 
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TREE_SAVER_XML_H
#define TREE_SAVER_XML_H

#if 0
/**
	@author Juan Linietsky <red@lunatea>
*/

#include "tree_saver.h"
#include "memory/mem_vector.h"
#include "tree_saver_loader_disk_shared.h"
#include "os/file_access.h"
#include "list.h"
/**
	@author Juan Linietsky <red@lunatea>
*/
class TreeSaverXML : public TreeSaver {

	List<String> path_stack;
	int level;
	FileAccess *f;
	bool own_f;

	void tabulate();
	void escape(String&);
	void write_string(String p_str,bool p_escape=true);
	void write_tag(String p_tag,bool p_escape=true);
	bool valid_name(String p_name);
public:
	
	virtual void enter(String p_dir);
	virtual void exit();
	virtual String get_path();
	virtual void goto_root();

	virtual void add_int(String p_name,int p_int);
	virtual void add_int_array(String p_name,const int *p_arr,int p_len);
	virtual void add_float(String p_name,float p_float);
	virtual void add_float_array(String p_name,const float *p_arr,int p_len);
	virtual void add_string(String p_name,String p_string);
	void add_string_array(String p_name,String *p_arr,int p_len);
	
	virtual void add_raw(String p_name,const unsigned char *p_raw,int p_bytes);
	
	virtual bool save(String p_filename,String p_ascii_header,FileAccess *p_custom=NULL);
	virtual void close();
	
	TreeSaverXML();
	~TreeSaverXML();

};

#endif
#endif