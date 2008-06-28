/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "config_handler.h"
#include "misc_funcs.h"
namespace ReShaked {

void ConfigHandler::set_header_check(String p_header_check) {

	header_check=p_header_check;
}



void ConfigHandler::save_subtree(String p_path,FILE *f, TreeLoader *p_loader) {
	
	/* SAVE VARS */
	for (int i=0;i<p_loader->get_var_count();i++) {
			
			
		String field=p_path;
		String name=p_loader->get_var_name( i );
		switch (p_loader->get_var_type( name ) ) {
							
				
			case TreeLoader::VAR_INT: {
					
				field+="(int)"+name+"=";
				field+=String::num(p_loader->get_int( name ));
			} break;
			case TreeLoader::VAR_FLOAT: {
				field+="(float)"+name+"=";
				
				char faux[100];

				sprintf(faux,"%g",p_loader->get_float(name) );
				field+=faux;
					
			} break;
			case TreeLoader::VAR_STRING: {
				field+="(string)"+name+"=";
				String validated=p_loader->get_string( name );
				validated.replace("\n","\\n");
				validated.replace(";","[scln]");
				field+=validated;
					
					
			} break;
			case TreeLoader::VAR_INT_ARRAY: {
				field+="(int_array)"+name+"=";
				int len=p_loader->get_int_array_len(name);
				int *aux_arr = new int[len];
				p_loader->get_int_array(name,aux_arr);
				for (int i=0;i<len;i++) {
					if (i!=0)
						field+=",";
					field+=String::num(aux_arr[i]);
				}
				delete[] aux_arr;
					
			} break;
			case TreeLoader::VAR_FLOAT_ARRAY: {
				field+="(float_array)"+name+"=";
					
				int len=p_loader->get_float_array_len(name);
				float *aux_arr = new float[len];
				p_loader->get_float_array(name,aux_arr);
				char faux[100];

				for (int i=0;i<len;i++) {
					if (i!=0)
						field+=",";
						
					sprintf(faux,"%g",aux_arr[i]);
					field+=faux;
				}
				delete[] aux_arr;
					
			} break;
			case TreeLoader::VAR_RAW: {
				field+="(raw)"+name+"=";
			
				int len=p_loader->get_raw_len( name);
				unsigned char *aux_arr = new unsigned char[len];
				p_loader->get_raw( name,aux_arr);
					
				const char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
				for (int i=0;i<len;i++) {
					if (i!=0)
						field+=",";
						
					int val=aux_arr[i];
					field+=hex[(val/16)%16];
					field+=hex[val%16];
				}
				delete[] aux_arr;
			
			} break;
			case TreeLoader::VAR_NONE: {
			
				continue;
			} break;

		}
		
		field+="\n";
								
		fwrite(field.ascii().get_data(),field.length(),1,f);
		
			
	}
	
	/* SAVE CHILDS */
	
	for (int i=0;i<p_loader->get_child_count();i++) {
	
		String subsection_name = p_loader->get_child_name(i);
		
		p_loader->enter(subsection_name);
		save_subtree( p_path+"/"+subsection_name, f, p_loader);
		p_loader->exit();
		
		
	}
	
}

bool ConfigHandler::save(String p_filename,TreeLoader *p_loader) {
	
	FILE *f;

	if ( !(f=fopen(p_filename.utf8().get_data(),"wb")) ) return true;

	
	fprintf(f,"[%s]\n\n",header_check.utf8().get_data());
	
	p_loader->goto_root(); 

	for (int i=0;i<p_loader->get_child_count();i++) {
		
		String section_name = p_loader->get_child_name(i);
		
		fprintf(f,"\n\n[%s]\n\n",section_name.utf8().get_data());
		
		p_loader->enter(section_name);
		save_subtree("",f,p_loader);
		p_loader->exit();
	}

	fclose(f);

	return false;
}

void ConfigHandler::remove_trailing_space(std::string *p_string) {

	
	while ((p_string->length()>0) && ((*p_string)[0]==' ')) {

		*p_string=p_string->substr(1,p_string->length()-1);
	}

	while ((p_string->length()>0) && ((*p_string)[p_string->length()-1]==' ')) {

		*p_string=p_string->substr(0,p_string->length()-1);
	}
	
}

void ConfigHandler::add_item(TreeSaver *p_saver,String p_section,String p_key,String p_value) {
	
	//printf("sec: %s key %s data %s\n",p_section.ascii().get_data(),p_key.ascii().get_data(),p_value.ascii().get_data());
	

	ERR_FAIL_COND( p_key.find("(")<0 );
	
	String p_key_path=p_key.substr( 0, p_key.find("(") );
	String p_key_nameandtype=p_key.substr( p_key.find("(")+1, p_key.size()-p_key.find("(")-1 );
		
	
	std::vector<String> key_path=split_str('/',p_key_path);
	
	ERR_FAIL_COND( key_path.size()==0 );
		
	String typeandname=p_key_nameandtype;
	
	ERR_FAIL_COND(typeandname.size()==0);
	
	ERR_FAIL_COND( typeandname.find(")")<0 );
	
	int bclosepos=typeandname.find(")");
	String var_type=typeandname.substr( 0, bclosepos );
	String key_name=typeandname.substr( bclosepos+1, (int)typeandname.length()-bclosepos-1 );
	
	//printf("path size: %i - type: %s - name %s\n",key_path.size(),var_type.ascii().get_data(),key_name.ascii().get_data());
	
	ERR_FAIL_COND(var_type=="" || key_name=="");
	
	p_saver->goto_root();
	
	p_saver->enter(p_section);
	
	for (int i=0;i<key_path.size();i++) {
		
		if (key_path[i]=="")
			continue;
		
		p_saver->enter(key_path[i]);
		
	}
	
	
	
	if (var_type=="int") {
		
		p_saver->add_int( key_name, atoi(p_value.ascii().get_data()));
	} else 	if (var_type=="float") {
		
		p_saver->add_float( key_name, atof(p_value.ascii().get_data()));
	} else 	if (var_type=="string") {
		
		p_value.replace("\\n","\n"); //return to de-validation
		p_value.replace("[scln]",";"); //return to de-validation
		p_saver->add_string( key_name, p_value);
	} else if (var_type=="int_array") {
		
		std::vector<String> values=split_str(',',p_value);
		
		ERR_FAIL_COND(values.size()==0);
		
		int * array = new int[values.size()];
		for (int i=0;i<values.size();i++) {
			
			array[i]=atoi(values[i].ascii().get_data());

		}
		
		p_saver->add_int_array( key_name, array, values.size());
		
		delete[] array;
	} else if (var_type=="float_array") {
		
		std::vector<String> values=split_str(',',p_value);
		
		ERR_FAIL_COND(values.size()==0);
		
		float * array = new float[values.size()];
		for (int i=0;i<values.size();i++) {
			
			array[i]=atof(values[i].ascii().get_data());

		}
		
		p_saver->add_float_array( key_name, array, values.size());
		
		delete[] array;
		
	} else if (var_type=="raw") {
		
		std::vector<String> values=split_str(',',p_value);
		
		ERR_FAIL_COND(values.size()==0);
		
		unsigned char * array = new unsigned char[values.size()];
		for (int i=0;i<values.size();i++) {
			
			if (values[i].size()!=2) {
				
				delete array;
				ERR_FAIL_COND( values[i].size()!=2 );
			}
			
			int val=0;
			
			int a=values[i][0];
			
			if (a>='0' && a<='9') val=(a-'0')<<8;
			else if (a>='A' && a<='F') val=(10+(a-'A'))<<8;
			else { ERR_PRINT("Invalid hex char"); return; }

			a=values[i][1];
			
			if (a>='0' && a<='9') val+=a-'0';
			else if (a>='A' && a<='F') val|=(10+(a-'A'));
			else { ERR_PRINT("Invalid hex char"); return; }
			
			array[i]=(unsigned char)val;

		}
		
		p_saver->add_raw( key_name, array, values.size());
		
		delete[] array;
	}

	//printf("saved at path %s\n",p_saver->get_path().ascii().get_data());
	
}

bool ConfigHandler::load(String p_filename,TreeSaver *p_saver) {

	
	String current_section;
	FILE *f;

	if ( !(f=fopen(p_filename.utf8().get_data(),"rb")) ) return true;

	bool load_ok=true;
	bool header_ok=false;
	while(1) {

		std::string auxstring;
		bool eof=false;
		while (true) {
			
			unsigned char byte;
			fread(&byte,1,1,f);
			if (feof(f)) {
				eof=true;
				break;
			}
			if (byte=='\n')
				break;
			if (byte=='\r')
				continue;
			
			auxstring+=byte;
		}
		if (eof)
			break;


		auxstring=auxstring.substr(0,auxstring.find(";"));

		if (auxstring.length()==0) continue;

		remove_trailing_space(&auxstring);

		if (auxstring[0]=='[') {

			auxstring=auxstring.substr(1,auxstring.length()-2);

			String section=auxstring.c_str();
			
			if (!header_ok) {
				
				if (section!=header_check) {
					load_ok=false;
					break;
				} else {
					header_ok=true;
				}
					
			}
			
			current_section=section;
			
		} else if (auxstring.find("=")<auxstring.length()) {

			std::string key,data;

			key=auxstring.substr(0,auxstring.find("="));

			remove_trailing_space(&key);
			data=auxstring.substr(auxstring.find("=")+1,(auxstring.length()-auxstring.find("="))-1);
			remove_trailing_space(&data);

			
			////printf("got \"%s\":\"%s\"\n",key.c_str(),data.c_str());

			//@TODO CONVERT FROM UTF8!
			add_item(p_saver,current_section,key.c_str(),data.c_str());
		}
	}

	fclose(f);
	
	p_saver->goto_root();
	return !load_ok;
}

ConfigHandler::ConfigHandler(){
}

ConfigHandler::~ConfigHandler(){
}

}