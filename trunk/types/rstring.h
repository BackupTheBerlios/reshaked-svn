//
// C++ Interface: rstring
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDRSTRING_H
#define RESHAKEDRSTRING_H

namespace ReShaked {

/**
	@author red <red@killy>
*/
class String {
			    
	typedef wchar_t CharType;
			    
	struct Shared {
		
		CharType * data;
		int len;
		int refcount;
		
	};
	
	Shared *shared;
	
	void create_shared();
	void free_shared();
	void resize_shared(int p_newsize);
	
	void copy_on_write(); //copy on write
	
	void create_shared(int p_len);
				    
	void copy_from(String& p_string);
	
public:
	
	/* Regular Operators */
	bool operator=(String p_str);
	bool operator==(String p_str) const;
	bool operator!=(String p_str) const;
	String operator+(String p_str);
	String operator+(CharType p_char);
	
	String operator+=(String p_str);
	String operator+=(CharType p_str);
	
	/* Compatibility Operators */
	
	void operator=(const char *p_str);
	void operator=(const CharType *p_str);
	bool operator==(const char *p_str) const;
	bool operator==(const CharType *p_str) const;
	bool operator!=(const char *p_str) const;
	bool operator!=(const CharType *p_str) const;
	
	/* [] op */	
	const CharType& operator[](int p_idx) const; //constref
	CharType& operator[](int p_idx); //assignment
	
	const CharType * c_str() const;
	/* standard size stuff */
	
	int length() const;
	int size() const;
	
	String();
	String(double p_fromnum);
	String(const char *p_str);
	String(const CharType *p_str);
	String(const String &p_string);
	~String();

};


bool operator==(const char*p_chr, const String& p_str);
}

#endif
