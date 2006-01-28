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
				    
				    
				    
class CharString {
					    
	struct Shared {
		
		int refcount;
		char *data;	
	};
	
	Shared *shared;
	void free_shared();
	void take_shared(Shared *p_shared);
	
friend class String;	
	CharString(char *p_data);	
public:	
	
	const CharString& operator=(CharString &p_str);
	const char *get_data();	
	CharString();	
	CharString(const CharString &p_src);
	~CharString();
};

class String {
public:			    
	typedef wchar_t CharType;
private:
			    
	struct Shared {
		
		CharType * data;
		int len;
		int refcount;
		
	};
	
	Shared *shared;
	
	/** 
	 * Creates the shared data, to a fixed length in case it's needed, and a zero always att he end
	 * By default creates empty
	 */
	void create_shared(int p_length=0);
	
	/**
	 * Free shared data. If still being used by other strings, it decreases the refcount, otherwise it deletes the data
	 */
	
	void free_shared();
	
	/**
	 * Copy the data from another string. It shares the data with the other string
	 */
	void copy_from(String& p_string);
	
	/**
	 * Copy the data from something else, the data is not shared as it is not possible
	 */
	
	void copy_from(const char *p_cstr);
	void copy_from(const CharType *p_cstr);
	void copy_from(const CharType& p_char);
	
	/**
	 * Resize the data, copy on write should often be used before
	 */
	
	void resize_shared(int p_newsize);
	
	
	 /**
 	 * Copy on write:
	 * If the current string is going to be EDITED (not replaced)
	 * this class creates a local copy of the shared string data (in case it is used by many strings, otherwise it's not touched)
 */
	void copy_on_write(); //copy on write
	
public:
	
	/* Regular Operators */
	bool operator=(String p_str);
	bool operator=(CharType p_chr);
	bool operator==(String p_str) const;
	bool operator!=(String p_str) const;
	String operator+(const String &) const;
	String operator+(CharType p_char) const;
	
	String& operator+=(const String &);
	String& operator+=(CharType p_str);
	String& operator+=(const char * p_str);
	String& operator+=(const CharType * p_str);
	
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
	bool empty() const;
	
	static String num(double p_num,int p_digits=-1);
	
	String left(int p_chars);
	
	CharString ascii(bool p_allow_extended=false);
	CharString utf8();
	/**
	 * The constructors must not depend on other overloads
	 */
	String();
	String(CharType p_char);
	String(const char *p_str);
	String(const CharType *p_str);
	String(const String &p_string);
	~String();

};


bool operator==(const char*p_chr, const String& p_str);

String operator+(const char*p_chr, const String& p_str);
String operator+(String::CharType p_chr, const String& p_str);




} /* end of namespace */

#endif
