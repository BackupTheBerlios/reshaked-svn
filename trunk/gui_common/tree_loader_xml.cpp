//
// C++ Implementation: tree_loader_xml
//
// Description:
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//



#include "tree_loader_xml.h"

#if 0
bool TreeLoaderXML::find_tag(String &p_tag_name, Uint32& p_tag_data_begin_pos,Uint32& p_tag_data_end_pos,MemVector<TagParameter> &p_paramters,Uint32 p_max_pos,bool& p_is_meta) {

	Uint32 current_pos=f->get_pos();
	if (current_pos>=p_max_pos)
		return true;

	p_paramters.clear();
	p_tag_data_end_pos=0; // means for now, no valid close tag
	/* find opening < , which means a tag exists */
	while (true) {

		Uint8 c = f->get_byte();

		if (c=='<') {
			break;
		}

		current_pos++;
		if (current_pos>=p_max_pos)
			return true;
	}


	/* Get the tag and type */
	bool first_char=true;
	bool tag_found=false;
	bool reading_param_data=false;
	bool no_close_tag=false;
	p_is_meta=false;
	TagParameter current_param;

	String tag;
	Uint8 prev_c=0;
	Uint8 c=0;

	while (true) {

		prev_c=c;
		c = f->get_byte();
		current_pos++;
		if (current_pos>=p_max_pos)
			return true;


		if (first_char) {

			first_char=false;
			// a close tag shouldnt be found in here
			ERR_FAIL_COND_V(c=='/', true );

			if (c=='?') {
				p_is_meta=true;
				no_close_tag=true;
				continue;
			}

		}

		if (c=='>') {
			if (prev_c=='/')
				no_close_tag=true;
			ERR_FAIL_COND_V(p_is_meta && prev_c!='?',true);
			break; //end of tag
		}

		if (!tag_found) {

			if (c==' ') { //end of tag
				tag_found=true;
				p_tag_name=tag;
			} else
				tag+=c;
		} else {

			if (!reading_param_data) {

				if (c==' ' || c=='=') //we start to read data if we find a "
					continue;

				if (c=='"')
					reading_param_data=true;
				else
					current_param.name+=c;
			} else {

				if (c=='"') {
					//finished reading parameter
					reading_param_data=false;
					p_paramters.push_back(current_param);
					current_param.name="";
					current_param.value="";
				} else
					current_param.value+=c;
			}

		}

	}

	/* Ok, got the tag, now search where the closing tag begins... */

	p_tag_data_begin_pos=f->get_pos();


	if (no_close_tag) {
		p_tag_data_end_pos=0;
	}

	int close_tag_stack=0;

	prev_c=0;

	while (!no_close_tag) {

		prev_c=c;
		c = f->get_byte();
		current_pos++;
		if (current_pos>=p_max_pos)
			return true;

		if (prev_c=='<') {

			if (c=='/') {

				if (close_tag_stack==0) {

					//end pos
					p_tag_data_end_pos=f->get_pos()-2;

					String cmp_tag;

					while( (c=f->get_byte())!='>' && !f->eof_reached())
						cmp_tag+=c;

					ERR_FAIL_COND_V(cmp_tag!=tag,true);

					break;

				} else
					close_tag_stack--;
			} else {

				close_tag_stack++;
			}
		}

		if (c=='>' && prev_c=='/')
			close_tag_stack--; //distinguish self closing tags
	}

	//printf("found tag: %s, meta: %s , self %s - data",tag.ascii().get_data(),(p_is_meta?"yes":"no"), (no_close_tag?"yes":"no"));

	for (int i=0;i<p_paramters.size();i++) {

		//printf("  %s = \"%s\"  ",p_paramters[i].name.ascii().get_data(),p_paramters[i].value.ascii().get_data());

	}
	//printf("\n");
	return false;
}


