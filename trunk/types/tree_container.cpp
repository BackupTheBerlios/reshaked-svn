//
// C++ Implementation: tree_container
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_container.h"

namespace ReShaked {


void TreeContainer::enter(String p_dir) {
	
	for (int i=0;i<current->childs.size();i++) {
		
		if (current->childs[i]->name==p_dir) {
			
			current=current->childs[i];
			return;
		}
	}
	
	/* Child doesnt exist, create it */
	 
	Node * n = new Node;
	n->parent=current;
	current->childs.push_back(n);
	n->name=p_dir;
	current=n;
}
void TreeContainer::exit() {
	
	ERR_FAIL_COND( current->parent==NULL );
	current=current->parent;
	
}
String TreeContainer::get_path() {
	
	String path;
	Node *n=current;
	
	while (n!=tree) {
		
		path=n->name+"/"+path;		
		n=n->parent;
	} 
	
	path="/"+path;
	
	return path;
}

TreeContainer::Value *TreeContainer::get_value(String p_value) {
	
	for (int i=0;i<current->values.size();i++) {
		
		if (current->values[i]->name==p_value) {
			
			return current->values[i];
			
		}
	}
	
	ERR_PRINT("CANT FIND PROPERTY AT PATH:");
	printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_value.ascii().get_data() );
	return NULL;
}



