//
// C++ Implementation: tree_loader_disk
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_loader_disk.h"

namespace ReShaked {


unsigned char TreeLoaderDisk::read_byte() {
	
	unsigned char b;
	fread(&b,1,1,f);
	
	return b;
}
unsigned short TreeLoaderDisk::read_short() {
	
	unsigned short s;
	s=read_byte() | (unsigned short)(read_byte()) << 8;
	return s;
	
}
unsigned int TreeLoaderDisk::read_int() {
	
	unsigned int i;
	i=read_short() | (unsigned int)(read_short()) << 16;
	
	return i;
	
}
int TreeLoaderDisk::read_sint() {
	
	unsigned int i=read_int();
	int *ip=(int*)&i;
	return *ip;
}

float TreeLoaderDisk::read_float() {
	
	unsigned int i=read_int();
	float *fp=(float*)&i;
	return *fp;
}


String TreeLoaderDisk::read_string() {
	
	unsigned int len=read_int();
	ERR_FAIL_COND_V( len>(4096*1024) , "" );
	
	String s;
	for (int i=0;i<len;i++) 
		s+=String::CharType( read_int() );
	
	return s;
	
}


void TreeLoaderDisk::enter(String p_dir) {
	
	for (int i=0;i<current->childs.size();i++) {
		
		if (current->childs[i]->name==p_dir) {
			
			current=current->childs[i];
			return;
		}
	}
	
	ERR_PRINT("Child dir doesnt exist");
}
void TreeLoaderDisk::exit() {
	
	ERR_FAIL_COND( current->parent==NULL );
	current=current->parent;
	
}
String TreeLoaderDisk::get_path() {
	
	String path;
	Node *n=current;
	
	while (n!=tree) {
		
		path=n->name+"/"+path;		
		n=n->parent;
	} 
	
	path="/"+path;
	
	return path;
}

TreeLoaderDisk::Value *TreeLoaderDisk::get_value(String p_value) {
	
	for (int i=0;i<current->values.size();i++) {
		
		if (current->values[i]->name==p_value) {
			
			fseek(f,current->values[i]->pos,SEEK_SET);
			return current->values[i];
			
		}
	}
	
	ERR_PRINT("CANT FIND PROPERTY AT PATH:");
	printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_value.ascii().get_data() );
	return NULL;
}

int TreeLoaderDisk::get_int(String p_name) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return -1;
	}
	if (v->type!=FILE_FIELD_INT) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_INT,0);
	
	return read_sint();
		
}
void TreeLoaderDisk::get_int_array(String p_name,int *p_arr) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return ;
	}
	if (v->type!=FILE_FIELD_INT_ARRAY) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}

	ERR_FAIL_COND(v->type!=FILE_FIELD_INT_ARRAY);
	
	for (int i=0;i<v->len;i++) {
		
		*p_arr=read_int();
		p_arr++;
	}
	
}
int TreeLoaderDisk::get_int_array_len(String p_name) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return 0;
	}
	if (v->type!=FILE_FIELD_INT_ARRAY) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_INT_ARRAY,0);
	
	return v->len;
}
float TreeLoaderDisk::get_float(String p_name) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return 0;
	}
	
	if (v->type!=FILE_FIELD_FLOAT) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_FLOAT,0);
	
	return read_float();
}
void TreeLoaderDisk::get_float_array(String p_name,float* p_arr) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return;
	}

	if (v->type!=FILE_FIELD_FLOAT_ARRAY) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND(v->type!=FILE_FIELD_FLOAT_ARRAY);
	
	for (int i=0;i<v->len;i++) {
		
		*p_arr=read_float();
		p_arr++;
	}
	
}
int TreeLoaderDisk::get_float_array_len(String p_name) {
	
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return 0;
	}
	
	if (v->type!=FILE_FIELD_FLOAT_ARRAY) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_FLOAT_ARRAY,0);

	return v->len;
}
String TreeLoaderDisk::get_string(String p_name) {
	
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return "";
	}
	
	if (v->type!=FILE_FIELD_STRING) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_STRING,"");
	
	return read_string();
}

void TreeLoaderDisk::get_raw(String p_name,unsigned char *p_raw) {
	
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return;
	}
	
	if (v->type!=FILE_FIELD_RAW) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND(v->type!=FILE_FIELD_RAW);
	
	for (int i=0;i<v->len;i++) {
		
		*p_raw=read_byte();
		p_raw++;
	}
	
	
}
int TreeLoaderDisk::get_raw_len(String p_name) {
	
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return 0;
	}
	
	if (v->type!=FILE_FIELD_RAW) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_RAW,0);
	
	return v->len;
}

TreeLoaderDisk::Value *TreeLoaderDisk::extract_value(FileFieldType p_type) {
	
	Value *v = new Value;
	v->type=p_type;
	v->name=read_string();
	v->pos=ftell(f);
		
	switch (p_type) {
		
		case FILE_FIELD_INT: {
				
			read_int(); //skip int				
								
		} break;
		case FILE_FIELD_INT_ARRAY: {
				
			v->len=read_int(); //save len
			for (int i=0;i<v->len;i++)
				read_int(); //skip it 
				
		} break;
		case FILE_FIELD_FLOAT: {
				
			read_float(); //skip it 
		} break;
		case FILE_FIELD_FLOAT_ARRAY: {
				
			v->len=read_int(); //save len
			for (int i=0;i<v->len;i++)
				read_float(); //skip it 
				
		} break;
		case FILE_FIELD_STRING: {
				
			read_string();
		} break;
		case FILE_FIELD_RAW: {
				
			v->len=read_int(); //save len
			for (int i=0;i<v->len;i++)
				read_byte(); //skip it 
				
		} break;
		default: {
			
			ERR_PRINT("Type not handled?");
			return NULL;
		}
	}
	
	return v;
	
}