bool TreeLoaderXML::enter(String p_dir) {

	for (int i=0;i<subdirs.size();i++) {

		if (subdirs[i].name==p_dir) {

			path.push_back(subdirs[i]);
			bool error=parse_node(subdirs[i].offset_beg,subdirs[i].offset_end);
			ERR_FAIL_COND_V(error,true);
			return false;
		}
	}

	ERR_PRINT("Unknown dir");
	return true;
}

bool TreeLoaderXML::enter_by_index(int p_index) {

	ERR_FAIL_INDEX_V(p_index,subdirs.size(),true);

	path.push_back(subdirs[p_index]);
	bool error=parse_node(subdirs[p_index].offset_beg,subdirs[p_index].offset_end);
	ERR_FAIL_COND_V(error,true);

	return false;

}


void TreeLoaderXML::exit() {

	if (path.size()<=1)
		return;

	path.resize( path.size() -1 );

	ChildDir c=path[ path.size() -1 ];

	bool error=parse_node(c.offset_beg,c.offset_end);
	ERR_FAIL_COND(error);


}
String TreeLoaderXML::get_path() {

	String pathstr="/";


	for (int i=1;i<path.size();i++) {

		if (i!=1)
			pathstr+="/";
		pathstr+=path[i].name;
	}

	return pathstr;

}
void TreeLoaderXML::goto_root() {

	if (path.size()<=1)
		return;

	path.resize( 1 );

	ChildDir c=path[ path.size() -1 ];

	bool error=parse_node(c.offset_beg,c.offset_end);
	ERR_FAIL_COND(error);

}

String TreeLoaderXML::get_data_string(Uint32 p_from,Uint32 p_to) {

	f->seek(p_from);

	String s;
	for (Uint32 i=0;i<(p_to-p_from);i++)
		s+=f->get_byte();

	return s;
}

TreeLoaderXML::ChildVar * TreeLoaderXML::get_subvar(String p_name) {
	//printf("looking for %s\n", (const char*)p_name.ascii());
	for (int i=0;i<vars.size();i++) {

		if (vars[i].name==p_name) {

			vars.lock();
			aux=vars(i);
			vars.unlock();
			return &aux;
		}
	}

	return NULL;
}

void TreeLoaderXML::setup_array(Uint32 p_beg,int p_from_item,Uint32 p_limit) {


	f->seek(p_beg);
	Uint32 pos=p_beg;

	if (p_from_item == 0) return;

	int skips=0;

	while(true) {

		Uint8 c = f->get_byte();

		if (c==',' ) {

			skips++;
			if (skips==p_from_item)
				return;
		}

		pos++;
		if (pos>=p_limit)
			return;
	}

}

String TreeLoaderXML::read_array_element(Uint32 p_limit) {

	String e;

	Uint32 pos=f->get_pos();

	while(true) {

		Uint8 c = f->get_byte();

		if (c==',' )
			return e;
		e = e + c;
		pos++;
		if (pos>=p_limit)
			return e;

	}

	return "";
}

int TreeLoaderXML::get_int(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,0);

	return get_data_string(v->offset_beg,v->offset_end).to_int();

}
void TreeLoaderXML::get_int_array(String p_name,int *p_arr,int p_from,int p_len) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND(!v);

	if (p_len==0)
		return;

	if (p_len<0) {

		p_len=v->array_len-p_from;
	}

	ERR_FAIL_COND( p_from<0 );
	ERR_FAIL_COND( (p_from+p_len) > v->array_len );

	setup_array(v->offset_beg,p_from,v->offset_end);

	for (int i=p_from;i<(p_from+p_len);i++) {


		String e=read_array_element(v->offset_end);

		p_arr[i-p_from]=e.to_int();
	}


}
int TreeLoaderXML::get_int_array_len(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,-1);

	return v->array_len;
}
float TreeLoaderXML::get_float(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,0.0f);

	return get_data_string(v->offset_beg,v->offset_end).to_double();

}
void TreeLoaderXML::get_float_array(String p_name,float* p_arr,int p_from,int p_len) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND(!v);

	if (p_len==0)
		return;

	if (p_len<0) {

		p_len=v->array_len-p_from;
	}

	ERR_FAIL_COND( p_from<0 );
	ERR_FAIL_COND( (p_from+p_len) > v->array_len );

	setup_array(v->offset_beg,p_from,v->offset_end);

	for (int i=p_from;i<(p_from+p_len);i++) {


		String e=read_array_element(v->offset_end);

		p_arr[i-p_from]=e.to_double();
	}

}
int TreeLoaderXML::get_float_array_len(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,-1);

	return v->array_len;
}
String TreeLoaderXML::get_string(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,"");

	return get_data_string(v->offset_beg,v->offset_end);

}

