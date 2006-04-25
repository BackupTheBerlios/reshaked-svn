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

namespace ReShaked {

void ConfigHandler::set_header_check(String p_header_check) {

	header_check=p_header_check;
}



void ConfigHandler::save_subtree(String p_path,FILE *f, TreeLoader *p_loader) {
	
	for (int i=0;i<p_loader->get_child_count();i++) {
	
		String subsection_name = p_loader->get_child_name(i);
		
		p_loader->enter(subsection_name);
		save_subtree( "/"+subsection_name, f, p_loader);
		p_loader->exit();
		
		
	}
	
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
					
					
			} break;
			case TreeLoader::VAR_STRING: {
				field+="(string)"+name+"=";
				field+=p_loader->get_string( name ); //should validate
					
					
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

void ConfigHandler::remove_trailing_space(String *p_String) {

	/*
	while ((p_String->length()>0) && ((*p_String)[0]==' ')) {

		*p_String=p_String->substr(1,p_String->length()-1);
	}

	while ((p_String->length()>0) && ((*p_String)[p_String->length()-1]==' ')) {

		*p_String=p_String->substr(0,p_String->length()-1);
	}
	*/
}

bool ConfigHandler::load(String p_filename,TreeSaver *p_saver) {

	/*
	String section="";

	FILE *f;

	if ( !(f=fopen(p_filename.c_str(),"rb")) ) return false;

	while(1) {

		String auxString;
		char axloadzone[500];

		fscanf(f," %[^\r\n]",axloadzone);

		if (feof(f)) break;

		auxString=axloadzone;

		auxString=auxString.substr(0,auxString.find(";"));

		if (auxString.length()==0) continue;

		remove_trailing_space(&auxString);

		if (auxString[0]=='[') {

			auxString=auxString.substr(1,auxString.length()-2);

			section=auxString;
			////printf("got section %s",section.c_str());
			add_section(section);
		} else if (auxString.find("=")<auxString.length()) {

			String key,data;

			key=auxString.substr(0,auxString.find("="));

			remove_trailing_space(&key);
			data=auxString.substr(auxString.find("=")+1,(auxString.length()-auxString.find("="))-1);

			remove_trailing_space(&data);

			////printf("got \"%s\":\"%s\"\n",key.c_str(),data.c_str());

			add_property(section,key,data);
		}
	}

	fclose(f);
	*/
	return true;
}

ConfigHandler::ConfigHandler(){
}

ConfigHandler::~ConfigHandler(){
}

}