TreeLoader::VarType TreeContainer::get_var_type(String p_var) {
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

bool TreeContainer::is_var(String p_name) {
	
	return (get_var_type(p_name)!=VAR_NONE);
}

int TreeContainer::get_int(String p_name) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return -1;
	}
	if (v->type!=FILE_FIELD_INT) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_INT,0);
	
	return v->data.int_val;
		
}
void TreeContainer::get_int_array(String p_name,int *p_arr) {
	
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
		
		p_arr[i]=v->data.int_array_val[i];
	}
	
}
int TreeContainer::get_int_array_len(String p_name) {
	
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
float TreeContainer::get_float(String p_name) {
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return 0;
	}
	
	if (v->type!=FILE_FIELD_FLOAT) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_FLOAT,0);
	
	return v->data.float_val;
}
void TreeContainer::get_float_array(String p_name,float* p_arr) {
	
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
		
		p_arr[i]=v->data.float_array_val[i];
	}
	
}
int TreeContainer::get_float_array_len(String p_name) {
	
	
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
String TreeContainer::get_string(String p_name) {
	
	
	Value *v;
	if ((v=get_value(p_name))==NULL) {
		
		return "";
	}
	
	if (v->type!=FILE_FIELD_STRING) {
		
		ERR_PRINT("WRONG_PROPERTY_TYPE:");
		printf("***PATH: %s : %s\n",get_path().ascii().get_data(), p_name.ascii().get_data() );
	}
	ERR_FAIL_COND_V(v->type!=FILE_FIELD_STRING,"");
	
	return v->string_val;
}

void TreeContainer::get_raw(String p_name,unsigned char *p_raw) {
	
	
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
		
		p_raw[i]=v->data.raw[i];
	}
	
	
}
int TreeContainer::get_raw_len(String p_name) {
	
	
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



int TreeContainer::get_var_count() {
	
	return current->values.size();
}
String TreeContainer::get_var_name(int i) {
	
	ERR_FAIL_INDEX_V(i,current->values.size(),"");
	return current->values[i]->name;
}

int TreeContainer::get_child_count() {
	
	return current->childs.size();
}
String TreeContainer::get_child_name(int i) {
	
	ERR_FAIL_INDEX_V(i,current->childs.size(),"");
	return current->childs[i]->name;
}




void TreeContainer::add_int(String p_name,int p_int) {
	
	ERR_FAIL_COND(is_var(p_name));	
	
	Value *v = new Value;
	v->type=FILE_FIELD_INT;
	v->name=p_name;
	v->data.int_val=p_int;
	
	current->values.push_back(v);
		
}
void TreeContainer::add_int_array(String p_name,int *p_arr,int p_len) {
	
	ERR_FAIL_COND(is_var(p_name));	
	
	Value *v = new Value;
	v->type=FILE_FIELD_INT_ARRAY;
	v->name=p_name;
	
	v->data.int_array_val=new int[p_len];
	
	for (int i=0;i<p_len;i++) 
		v->data.int_array_val[i]=p_arr[i];
	
		
	v->len=p_len;
	current->values.push_back(v);
	
}
void TreeContainer::add_float(String p_name,float p_float)  {
	
	ERR_FAIL_COND(is_var(p_name));	
	
	Value *v = new Value;
	v->type=FILE_FIELD_FLOAT;
	v->name=p_name;
	v->data.float_val=p_float;
	current->values.push_back(v);
	
}
void TreeContainer::add_float_array(String p_name,float *p_arr,int p_len) {
	
	ERR_FAIL_COND(is_var(p_name));	
	
	Value *v = new Value;
	v->type=FILE_FIELD_FLOAT_ARRAY;
	v->name=p_name;
	
	v->data.float_array_val=new float[p_len];
	
	for (int i=0;i<p_len;i++) 
		v->data.float_array_val[i]=p_arr[i];
	
		
	v->len=p_len;
	current->values.push_back(v);
	
}
void TreeContainer::add_string(String p_name,String p_string) {
	
	ERR_FAIL_COND(is_var(p_name));	
	
	Value *v = new Value;
	v->type=FILE_FIELD_STRING;
	v->name=p_name;
	v->string_val=p_string;
	current->values.push_back(v);
}
void TreeContainer::add_raw(String p_name,unsigned char *p_raw,int p_bytes) {
	
	ERR_FAIL_COND(is_var(p_name));	
	
	Value *v = new Value;
	v->type=FILE_FIELD_RAW;
	v->name=p_name;
	
	v->data.raw=new unsigned char[p_bytes];
	
	for (int i=0;i<p_bytes;i++) 
		v->data.raw[i]=p_raw[i];
	
		
	v->len=p_bytes;
	
	current->values.push_back(v);
	
}

void TreeContainer::erase_node(Node *p_node) {
	
	
	for (int i=0;i<p_node->values.size();i++) {
		
		switch (p_node->values[i]->type) {
			
			case FILE_FIELD_RAW: delete p_node->values[i]->data.raw; break;
			case FILE_FIELD_INT_ARRAY: delete p_node->values[i]->data.int_array_val; break;
			case FILE_FIELD_FLOAT_ARRAY: delete p_node->values[i]->data.float_array_val; break;
			default: {}
		}
		
		
		delete p_node->values[i];
		
	}
	
	for (int i=0;i<p_node->childs.size();i++)
		erase_node( p_node->childs[i] );
	
	
	delete p_node;
}



void TreeContainer::load_node(TreeLoader *p_loader) {
	
	p_loader->goto_root();
	for (int i=0;p_loader->get_child_count();i++) {
	
		p_loader->enter( p_loader->get_child_name(i) );
		enter(p_loader->get_child_name(i));
		
		load_node(p_loader);
		
		p_loader->exit();
		exit();
	}

	for (int i=0;i<p_loader->get_var_count();i++) {
		
		String name=p_loader->get_var_name(i);
		
		switch(p_loader->get_var_type(name)) {
			

			case VAR_INT: {
				
				add_int( name, p_loader->get_int( name ) );		
			} break;	
			case VAR_FLOAT:{
				
				add_float( name, p_loader->get_float( name ) );		
				
			} break;	
			case VAR_STRING: {
				
				add_string( name, p_loader->get_string( name ) );		
				
			} break;	
			case VAR_INT_ARRAY: {
				
				int len=p_loader->get_int_array_len( name );
				int *aux = new int[len];
				p_loader->get_int_array( name, aux);
				add_int_array(name,aux,len);
				delete aux;
				
			} break;	
			case VAR_FLOAT_ARRAY: {
				
				int len=p_loader->get_float_array_len( name );
				float *aux = new float[len];
				p_loader->get_float_array( name, aux);
				add_float_array(name,aux,len);
				delete aux;
				
			} break;	
			case VAR_RAW: {
				
				int len=p_loader->get_raw_len( name );
				unsigned char *aux = new unsigned char[len];
				p_loader->get_raw( name, aux);
				add_raw(name,aux,len);
				
			} break;	
			case VAR_NONE: {
				
				// ?
			} break;	

			
		}
		
		
	}
	
}

void TreeContainer::goto_root() {
	
	current=tree;
}


TreeContainer::TreeContainer(TreeLoader *p_loader) {
	
	
	tree = new Node;
	tree->parent=NULL;
	current=tree;
	
	load_node( p_loader );
	
}

TreeContainer::TreeContainer()  {
	
	tree = new Node;
	tree->parent=NULL;
	current=tree;
	
	
}


TreeContainer::~TreeContainer() {
	
	erase_node( tree );
	
}


}