void TreeLoaderXML::get_string_array(String p_name,String* p_arr,int p_from,int p_len) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND(!v);

	if (p_len==0)
		return;

	if (p_len<0) {

		p_len=v->array_len-p_from;
	}

	ERR_FAIL_COND( p_from<0 );
	ERR_FAIL_COND( (p_from+p_len) > v->array_len );

	setup_array(v->offset_beg,p_from,v->offset_end);

	for (int i=p_from;i<(p_from+p_len);i++) {


		String e=read_array_element(v->offset_end);

		p_arr[i-p_from]=e;
	}

}

int TreeLoaderXML::get_string_array_len(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,-1);

	return v->array_len;

}

void TreeLoaderXML::get_raw(String p_name,unsigned char *p_arr,int p_from,int p_len) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND(!v);

	if (p_len==0)
		return;

	if (p_len<0) {

		p_len=v->array_len-p_from;
	}

	ERR_FAIL_COND( p_from<0 );
	ERR_FAIL_COND( (p_from+p_len) > v->array_len );

	setup_array(v->offset_beg,p_from,v->offset_end);

	for (int i=p_from;i<(p_from+p_len);i++) {


		String e=read_array_element(v->offset_end);

		p_arr[i]=e.to_int();
	}

}
int TreeLoaderXML::get_raw_len(String p_name) {

	ChildVar *v=get_subvar(p_name);
	ERR_FAIL_COND_V(!v,-1);

	return v->array_len;
}

int TreeLoaderXML::get_var_count() {


	return vars.size();
}
String TreeLoaderXML::get_var_name(int i) {

	ERR_FAIL_INDEX_V(i,vars.size(),"");

	return vars [i].name;

}


int TreeLoaderXML::get_child_count() {

	return subdirs.size();
}

String TreeLoaderXML::get_child_name(int i) {

	ERR_FAIL_INDEX_V(i,subdirs.size(),"");

	return subdirs[i].name;

}
bool TreeLoaderXML::is_child(String p_dir) {

	for (int i=0;i<subdirs.size();i++) {

		if (subdirs[i].name==p_dir) {

			return true;
		}
	}

	return false;
}

bool TreeLoaderXML::is_var(String p_var) {

	for (int i=0;i<vars.size();i++) {

		if (vars[i].name==p_var) {

			return true;
		}
	}

	return false;

}



TreeLoader::VarType TreeLoaderXML::get_var_type(String p_var) {

	for (int i=0;i<vars.size();i++) {

		if (vars[i].name==p_var) {

			return vars[i].type;
		}
	}

	return TreeLoader::VAR_NONE;

}

void TreeLoaderXML::skip_tag_end(Uint32 p_limit) {

	Uint32 pos=f->get_pos();

	while (true) {

		Uint8 c=f->get_byte();
		pos++;
		if (c=='>' || pos>=p_limit)
			break;
	}

}

String TreeLoaderXML::get_param_val(MemVector<TagParameter> &parameters, String p_name) {


	for (int i=0;i<parameters.size();i++) {

		if (parameters[i].name==p_name)
			return parameters[i].value;
	}

	return "";
}