TreeLoaderDisk::ErrorReading TreeLoaderDisk::open_file(String p_file) {
	
	f = fopen(p_file.utf8().get_data(),"rb");
	
	if (!f) {
		
		return ERROR_CANT_OPEN_FILE;
	}
	
	char *buffer=new char[mime.length()+1];
	fread(buffer,mime.length()+1,1,f);
	buffer[mime.length()]=0;//zero just in case
	String mime_compare=buffer;
	delete buffer;
	if (mime_compare!=mime) {
		
		fclose(f);
		return ERROR_FILE_UNRECOGNIZED;
	}
	
	int version_compare=read_int();
	if (version<version_compare) {
		
		fclose(f);
		return ERROR_VERSION_TOO_NEW;
	}
	if (compatible_version>version_compare) {
		
		fclose(f);
		return ERROR_VERSION_TOO_OLD;
	}
		
	
	tree = new Node;
	tree->parent=NULL;
	current=tree;
	
	bool read_ok=false;
	
	while(true) {
		
		
		unsigned char command=read_byte();
		
		switch (command) {
			
			
			case FILE_FIELD_ENTER: {
				
				Node *new_n=new Node;
				new_n->parent=current;
				new_n->name=read_string();
				printf("ENTER: %lls\n",new_n->name.c_str());
				current->childs.push_back(new_n);
				enter(new_n->name);		
				
			} break;
			case FILE_FIELD_EXIT: {
				printf("EXIT: %lls\n",current->name.c_str());
				exit();
			} break;
			case FILE_FIELD_INT:
			case FILE_FIELD_INT_ARRAY:
			case FILE_FIELD_FLOAT:
			case FILE_FIELD_FLOAT_ARRAY:
			case FILE_FIELD_STRING:
			case FILE_FIELD_RAW: {
				
				Value * v= extract_value( (FileFieldType)command);
				if (!v)
					break; //corrupt
				printf("Value: %lls\n",v->name.c_str());
				current->values.push_back(v);
				
			} break;
			case FILE_FIELD_EOF: {
				printf("EOF!\n");
				read_ok=true;
				
			} break;
			
			default: {
				
				ERR_PRINT("Invalid Command!");
				break; //there should be no default!
				
				
			}
	
		}
		
		if (read_ok)
			break;
		
		if (feof(f)) {
			ERR_PRINT("Premature end of file!");
			break;
		}
		
	}
	if (!read_ok) {
		
		fclose(f);
		return ERROR_FILE_CORRUPTED;
		
	}
	current=tree;
	
	return ERROR_NONE;

	
}

int TreeLoaderDisk::get_var_count() {
	
	return current->values.size();
}
String TreeLoaderDisk::get_var_name(int i) {
	
	ERR_FAIL_INDEX_V(i,current->values.size(),"");
	return current->values[i]->name;
}

bool TreeLoaderDisk::is_var(String p_var) {
	
	for (int i=0;i<current->values.size();i++)
		if (current->values[i]->name==p_var)
			return true;
	
	return false;
}
TreeLoader::VarType TreeLoaderDisk::get_var_type(String p_var) {
	Value *v=NULL;
	for (int i=0;i<current->values.size();i++) { 
		if (current->values[i]->name==p_var) {
			v=current->values[i];
		}
	}
		
	if (v==NULL)
		return VAR_NONE;		
		
	switch (v->type) {
		
		case FILE_FIELD_INT: {
			
			return VAR_INT;
		} break;
		case FILE_FIELD_INT_ARRAY: {
			
			return VAR_INT_ARRAY;
		} break;
		case FILE_FIELD_FLOAT: {
			
			return VAR_FLOAT;
		} break;
		case FILE_FIELD_FLOAT_ARRAY: {
		
			return VAR_FLOAT_ARRAY;
		} break;
		case FILE_FIELD_STRING: {
		
			return VAR_STRING;
		} break;
		case FILE_FIELD_RAW: {
			
			return VAR_RAW;
		} break;
		
		default: return VAR_NONE; break;
		
	}

	//unreachable code
	
	return VAR_NONE;
}

void TreeLoaderDisk::goto_root() {
	
	current=tree;
}

int TreeLoaderDisk::get_child_count() {
	
	return current->childs.size();
}
String TreeLoaderDisk::get_child_name(int i) {
	
	ERR_FAIL_INDEX_V(i,current->childs.size(),"");
	return current->childs[i]->name;
}


void TreeLoaderDisk::erase_node(Node *p_node) {
	
	
	for (int i=0;i<p_node->values.size();i++)
		delete p_node->values[i];
	
	for (int i=0;i<p_node->childs.size();i++)
		erase_node( p_node->childs[i] );
	
	
	delete p_node;
}

void TreeLoaderDisk::close_file() {
	
	
	fclose(f);
	erase_node( tree );
	
}



TreeLoaderDisk::TreeLoaderDisk(String p_mime,int p_version,int p_compatible_version)
{
	mime=p_mime;
	version=p_version;
	compatible_version=p_compatible_version;
	
}


TreeLoaderDisk::~TreeLoaderDisk()
{
}


}
