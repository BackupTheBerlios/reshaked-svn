//
// C++ Interface: tree_loader_disk
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTREE_LOADER_DISK_H
#define RESHAKEDTREE_LOADER_DISK_H


#include "tree_loader.h"
#include <vector>
#include "tree_saver_loader_disk_shared.h"
#include "typedefs.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class TreeLoaderDisk : public TreeLoader {
	
	
	struct Value {
		
		String name;
		FileFieldType type;
		unsigned int pos;	
		int len; //for arrays and data		
	};
	
	
	struct Node {
		
		String name;
		Node *parent;
		
		std::vector<Value*> values;
		std::vector<Node*> childs;
	};
	
	Node *tree;
	Node *current;
	
	FILE *f;
	
	unsigned char read_byte();
	unsigned short read_short();
	unsigned int read_int();
	float read_float();
	int read_sint();
	String read_string();
	
	
	
	Value *get_value(String p_value);
	Value *extract_value(FileFieldType p_type);
	
	void erase_node(Node *p_node);
public:
	
	enum ErrorReading {
		
		ERROR_NONE,
		ERROR_CANT_OPEN_FILE,
		ERROR_FILE_UNRECOGNIZED,
		ERROR_FILE_CORRUPTED
	};
	
	
	void enter(String p_dir);
	void exit();
	String get_path();
	
	int get_int(String p_name);
	void get_int_array(String p_name,int *p_arr);
	int get_int_array_len(String p_name);
	float get_float(String p_name);
	void get_float_array(String p_name,float*);
	int get_float_array_len(String p_name);
	String get_string(String p_name);
	
	void get_raw(String p_name,unsigned char *p_raw);
	int get_raw_len(String p_name);
	
	int get_var_count();
	String get_var_name(int i);
	
	int get_child_count();
	String get_child_name(int i);
	
	ErrorReading open_file(String p_filename);
	void close_file();
	
	TreeLoaderDisk();
	~TreeLoaderDisk();

};

}

#endif