bool TreeLoaderXML::parse_node(Uint32 p_from,Uint32 p_to) {

	String tagname;
	Uint32 tag_begin=p_from,tag_end=p_to;
	MemVector<TagParameter> paramters;
	bool meta;

	//printf("Parse from %i - %i\n",p_from,p_to);

	f->seek(p_from);
	subdirs.clear();
	vars.clear();
	while (true) {

		bool notfound=find_tag(tagname,tag_begin,tag_end,paramters, p_to, meta );
		if (notfound)
			return false;

		if (tagname=="dir") {

			//check wether this has the NAME field
			ERR_FAIL_COND_V(get_param_val(paramters,"name")=="",true);

			ChildDir c;
			c.name=get_param_val(paramters,"name");
			c.offset_beg=tag_begin;
			c.offset_end=tag_end;
			subdirs.push_back(c);

		} else if (tagname=="data") {

			ERR_FAIL_COND_V(get_param_val(paramters,"name")=="",true);
			ERR_FAIL_COND_V(get_param_val(paramters,"type")=="",true);

			ChildVar c;
			c.name=get_param_val(paramters,"name");
			c.offset_beg=tag_begin;
			c.offset_end=tag_end;

			String type=get_param_val(paramters,"type");
			if (type=="int") {

				c.type=TreeLoader::VAR_INT;
			} else if (type=="int_array") {

				c.type=TreeLoader::VAR_INT_ARRAY;
				c.array_len=get_param_val(paramters,"len").to_int();

			} else if (type=="float") {

				c.type=TreeLoader::VAR_FLOAT;

			} else if (type=="float_array") {

				c.type=TreeLoader::VAR_FLOAT_ARRAY;
				c.array_len=get_param_val(paramters,"len").to_int();

			} else if (type=="string") {

				c.type=TreeLoader::VAR_STRING;

			} else if (type=="string_array") {

				c.type=TreeLoader::VAR_STRING_ARRAY;
				c.array_len=get_param_val(paramters,"len").to_int();

			} else  if (type=="raw") {

				c.type=TreeLoader::VAR_RAW;
				c.array_len=get_param_val(paramters,"len").to_int();
			} else {

				ERR_PRINT("Unsupported DataType!");
				return true;
			}

			vars.push_back(c);

		} else {

			ERR_PRINT("Unsupported Tag!");
			return true;

		}

		f->seek(tag_end);

		skip_tag_end(p_to);

		tag_begin=f->get_pos();
		tag_end=p_to;

	}
	return false;
}



Error TreeLoaderXML::open(String p_file,String p_header,FileAccess *p_custom) {

	f=p_custom?p_custom:FileAccess::create(); //use default

	if (f->open(p_file.ascii().get_data(),FileAccess::READ) != FileAccess::OK) {

		ERR_PRINT( String("Cannot open file: "+p_file).ascii().get_data() );
		return ERR_CANT_OPEN;
	}

	String tagname;
	Uint32 tag_begin,tag_end;
	MemVector<TagParameter> paramters;
	bool meta;

	f->seek_end();
	Uint32 end=f->get_pos(); //get size of xml file
	f->seek(0);

	while (true) {

		bool error=find_tag(tagname,tag_begin,tag_end,paramters, end, meta );

		if (error) {
			f->close();
			return ERR_INVALID_DATA;
		}

		if (meta) {

			/* should check XML encoding... */

		} else {

			ERR_FAIL_COND_V(tag_end==0, ERR_INVALID_DATA); //invalid format

			if (tagname!="tree") {

				f->close();
				return ERR_INVALID_DATA;

			}

			if (parse_node(tag_begin,tag_end))
				return ERR_INVALID_DATA;

			ChildDir root;
			root.name="root";
			root.offset_beg=tag_begin;
			root.offset_end=tag_end;
			path.push_back(root);

			break;

		}


	}


	return OK;
}
void TreeLoaderXML::close() {

	f->close();
	delete f;
	f=NULL;

}


TreeLoaderXML::TreeLoaderXML() {
	f=NULL;
}


TreeLoaderXML::~TreeLoaderXML() {

	if (f)
		close();
}

#endif
