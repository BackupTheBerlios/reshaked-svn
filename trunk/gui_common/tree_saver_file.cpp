//
// C++ Implementation: tree_saver_file
//
// Description:
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_saver_file.h"

static inline unsigned int hash_djb2(const char *p_cstr) {

	const unsigned char* chr=(const unsigned char*)p_cstr;
	unsigned int hash = 5381;
	unsigned int c;
	
	while ((c = *chr++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}

void TreeSaverFile::enter(String p_dir) {

	ERR_FAIL_COND(p_dir.length()>MAX_ENTRY_NAME_LEN);
	ERR_FAIL_COND(p_dir.length()==0);

	for (int i=0;i<current->childs.size();i++) {

		if (current->childs[i]->name==p_dir) {

			ERR_FAIL_COND( current->childs[i]->type != FILE_FIELD_ENTER );

			current=current->childs[i];
			return; //already exists
		}
	}

	for (int i=0;i<p_dir.size();i++) {

		ERR_FAIL_COND(p_dir[i]=='/');
		ERR_FAIL_COND(p_dir[i]=='"');
		ERR_FAIL_COND(p_dir[i]=='&');
		ERR_FAIL_COND(p_dir[i]=='\'');
		ERR_FAIL_COND(p_dir[i]=='<');
		ERR_FAIL_COND(p_dir[i]=='>');

		ERR_FAIL_COND(p_dir[i]<32 && p_dir[i]>127);

	}



	TreeNode *n = new TreeNode;

	n->name=p_dir;
	n->type=FILE_FIELD_ENTER;
	n->parent=current;
	current->childs.push_back(n);


	current=n;

}
void TreeSaverFile::exit() {

	ERR_FAIL_COND(!current->parent);
	current=current->parent;
}
String TreeSaverFile::get_path(){

	String path;

	while(current->parent) {

		path="/"+current->name+path;
		current=current->parent;
	}

	return path;
}

void TreeSaverFile::goto_root(){

	current=&root;
}

bool TreeSaverFile::validate_child_name(String p_name) {

	ERR_FAIL_COND_V(p_name=="",true);
	ERR_FAIL_COND_V(p_name.length()>MAX_ENTRY_NAME_LEN,true);

	for (int i=0;i<current->childs.size();i++) {

		ERR_FAIL_COND_V(current->childs[i]->name==p_name,true);
	}

	for (int i=0;i<p_name.size();i++) {

		ERR_FAIL_COND_V(p_name[i]=='/',true);
		ERR_FAIL_COND_V(p_name[i]=='"',true);
		ERR_FAIL_COND_V(p_name[i]=='&',true);
		ERR_FAIL_COND_V(p_name[i]=='\'',true);
		ERR_FAIL_COND_V(p_name[i]=='<',true);
		ERR_FAIL_COND_V(p_name[i]=='>',true);

		ERR_FAIL_COND_V(p_name[i]<32 && p_name[i]>127,true); //must be ascii

	}

	return false;
}

void TreeSaverFile::add_int(String p_name,int p_int){

	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_INT;
	n->int_val=p_int;
	n->parent=current;
	current->childs.push_back(n);
}
void TreeSaverFile::add_int_array(String p_name,const int *p_arr,int p_len){

	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_INT_ARRAY;
	n->int_array.resize(p_len);
	n->parent=current;

	for (int i=0;i<p_len;i++) {

		n->int_array[i]=p_arr[i];
	}

	current->childs.push_back(n);

}
void TreeSaverFile::add_float(String p_name,float p_float){

	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_FLOAT;
	n->float_val=p_float;
	n->parent=current;

	current->childs.push_back(n);

}

void TreeSaverFile::add_float_array(String p_name,const float *p_arr,int p_len){

	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_FLOAT_ARRAY;
	n->float_array.resize(p_len);
	n->parent=current;

	for (int i=0;i<p_len;i++) {

		n->float_array[i]=p_arr[i];
	}

	current->childs.push_back(n);

}

void TreeSaverFile::add_string(String p_name,String p_string){


	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_STRING;
	n->string_val=p_string;
	n->parent=current;

	current->childs.push_back(n);

}
void TreeSaverFile::add_string_array(String p_name,String *p_arr,int p_len){


	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_STRING_ARRAY;
	n->string_array.resize(p_len);
	n->parent=current;

	for (int i=0;i<p_len;i++) {

		n->string_array[i]=p_arr[i];
	}

	current->childs.push_back(n);

}

void TreeSaverFile::add_raw(String p_name,const unsigned char *p_arr,int p_bytes){

	ERR_FAIL_COND(validate_child_name(p_name));

	TreeNode *n = new TreeNode;
	n->name=p_name;
	n->type=FILE_FIELD_FLOAT_ARRAY;
	n->raw_array.resize(p_bytes);
	n->parent=current;

	for (int i=0;i<p_bytes;i++) {

		n->raw_array[i]=p_arr[i];
	}

	current->childs.push_back(n);

}

struct ChildHash {

	unsigned int hash;
	unsigned int offset;
};

void TreeSaverFile::save_listing_node_data(GUI::File *f,std::vector<unsigned int>& childpos) {


	ERR_FAIL_COND( childpos.size()!=current->childs.size() ); //just check for bugs

	f->store_32( childpos.size() ); //tablesize, if 0, no table

	if (childpos.size()) { //if table..
		std::vector<ChildHash> child_hashes;
		child_hashes.resize( childpos.size() );

		for (int i=0;i<childpos.size();i++) {

			ChildHash ch;
			ch.offset=childpos[i];
			ch.hash=hash_djb2(current->childs[i]->name.ascii().get_data());
			child_hashes[i]=ch;

		}



		{ //sort them, not many elements overall so bubblesort
			int swaps=0;

			do {
				swaps=0;

				for (int i=0;i<(child_hashes.size()-1);i++) {

					if ( child_hashes[i].hash<=child_hashes[i+1].hash )
						continue;
					ChildHash aux=child_hashes[i];
					child_hashes[i]=child_hashes[i+1];
					child_hashes[i+1]=aux;
					swaps++;
				}
			} while (swaps);
		}

		/* Save the ordered hashtable */

		for (int i=0;i<child_hashes.size();i++) {

			f->store_32( child_hashes[i].hash );
			f->store_32( child_hashes[i].offset );
		}


	}

	int subdirs=0;
	for (int i=0;i<current->childs.size();i++)
		if (current->childs[i]->type==FILE_FIELD_ENTER)
			subdirs++;

	f->store_32( subdirs ); /// store amount of childs that are subdirs
	f->store_32( current->childs.size()-subdirs ); /// store amount of childs that not subdirs
	//save subdirs
	for (int i=0;i<current->childs.size();i++) {

		if (current->childs[i]->type!=FILE_FIELD_ENTER)
			continue;

		f->store_32( childpos[i] );
	}
	//save variables
	for (int i=0;i<current->childs.size();i++) {

		if (current->childs[i]->type==FILE_FIELD_ENTER)
			continue;

		f->store_32( childpos[i] );
	}


}

union FloatIntUnion {

	float f;
	unsigned int i;
};

unsigned int TreeSaverFile::save_node(GUI::File *f) {

	std::vector<unsigned int> savepos;

	for (int i=0;i<current->childs.size();i++) {


		current=current->childs[i]; //change to child node
		unsigned int nodepos=save_node(f);
		savepos.push_back( nodepos );
		if (nodepos==0xFFFFFFFF)
			return nodepos; //error
		current=current->parent; //change to this child again

	}

	unsigned int nodepos=f->get_pos();

	f->store_8( current->type );

	f->store_32( current->name.length() );
	f->store_buffer( (unsigned char*)current->name.ascii().get_data(), current->name.length() );

	switch( current->type ) {

		case FILE_FIELD_ENTER: {

			save_listing_node_data(f,savepos);

		} break;
		case FILE_FIELD_INT: {
#define save_signed_int(m_int) {\
	unsigned int *me=(unsigned int*)&m_int;\
	f->store_32( *me );\
}
			save_signed_int(current->int_val);
		} break;
		case FILE_FIELD_INT_ARRAY: {

			f->store_32( current->int_array.size() );
			for (int i=0;i<current->int_array.size();i++) {

				int val=current->int_array[i];
				save_signed_int(val);
			}

		} break;
		case FILE_FIELD_FLOAT: {

			FloatIntUnion fu;
			fu.f=current->float_val;
#ifdef INVERTED_FLOAT_ENDIANNESS
			fu.i=BSWAP32(fu.i);
#endif
			f->store_32( fu.i );

		} break;
		case FILE_FIELD_FLOAT_ARRAY: {

			f->store_32( current->float_array.size() );
			for (int i=0;i<current->float_array.size();i++) {

				FloatIntUnion fu;
				fu.f=current->float_array[i];
#ifdef INVERTED_FLOAT_ENDIANNESS
				fu.i=BSWAP32(fu.i);
#endif
				f->store_32( fu.i );

			}

		} break;
		case FILE_FIELD_STRING: {


			String s=current->string_val;

			if (utf8)
				s=s.utf8().get_data(); // reinterpret utf8 as a regular string

			f->store_32( s.length() );
			for (int i=0;i<s.length();i++) {

				if (utf8 || sizeof(String::CharType)==1) {

					f->store_8( s[i] );
				} else if (sizeof(String::CharType)==4) {

					f->store_32( s[i] );
				} else if (sizeof(String::CharType)==2) {

					f->store_16( s[i] );
				} else {

					ERR_PRINT("BUG ??");
				}
			}

		} break;
		case FILE_FIELD_STRING_ARRAY: {

			f->store_32( current->string_array.size() );

			for (int j=0;j<current->string_array.size();j++) {

				String s=current->string_array[j];
				if (utf8)
					s=s.utf8().get_data(); // reinterpret utf8 as a regular string

				f->store_32( s.length() );
				for (int i=0;i<s.length();i++) {

					if (utf8 || sizeof(String::CharType)==1) {

						f->store_8( s[i] );
					} else if (sizeof(String::CharType)==4) {

						f->store_32( s[i] );
					} else if (sizeof(String::CharType)==2) {

						f->store_16( s[i] );
					} else {

						ERR_PRINT("BUG ??");
					}
				}
			}

		} break;
		case FILE_FIELD_RAW: {


			f->store_buffer( (unsigned char*)&current->raw_array[0], current->raw_array.size() );

		} break;
		default: {
			ERR_PRINT("Invalid Node");
			return 0xFFFFFFFF;
		}

	}

	return nodepos;
}


Error TreeSaverFile::save(String p_fileID, String p_filename) {



	ERR_FAIL_COND_V(f,ERR_ALREADY_IN_USE);

	f = GUI::File::create();
	
	GUI::File::FileError err = f->open(p_filename,GUI::File::WRITE);
	if ( err != GUI::File::OK) {

		ERR_PRINT("Error opening file for writing: %i");
		printf("errcode %i\n",err);
		return ERR_CANT_OPEN;
	}

	f->store_buffer( (unsigned char*)"RSHT", 4 ); //magic number

	f->store_32( p_fileID.length() ); //saver version

	f->store_buffer( (unsigned char*)p_fileID.ascii().get_data(), p_fileID.length() );

	f->store_32( 1 ); //saver version

	if (utf8)
		f->store_buffer( (unsigned char*)"UTF-8", 5 );
	else if (sizeof(String::CharType)==4)
		f->store_buffer( (unsigned char*)"UCS32", 5 );
	else if (sizeof(String::CharType)==2)
		f->store_buffer( (unsigned char*)"UCS16", 5 );
	else if (sizeof(String::CharType)==1)
		f->store_buffer( (unsigned char*)"ASCII", 5 );
	else {
		ERR_PRINT("Unknown Encoding\n");
		delete f;
		return ERR_INVALID_DATA;
	}

	f->store_buffer( (unsigned char*)"LE", 2 ); /* Little Endian */
	f->store_buffer( (unsigned char*)"DJB2", 4 ); /* Hash Type */
	f->store_32( MAX_ENTRY_NAME_LEN ); /* max name len */

	current=&root;
	printf("opened ok\n");
	return OK;

}

void TreeSaverFile::close() {

	ERR_FAIL_COND(!f);

	unsigned int seekpos=f->get_pos();
	f->store_32( 0 ); //POS FOR ROOT NODE
	current=&root;
	unsigned int rootpos=save_node(f);
	if (rootpos==0xFFFFFFFF) { //error saving, return error
		printf("error saving :(\n");
		f->close();
		return;
	}
	f->seek(seekpos);
	f->store_32( rootpos ); //POS FOR ROOT NODE, REAL
	f->close();

	delete f;
	f=NULL;

}

TreeSaver* TreeSaverFile::create_tree_saver_file() {

	return new TreeSaverFile;
}

void TreeSaverFile::make_default() {

	create_func=create_tree_saver_file;
}

TreeSaverFile::TreeSaverFile() {

	f=NULL;
	utf8=true;
	root.name="";
	root.type=FILE_FIELD_ENTER;
}


TreeSaverFile::~TreeSaverFile() {

	if (f)
		close();

}



