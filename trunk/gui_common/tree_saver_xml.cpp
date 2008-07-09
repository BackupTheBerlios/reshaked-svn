//
// C++ Implementation: tree_saver_xml
//
// Description:
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_saver_xml.h"
#if 0

void TreeSaverXML::tabulate() {

	f->store_byte('\n');
	for (int i=0;i<level;i++)
		f->store_byte('\t');

}

bool TreeSaverXML::valid_name(String p_name) {

	ERR_FAIL_COND_V(p_name.length()>MAX_ENTRY_NAME_LEN,false);
	ERR_FAIL_COND_V(p_name.length()==0,false);


	for (int i=0;i<p_name.size();i++) {

		ERR_FAIL_COND_V(p_name[i]=='/',false);
		ERR_FAIL_COND_V(p_name[i]=='"',false);
		ERR_FAIL_COND_V(p_name[i]=='&',false);
		ERR_FAIL_COND_V(p_name[i]=='\'',false);
		ERR_FAIL_COND_V(p_name[i]=='<',false);
		ERR_FAIL_COND_V(p_name[i]=='>',false);

		ERR_FAIL_COND_V(p_name[i]<32 && p_name[i]>127,false);

	}

	return true;
}

void TreeSaverXML::escape(String& p_str) {

	p_str.replace("&","&amp");
	p_str.replace("<","&gt");
	p_str.replace(">","&lt");
	p_str.replace("'","&apos");
	p_str.replace("\"","&quot");

}

void TreeSaverXML::write_string(String p_str,bool p_escape) {

	/* write an UTF8 string */
	if (p_escape)
		escape(p_str);

	CharString cs=p_str.utf8();
	const char *data=cs.get_data();

	while (*data) {
		f->store_byte(*data);
		data++;
	}

}

void TreeSaverXML::write_tag(String p_tag,bool p_escape) {


	tabulate();
	f->store_byte('<');
	write_string(p_tag,p_escape);
	f->store_byte('>');

}

void TreeSaverXML::enter(String p_dir) {

	tabulate();
	write_tag("dir name=\""+p_dir+"\"",false);
	path_stack.push_front(p_dir);
	level++;


}
void TreeSaverXML::exit() {

	ERR_FAIL_COND(path_stack.empty());

	level--;
	tabulate();
	write_tag("/dir");
	path_stack.erase( path_stack.begin() );

}
String TreeSaverXML::get_path() {

	String path;

	List<String>::Iterator *it=path_stack.begin();


	while (it) {

		if (path!="") {

			path="/"+path;
		}

		path=it->get()+path;
		it=it->next();
	}

	return path;
}
void TreeSaverXML::goto_root() {

	while (!path_stack.empty())
		exit();
}
void TreeSaverXML::add_int(String p_name,int p_int) {

	ERR_FAIL_COND(!valid_name(p_name));
	tabulate();
	write_string("<data type=\"int\" name=\"",false);
	write_string(p_name);
	write_string("\">",false);
	write_string(String::num(p_int));
	write_string("</data>",false);

}
void TreeSaverXML::add_int_array(String p_name,const int *p_arr,int p_len) {

	ERR_FAIL_COND(!valid_name(p_name));

	tabulate();
	write_string("<data type=\"int_array\" name=\"",false);
	write_string(p_name);
	write_string("\" len=\""+String::num(p_len)+"\">",false);
	for (int i=0;i<p_len;i++) {
		if (i>0)
			write_string(",");
		write_string(String::num(p_arr[i]));
	}

	write_string("</data>",false);

}
void TreeSaverXML::add_float(String p_name,float p_float) {

	ERR_FAIL_COND(!valid_name(p_name));

	tabulate();
	write_string("<data type=\"float\" name=\"",false);
	write_string(p_name);
	write_string("\">",false);
	write_string(String::num(p_float));
	write_string("</data>",false);

}
void TreeSaverXML::add_float_array(String p_name,const float *p_arr,int p_len) {

	ERR_FAIL_COND(!valid_name(p_name));

	tabulate();write_string("<data type=\"float_array\" name=\"",false);
	write_string(p_name);
	write_string("\" len=\""+String::num(p_len)+"\">",false);

	for (int i=0;i<p_len;i++) {
		if (i>0)
			write_string(",");
		write_string(String::num(p_arr[i]));
		//printf("write string %s from %f\n", String::num(p_arr[i]).ascii().get_data(), p_arr[i]);
	}

	write_string("</data>",false);

}
void TreeSaverXML::add_string(String p_name,String p_string) {

	ERR_FAIL_COND(!valid_name(p_name));

	tabulate();
	write_string("<data type=\"string\" name=\"",false);
	write_string(p_name);
	write_string("\">",false);
	write_string(p_string);
	write_string("</data>",false);


}
void TreeSaverXML::add_string_array(String p_name,String *p_arr,int p_len) {

	ERR_FAIL_COND(!valid_name(p_name));

	tabulate();
	write_string("<data type=\"string_array\" name=\"",false);
	write_string(p_name);
	write_string("\" len=\""+String::num(p_len)+"\">",false);

	level++;
	for (int i=0;i<p_len;i++) {

		tabulate();
		write_string("<str>");
		write_string(p_arr[i]);
		write_string("</str>");
	}
	level--;

	write_string("</data>",false);

}

void TreeSaverXML::add_raw(String p_name,const unsigned char *p_raw,int p_len) {

	ERR_FAIL_COND(!valid_name(p_name));

	tabulate();
	write_string("<data type=\"raw\" name=\"",false);
	write_string(p_name);
	write_string("\" len=\""+String::num(p_len)+"\">",false);
	for (int i=0;i<p_len;i++) {
		if (i>0)
			write_string(",");
		write_string(String::num(p_raw[i]));
	}

	write_string("</data>",false);

}
bool TreeSaverXML::save(String p_filename,String p_ascii_header,FileAccess *p_custom) {

	ERR_FAIL_COND_V(f,true);

	f=p_custom?p_custom:FileAccess::create();
	own_f=p_custom==NULL;

	f->open(p_filename.ascii().get_data(),FileAccess::WRITE);

	write_string("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n",false); //no escape
	write_string("<tree name=\""+p_ascii_header+"\">",false);


	level=0;

	return false;
}

void TreeSaverXML::close() {

	ERR_FAIL_COND(!f);
	write_string("</tree>",false);

	goto_root();
	f->close();
	if (own_f)
		memdelete(f);
	f=NULL;
}


TreeSaverXML::TreeSaverXML() {

	f=NULL;

	level=0;
}


TreeSaverXML::~TreeSaverXML()
{
	if (f)
		close();
}


#endif