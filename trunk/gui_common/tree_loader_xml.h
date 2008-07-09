//
// C++ Interface: tree_loader_xml
//
// Description: 
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TREE_LOADER_XML_H
#define TREE_LOADER_XML_H
#if 0
#include "tree_loader.h"
#include "memory/mem_vector.h"
#include "base/file.h"
#include "tree_saver_loader_disk_shared.h"

/**
	@author Juan Linietsky <red@lunatea>
*/
class TreeLoaderXML : public TreeLoader {
	
	
	struct TagParameter {
		
		String name;
		String value;
	};
	
	
	Uint32 tag_offset;
		
	struct ChildDir {
		
		String name;
		Uint32 offset_beg;
		Uint32 offset_end;
	};
	struct ChildVar {
		
		VarType type;
		String name;
		Uint32 offset_beg;
		Uint32 offset_end;
		int array_len; //cached, in the case of an array
	};
	MemVector<ChildDir> subdirs;
	MemVector<ChildVar> vars;

	MemVector<ChildDir> path;
	
	ChildVar aux;
	FileAccess *f;
	
	bool find_tag(String &p_tag_name, Uint32& p_tag_data_begin_pos,Uint32& p_tag_data_end_pos,MemVector<TagParameter> &p_paramters,Uint32 p_max_pos,bool &p_is_meta);
	
	String get_param_val(MemVector<TagParameter> &paramters, String p_name); 	
	ChildVar * get_subvar(String p_name);
	String get_data_string(Uint32 p_from,Uint32 p_to);
	void skip_tag_end(Uint32 p_limit);
	bool parse_node(Uint32 p_from,Uint32 p_to);
	
	void setup_array(Uint32 p_beg,int p_from_item,Uint32 p_limit);
	String read_array_element(Uint32 p_limit);
	
	
public:
	

	virtual bool enter(String p_dir);
	virtual bool enter_by_index(int p_index);
	virtual void exit();
	virtual String get_path();
	virtual void goto_root();

	virtual int get_int(String p_name);
	virtual void get_int_array(String p_name,int *p_arr,int p_from=0,int p_len=-1);
	virtual int get_int_array_len(String p_name);
	virtual float get_float(String p_name);
	virtual void get_float_array(String p_name,float*,int p_from=0,int p_len=-1);
	virtual int get_float_array_len(String p_name);
	virtual String get_string(String p_name);
	virtual void get_string_array(String p_name,String*,int p_from=0,int p_len=-1);
	virtual int get_string_array_len(String p_name);
	
	virtual void get_raw(String p_name,unsigned char *p_raw,int p_from=0,int p_len=-1);
	virtual int get_raw_len(String p_name);

	virtual int get_var_count();
	virtual String get_var_name(int i);

	virtual int get_child_count();
	virtual String get_child_name(int i);
	virtual bool is_child(String p_dir);

	virtual bool is_var(String p_var);
	virtual VarType get_var_type(String p_var);
	
	virtual Error open(String p_file,String p_header,FileAccess *p_custom=NULL);
	virtual void close();
	
	
	TreeLoaderXML();
	~TreeLoaderXML();
};
#endif
#endif
