//
// C++ Implementation: tree_saver_disk
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_saver_disk.h"
#include <stdio.h>
namespace ReShaked {


void TreeSaverDisk::write_byte(unsigned char p_byte) {
	
	fwrite(&p_byte,1,1,f);
	
}
void TreeSaverDisk::write_short(unsigned short p_short) {
	
	/* little endian */
	write_byte( p_short & 0xFF ); 
	write_byte( p_short >> 8); 
	
	
}
void TreeSaverDisk::write_int(unsigned int p_int) {
	
	write_short( p_int & 0xFFFF ); 
	write_short( p_int >> 16 ); 
	
	
}
void TreeSaverDisk::write_sint(int p_int) {
	
	unsigned int *unsint=(unsigned int*)&p_int;
	write_int(*unsint);
	
	
	
}
void TreeSaverDisk::write_string(String p_string) {

	write_int(p_string.length());
	for (int i=0;i<p_string.length();i++)
		write_int((unsigned int)p_string[i]);
}
void TreeSaverDisk::write_float(float p_float) {
	
	unsigned int *unsint=(unsigned int*)&p_float;
	write_int(*unsint);
}

void TreeSaverDisk::enter(String p_dir) {
	
	write_byte(FILE_FIELD_ENTER);
	write_string(p_dir);
	path.push_back(p_dir);
	
}
void TreeSaverDisk::exit() {
	
	ERR_FAIL_COND(path.empty());
	write_byte(FILE_FIELD_EXIT);
	path.pop_back();
	
}
String TreeSaverDisk::get_path() {
	
	String res="/";
	foreach(I,path) {
		
		res+=*I+"/";
	}
	
	return res;
}

void TreeSaverDisk::add_int(String p_name,int p_int){
	
	write_byte(FILE_FIELD_INT);
	write_string(p_name);
	write_sint(p_int);
}
void TreeSaverDisk::add_int_array(String p_name,int *p_arr,int p_len){
	
	write_byte(FILE_FIELD_INT_ARRAY);
	write_string(p_name);
	write_int(p_len); //amount of elements
	for (int i=0;i<p_len;i++) {
		
		write_sint(p_arr[i]);
	}
	
}
void TreeSaverDisk::add_float(String p_name,float p_float){
	
	write_byte(FILE_FIELD_FLOAT);
	write_string(p_name);
	write_float(p_float); 
	
}
void TreeSaverDisk::add_float_array(String p_name,float *p_arr,int p_len){
	
	write_byte(FILE_FIELD_FLOAT_ARRAY);
	write_string(p_name);
	write_int(p_len); //amount of elements
	for (int i=0;i<p_len;i++) {
		
		write_float(p_arr[i]);
	}
	
	
}
void TreeSaverDisk::add_string(String p_name,String p_string){
	
	write_byte(FILE_FIELD_STRING);
	write_string(p_name);
	write_string(p_string);
	
}
void TreeSaverDisk::add_raw(String p_name,unsigned char *p_raw,int p_bytes){
	
	write_byte(FILE_FIELD_RAW);
	write_string(p_name);
	write_int(p_bytes); //amount of elements
	for (int i=0;i<p_bytes;i++) 
		write_byte(p_raw[i]);
		
}


bool TreeSaverDisk::open_file(String p_file) {
	
	f = fopen(p_file.utf8().get_data(),"wb");
	
	if (!f) {
		
		f = fopen(p_file.ascii().get_data(),"wb");
		if (f) {
			ERR_PRINT("COUDNT SAVE UTF-8 FILENAME");
		}
	}
	
	return (!f);
}
void TreeSaverDisk::close_file() {
	
	write_byte(FILE_FIELD_EOF);
	fclose(f);
}


TreeSaverDisk::TreeSaverDisk()
{
}


TreeSaverDisk::~TreeSaverDisk()
{
}


}
