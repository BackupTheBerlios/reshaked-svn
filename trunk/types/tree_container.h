//
// C++ Interface: tree_container
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTREE_CONTAINER_H
#define RESHAKEDTREE_CONTAINER_H

#include "tree_loader.h"
#include "tree_saver.h"
#include "tree_saver_loader_disk_shared.h"
#include <vector>
#include "typedefs.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class TreeContainer : public TreeSaver, public TreeLoader {
	
	
	
	struct Value {
		
		String name;
		FileFieldType type;
		
		String string_val;
		union {
			int int_val;
			float float_val;
			float *float_array_val;
			int *int_array_val;
			unsigned char * raw;
			
		} data;
		
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
	
	
	void erase_node(Node *p_node);
	Value *get_value(String p_value);
	
	void load_node(TreeLoader *p_loader);
public:
	
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
	
	void add_int(String p_name,int p_int);
	void add_int_array(String p_name,int *p_arr,int p_len);
	void add_float(String p_name,float p_float);
	void add_float_array(String p_name,float *p_arr,int p_len);
	void add_string(String p_name,String p_string);
	void add_raw(String p_name,unsigned char *p_raw,int p_bytes);
	
	bool is_var(String p_name);
	VarType get_var_type(String p_var);
	
	void goto_root();

	TreeContainer(TreeLoader *p_loader);
	TreeContainer();
	~TreeContainer();

};

}

#endif
