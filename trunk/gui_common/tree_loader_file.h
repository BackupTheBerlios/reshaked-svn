
//
// C++ Interface: tree_loader_file
//
// Description: 
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TREE_LOADER_FILE_H
#define TREE_LOADER_FILE_H

#include "tree_loader.h"
#include "tree_saver_loader_disk_shared.h"
#include "base/file.h"
#include <vector>

/**
	@author Juan Linietsky <red@lunatea>
	Binaty format is docummented at:
	docs/data_tree_format_binary.txt

*/
class TreeLoaderFile : public TreeLoader {

	
	enum {
			
		MAX_HASHTABLE_SIZE=256
	};
	enum HashType {
		HASH_NONE,
 		HASH_DJB2
	};
	enum StringType {
		
		TYPE_UCS32,
  		TYPE_UCS16,
    		TYPE_ASCII,
      		TYPE_UTF8
	};
		

	HashType hash_type;
	StringType string_type;
	uint32_t max_entry_len;
	
	
	struct HashItem {
		
		uint32_t hash;
		uint32_t offset;
	};
	
	std::vector<HashItem> hash_table;
	
	/* Buffers */
	char name_buf[MAX_ENTRY_NAME_LEN];
	
	
	struct Current {

		std::vector<uint32_t> stack;
		
		uint32_t dir_count;
		uint32_t data_count;
		uint32_t dir_offsets_offset;
		uint32_t data_offsets_offset; //helper , same as dir_offsets_offset+data_count*4
	} current;
	
	/* helpers */
	
	bool enter_offset(uint32_t p_offset);
	bool is_offset_name_and_type( uint32_t p_ofs, String p_name, bool p_check_type,FileFieldType p_type );
	uint32_t get_node_offset(String p_name, bool p_check_type,FileFieldType p_type);
	bool enter_data( String p_name, FileFieldType p_type );
	
	GUI::File *f;

	static TreeLoader *create_tree_loader_file();
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

	
	virtual Error open(String p_fileID, String p_filename);

	virtual void close();

	static void make_default();

	TreeLoaderFile();
	~TreeLoaderFile();

};

#endif
