
//
// C++ Implementation: tree_loader_file
//
// Description:
//
//
// Author: Juan Linietsky <red@lunatea>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree_loader_file.h"
#include "error_macros.h"

static inline uint32_t hash_djb2(const char *p_cstr) {

	const unsigned char* chr=(const unsigned char*)p_cstr;
	uint32_t hash = 5381;
	uint32_t c;
	
	while ((c = *chr++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}


#define LOADER_FILE_VERSION 1

bool TreeLoaderFile::enter_offset(uint32_t p_offset) {

	f->seek(p_offset);

	uint8_t type = f->get_8();
//	printf("Type is %i\n",type);
	ERR_FAIL_COND_V( type != FILE_FIELD_ENTER, true );

	uint32_t namesize=f->get_32();

	ERR_FAIL_COND_V( namesize>MAX_ENTRY_NAME_LEN, true );

//	printf("name len %i\n",namesize);

	f->seek( f->get_pos()+namesize );

	uint32_t new_hash_table_size=f->get_32();

//	printf("hash table size: %i\n",new_hash_table_size);
	if (new_hash_table_size==0) {
		hash_table.resize(0); //no hashtable
	} else if (new_hash_table_size>MAX_HASHTABLE_SIZE) {
		hash_table.resize(0); //not using hash table
		f->seek( f->get_pos() + new_hash_table_size*8 ); //skip over the hash table
	} else {
		hash_table.resize( new_hash_table_size );

		for (int i=0;i<hash_table.size();i++) {

			HashItem hi;
			hi.hash = f->get_32();
			hi.offset = f->get_32();
			hash_table[i]=hi;
		}
	}

	current.dir_count=f->get_32();
	current.data_count=f->get_32();
	current.dir_offsets_offset=f->get_pos();
	current.data_offsets_offset=current.dir_offsets_offset+current.dir_count*4;

	return false;

}


bool TreeLoaderFile::is_offset_name_and_type( uint32_t p_ofs, String p_name,bool p_check_type,FileFieldType p_type ) {


	f->seek( p_ofs );
	uint8_t t=f->get_8();
	if (p_check_type && t!=p_type)
		return false;

	uint32_t name_len=f->get_32();

/*
	uint32_t auxp = f->get_pos();

	printf("COMPARE: ");
	for (int i=0;i<(int)name_len;i++) {

		printf("%c",f->get_8());
	}

	printf(" with %s\n",p_name.ascii().get_data());

	f->seek( auxp );
*/
	if (name_len!=(int)p_name.length())
		return false;

	for (int i=0;i<(int)name_len;i++) {

		if (f->get_8()!=p_name[i])
			return false;
	}

	return true;

}

#define OFFSET_ERROR 0xFFFFFFFF

bool TreeLoaderFile::enter_data( String p_name, FileFieldType p_type ) {

	uint32_t ofs=get_node_offset( p_name, true, p_type);

	ERR_FAIL_COND_V(ofs==OFFSET_ERROR,true);

	f->seek( ofs );

	uint8_t t=f->get_8(); //type
	ERR_FAIL_COND_V(t!=p_type,true);


	uint32_t name_len=f->get_32();
	f->seek( f->get_pos() + name_len ); //skip data

	return false;

}


uint32_t TreeLoaderFile::get_node_offset(String p_name, bool p_check_type, FileFieldType p_type) {

	ERR_FAIL_COND_V(p_name.length()>MAX_ENTRY_NAME_LEN,OFFSET_ERROR);

	char src_string[MAX_ENTRY_NAME_LEN+1];

	for (int i=0;i<p_name.length();i++)
		src_string[i]=p_name[i];

	src_string[p_name.length()]=0;



	if (  hash_table.size() && hash_type!=HASH_NONE) {
		//try with hash table
		uint32_t hash;

		switch(hash_type) {
			case HASH_DJB2: {
				hash = hash_djb2(src_string);
			} break;
			default: {
				ERR_PRINT("Uknown hash type");
				return OFFSET_ERROR;
			} break;
		}

		int idx=-1;
		/* Linear search for now, will do binary later */
		for (int i=0;i<hash_table.size();i++) {

			if (hash_table[i].hash==hash) {
				idx=i;
				break;
			}
		}

		if (idx==-1) {

			return OFFSET_ERROR; //shouldnt not report error, since is_var uses it
			//printf("key %s not found\n",src_string);
			//ERR_FAIL_COND_V( idx==-1, OFFSET_ERROR );
		}

		while (true) {

			if (is_offset_name_and_type( hash_table[idx].offset, p_name, p_check_type,p_type )) {
				uint32_t offset=hash_table[idx].offset;

				return offset;
			}

			idx++;

			if (idx>=hash_table.size()) {

				//already got to the end of the hash table!
				ERR_FAIL_COND_V( idx>=hash_table.size(), OFFSET_ERROR );
			}

			if (hash_table[idx].hash!=hash) {

				// hash changed, so it means our search simply collided, but did nto
				// match
				ERR_FAIL_COND_V( hash_table[idx].hash!=hash, OFFSET_ERROR );
			}
		}

	//	ERR_PRINT("Hashcode not found in offset table");
		return OFFSET_ERROR;
	} else {
		// try one by one, this sucks, and is very slow...
		//printf("Dir count %i, data count %i\n",current.dir_count,current.data_count);
		for (int i=0;i<(int)( current.dir_count+current.data_count );i++) {

			f->seek( current.dir_offsets_offset+i*4 );
			uint32_t offset=f->get_32();

			if (is_offset_name_and_type( offset, p_name, p_check_type,p_type )) {
				return offset;
			}

		}

	//	ERR_PRINT("Offset not found in offset table");
		return OFFSET_ERROR;
	}
}

bool TreeLoaderFile::enter(String p_dir) {

	uint32_t ofs = get_node_offset(p_dir,true,FILE_FIELD_ENTER);
	ERR_FAIL_COND_V( ofs == OFFSET_ERROR , true );

	if (!enter_offset(ofs)) {
		//enter ok, add to stack
		current.stack.push_back(ofs);

	}
	return false;
}
bool TreeLoaderFile::enter_by_index(int p_index) {

	ERR_FAIL_INDEX_V( p_index, current.dir_count, true );

	f->seek( current.dir_offsets_offset+p_index*4 );
	uint32_t offset=f->get_32();

	if (!enter_offset( offset )) {

		current.stack.push_back(offset);
	}

	return false;
}
void TreeLoaderFile::exit() {

	ERR_FAIL_COND( current.stack.size() == 1 );

	uint32_t offset=current.stack[ current.stack.size() -2 ];
	if (!enter_offset( offset )) {

		current.stack.resize( current.stack.size() -1 );
	}
}
String TreeLoaderFile::get_path() {

	String path;

	for (int i=0;i<current.stack.size();i++) {

		f->seek( current.stack[i] );

		uint8_t type = f->get_8();
		ERR_FAIL_COND_V( type != FILE_FIELD_ENTER, "" );

		uint32_t namesize=f->get_32();

		ERR_FAIL_COND_V( namesize>MAX_ENTRY_NAME_LEN, "" );

		path+="/";
		for (unsigned int i=0;i<namesize;i++)
			path+=f->get_8();
	}

	return path;
}
void TreeLoaderFile::goto_root() {

	ERR_FAIL_COND( current.stack.size() < 1 );

	uint32_t offset=current.stack[ 0 ];

	if (!enter_offset( offset )) {

		current.stack.resize( 1 );
	}

}

int TreeLoaderFile::get_int(String p_name) {

	if (enter_data( p_name, FILE_FIELD_INT )) {

		ERR_PRINT( String( "Can't enter int: " + p_name).ascii().get_data() );
		return 0;
	}

	uint32_t v = f->get_32();

	return (int32_t)v;

}
void TreeLoaderFile::get_int_array(String p_name,int *p_arr,int p_from,int p_len) {

	if (enter_data( p_name, FILE_FIELD_INT_ARRAY )) {

		ERR_PRINT( String( "Can't enter int: " + p_name).ascii().get_data() );
		return;
	}

	uint32_t len = f->get_32();

	if (p_len==-1)
		p_len=len-p_from;

	ERR_FAIL_COND( p_from<0 || (unsigned int)(p_from+p_len)>len);

	if (p_from>0)
		f->seek( f->get_pos() + p_from * 4 );

	for (int i=0;i<p_len;i++)
		p_arr[i]=(int32_t)f->get_32();

}
int TreeLoaderFile::get_int_array_len(String p_name) {

	if (enter_data( p_name, FILE_FIELD_INT_ARRAY )) {

		ERR_PRINT( String( "Can't enter int_array: " + p_name).ascii().get_data() );
		return 0;
	}

	uint32_t len = f->get_32();

	return len;
}

union FloatIntUnion {

	float f;
	uint32_t i;
};
float TreeLoaderFile::get_float(String p_name) {

	if (enter_data( p_name, FILE_FIELD_FLOAT )) {

		ERR_PRINT( String( "Can't enter float: " + p_name).ascii().get_data() );
		return 0;
	}

	FloatIntUnion fu;
	fu.i = f->get_32();

#ifdef INVERTED_FLOAT_ENDIANNESS
	fu.i=BSWAP32(fu.i);
#endif
	return fu.f;

}
void TreeLoaderFile::get_float_array(String p_name,float* p_array,int p_from,int p_len) {

	if (enter_data( p_name, FILE_FIELD_FLOAT_ARRAY )) {

		ERR_PRINT( String( "Can't enter float_array: " + p_name).ascii().get_data() );
		return;
	}

	uint32_t len = f->get_32();

	if (p_len==-1)
		p_len=len-p_from;

	ERR_FAIL_COND( p_from<0 || (p_from+p_len)>len);

	if (p_from>0)
		f->seek( f->get_pos() + p_from * 4 );

	for (int i=0;i<p_len;i++)  {

		FloatIntUnion fu;
		fu.i = f->get_32();

#ifdef INVERTED_FLOAT_ENDIANNESS
		fu.i=BSWAP32(fu.i);
#endif

		p_array[i]=fu.f;
	}

}
int TreeLoaderFile::get_float_array_len(String p_name) {

	if (!enter_data( p_name, FILE_FIELD_FLOAT_ARRAY )) {

		ERR_PRINT( String( "Can't enter float_array: " + p_name).ascii().get_data() );
		return 0;
	}

	uint32_t len = f->get_32();

	return len;

}
String TreeLoaderFile::get_string(String p_name) {

	if (enter_data( p_name, FILE_FIELD_STRING )) {

		ERR_PRINT( String( "Can't enter string: " + p_name).ascii().get_data() );
		return "";
	}

	uint32_t len = f->get_32();

	String s;

	for (int i=0;i<len;i++) {

		String::CharType c;

		switch (string_type) {

			case TYPE_UCS32: {

				c=f->get_32();
			} break;
			case TYPE_UCS16: {

				c=f->get_16();

			} break;
			case TYPE_UTF8:
			case TYPE_ASCII: {

				c=f->get_8();
			} break;
		}

		s+=c;
	}

	if (string_type==TYPE_UTF8) {

		String str;
		str.parse_utf8(s.ascii(true).get_data());
		return str;
	}

	return s;

}
void TreeLoaderFile::get_string_array(String p_name,String*p_array,int p_from,int p_len) {

	if (enter_data( p_name, FILE_FIELD_STRING_ARRAY )) {

		ERR_PRINT( String( "Can't enter string_array: " + p_name).ascii().get_data() );
		return;
	}

	uint32_t count=f->get_16();

	if (p_len==-1)
		p_len=count-p_from;

	ERR_FAIL_COND( p_from<0 || (p_from+p_len)>count);

	if (p_from>0) {

		for (int i=0;i<p_from;i++) {

			uint32_t len = f->get_32();

			switch (string_type) {

				case TYPE_UCS32: {

					f->seek(f->get_pos()+len*4);
				} break;
				case TYPE_UCS16: {

					f->seek(f->get_pos()+len*2);

				} break;
				case TYPE_UTF8:
				case TYPE_ASCII: {
					f->seek(f->get_pos()+len);
				} break;
			}
		}
	}


	for (int j=0;j<count;j++) {

		uint32_t len = f->get_32();

		String s;

		for (int i=0;i<len;i++) {

			String::CharType c;

			switch (string_type) {

				case TYPE_UCS32: {

					c=f->get_32();
				} break;
				case TYPE_UCS16: {

					c=f->get_16();

				} break;
				case TYPE_UTF8:
					case TYPE_ASCII: {

						c=f->get_8();
					} break;
				default: {
					ERR_PRINT("Unknown encoding");
					return;
				} break;

			}

			s+=c;
		}

		if (string_type==TYPE_UTF8) {

			String str;
			str.parse_utf8( s.ascii(true).get_data() );
			p_array[j]=str;
		} else {

			p_array[j]=s;
		};
	}

}
int TreeLoaderFile::get_string_array_len(String p_name) {


	if (enter_data( p_name, FILE_FIELD_STRING_ARRAY )) {

		ERR_PRINT( String( "Can't enter string_array: " + p_name).ascii().get_data() );
		return 0;
	}

	uint32_t len = f->get_32();

	return len;

}

void TreeLoaderFile::get_raw(String p_name,unsigned char *p_raw,int p_from,int p_len)  {

	if (enter_data( p_name, FILE_FIELD_RAW )) {

		ERR_PRINT( String( "Can't enter raw: " + p_name).ascii().get_data() );
		return;
	}

	uint32_t len = f->get_32();

	if (p_len==-1)
		p_len=len-p_from;

	ERR_FAIL_COND( p_from<0 || (unsigned int)(p_from+p_len)>len);

	if (p_from>0)
		f->seek( f->get_pos() + p_from );

	f->get_buffer(p_raw, p_len );

}
int TreeLoaderFile::get_raw_len(String p_name)  {


	if (enter_data( p_name, FILE_FIELD_RAW )) {

		ERR_PRINT( String( "Can't enter raw: " + p_name).ascii().get_data() );
		return 0;
	}

	uint32_t len = f->get_32();

	return len;
}

int TreeLoaderFile::get_var_count()  {

	return current.data_count;
}
String TreeLoaderFile::get_var_name(int i)  {

	ERR_FAIL_INDEX_V( i, current.data_count, true );

	f->seek( current.data_offsets_offset+i*4 );
	uint32_t offset=f->get_32();

	f->seek( offset+1 );
	uint32_t name_len=f->get_32();

	String s;

	for (int i=0;i<name_len;i++)
		s+=f->get_8();

	return s;


}

int TreeLoaderFile::get_child_count()  {

	return current.dir_count;

}
String TreeLoaderFile::get_child_name(int i)  {

	ERR_FAIL_INDEX_V( i, current.dir_count, true );

	f->seek( current.dir_offsets_offset+i*4 );
	uint32_t offset=f->get_32();

	f->seek( offset+1 );
	uint32_t name_len=f->get_32();

	String s;

	for (int i=0;i<name_len;i++)
		s+=f->get_8();

	return s;

}
bool TreeLoaderFile::is_child(String p_dir)  {

	return get_node_offset(p_dir, true, FILE_FIELD_ENTER)!=OFFSET_ERROR;

}

bool TreeLoaderFile::is_var(String p_var)  {

	uint32_t ofs=get_node_offset(p_var, false, FILE_FIELD_ENTER);
	if (ofs==OFFSET_ERROR)
		return false;

	f->seek(ofs);

	uint8_t b=f->get_8();

	switch (b) {

		case FILE_FIELD_INT:
		case FILE_FIELD_INT_ARRAY:
		case FILE_FIELD_FLOAT:
		case FILE_FIELD_FLOAT_ARRAY:
		case FILE_FIELD_STRING:
		case FILE_FIELD_STRING_ARRAY:
		case FILE_FIELD_RAW: {

			return true;
		} break;
		default: {}
	}

	return false;
}

TreeLoader::VarType TreeLoaderFile::get_var_type(String p_var) {

	uint32_t ofs=get_node_offset(p_var, false, FILE_FIELD_ENTER);

	ERR_FAIL_COND_V(ofs==OFFSET_ERROR,VAR_NONE);

	f->seek(ofs);

	uint8_t b=f->get_8();

	switch (b) {

		case FILE_FIELD_ENTER: {

			ERR_PRINT("Variable is a dir");
			return VAR_NONE;
		} break;
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
		case FILE_FIELD_STRING_ARRAY: {

			return VAR_STRING_ARRAY;
		} break;
		case FILE_FIELD_RAW: {

			return VAR_RAW;
		} break;
		default: {}
	}

	ERR_PRINT("Invalid variable type");
	return VAR_NONE;
}

Error TreeLoaderFile::open(String p_file,String p_header,GUI::File *p_custom) {

	ERR_FAIL_COND_V(f,ERR_ALREADY_IN_USE); // loader in use

	if (p_custom) {
		f=p_custom;
	} else {
		f=GUI::File::create(); //use default
	}

	if (f->open(p_file.ascii().get_data(),GUI::File::READ)) {


		ERR_PRINT("Can't open file for reading!\n");
		return ERR_CANT_OPEN;
	}

	char magic[4];
	f->get_buffer( (uint8_t*)magic, 4 );

	if (magic[0]!='C' || magic[1]!='N' || magic[2]!='X' || magic[3]!='T') {

		ERR_PRINT("Not a CNX Tree");
		return ERR_INVALID_DATA;
	}

	uint32_t header_len=f->get_32();

	String id;
	for (int i=0;i<header_len;i++) {

		id+=f->get_8();
	}

	if (p_header!="" && id!=p_header) {

		ERR_PRINT("Header Mismatch");
		return ERR_INVALID_DATA;
	}

	uint32_t version=f->get_32();

	if (version>LOADER_FILE_VERSION) {

		ERR_PRINT("Incompatible tree version");
		return ERR_UNAVAILABLE;
	}

	char encoding[5];

	f->get_buffer((uint8_t*)encoding,5);

	if (	encoding[0]=='U' &&
		encoding[1]=='T' &&
		encoding[2]=='F' &&
		encoding[3]=='-' &&
		encoding[4]=='8' ) {

		string_type=TYPE_UTF8;

	} else
	if (
	    	encoding[0]=='U' &&
		encoding[1]=='C' &&
		encoding[2]=='S' &&
		encoding[3]=='3' &&
		encoding[4]=='2' ) {

		string_type=TYPE_UCS32;
	} else
	if (
	    	encoding[0]=='U' &&
		encoding[1]=='C' &&
		encoding[2]=='S' &&
		encoding[3]=='1' &&
		encoding[4]=='6' ) {

		string_type=TYPE_UCS16;
	} else
	if (
	    	encoding[0]=='A' &&
		encoding[1]=='S' &&
		encoding[2]=='C' &&
		encoding[3]=='I' &&
		encoding[4]=='I' ) {

		string_type=TYPE_ASCII;
	} else {

		ERR_PRINT("Invalid Encoding!!");
		return ERR_INVALID_DATA;
	}

	char endian[2];

	f->get_buffer((uint8_t*)endian,2);

	if (endian[0]=='L' && endian[1]=='E') {

		f->set_endian_swap( false );

	} else if (endian[0]=='B' && endian[1]=='E') {

		f->set_endian_swap( true );
	} else {

		ERR_PRINT("Invalid Endian Mode");
		return ERR_INVALID_DATA;
	}

	char hash_str[4];

	f->get_buffer( (uint8_t*)hash_str,4);

	if (hash_str[0]=='D' && hash_str[1]=='J' && hash_str[2]=='B' && hash_str[3]=='2') {

		hash_type=HASH_DJB2;
	} else if (hash_str[0]=='N' && hash_str[1]=='O' && hash_str[2]=='N' && hash_str[3]=='E') {

		hash_type=HASH_NONE;
	} else {

		ERR_PRINT("Invalid Hash Function");
		return ERR_INVALID_DATA;

	}

	max_entry_len=f->get_32();

	uint32_t root_offset=f->get_32();

	enter_offset( root_offset );

	return OK;
}


void TreeLoaderFile::close() {


	if (f) {

		f->close();
		delete f;
	} else {

		ERR_PRINT("File already closed");
	}

}

TreeLoaderFile::TreeLoaderFile() {

	f=NULL;

	current.dir_count=0;
	current.data_count=0;
	current.dir_offsets_offset=0;
	current.data_offsets_offset=0;

}


TreeLoaderFile::~TreeLoaderFile() {

	if (f)
		close();
}


