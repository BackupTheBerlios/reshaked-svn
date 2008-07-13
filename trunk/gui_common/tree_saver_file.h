//
// C++ Interface: tree_saver_file
//
// Description: 
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TREE_SAVER_FILE_H
#define TREE_SAVER_FILE_H

#include "tree_saver.h"
#include <vector>
#include "tree_saver_loader_disk_shared.h"
#include "base/file.h"
/**
	@author Juan Linietsky <red@lunatea>
	Binaty format is docummented at:
	docs/data_tree_format_binary.txt
*/
class TreeSaverFile : public TreeSaver {

	struct TreeNode {
		
		TreeNode *parent;
		std::vector<TreeNode*> childs;
		String name;
		
		FileFieldType type;
		
		std::vector<int> int_array;
		std::vector<float> float_array;
		std::vector<String> string_array;
		String string_val;
		int int_val;
		float float_val;
		std::vector<unsigned char> raw_array;
		
		TreeNode() { parent=0; type=FILE_FIELD_EOF; int_val=0; float_val=0; }
		~TreeNode() {
			for (int i=0; i<childs.size(); i++) {
				delete childs[i];
			};
		};
	};

	TreeNode root;
	TreeNode *current;		
	bool utf8;
	
	bool validate_child_name(String p_name);
	
	void save_listing_node_data(GUI::File *f,std::vector<uint32_t>& childpos);
	unsigned int save_node(GUI::File *f);
	
	GUI::File *f;
	bool own_f;
	
	static TreeSaver* create_tree_saver_file();
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
	
	virtual Error save(String p_fileID, String p_filename);
	void close();
	
	static void make_default();
	
	TreeSaverFile();
	~TreeSaverFile();

};

#endif
