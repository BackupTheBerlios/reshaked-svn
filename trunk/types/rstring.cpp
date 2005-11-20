//
// C++ Implementation: rstring
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rstring.h"
#include <stdio.h>
#include <wchar.h>
#include "error_macros.h"
#include <math.h>


#define MAX_DIGITS 6

namespace ReShaked {


 
void String::copy_on_write() {
	
	if (shared->refcount==1)
		return; //no need to copy on write!
	
	Shared *old=shared;
	
	create_shared();
	
	shared->len=old->len;
	shared->data=(CharType *)malloc((shared->len+1)*sizeof(CharType));
	for (int i=0;i<shared->len;i++) {
		
		shared->data[i]=old->data[i];		
	}
	
	old->refcount--;
}

void String::resize_shared(int p_newsize) {
	
	shared->data=(CharType*)realloc(shared->data,(p_newsize+1)*sizeof(CharType) );
	shared->len=p_newsize;
	shared->data[shared->len]=0; //append 0 at the end so it's compatible to a cstring
}

void String::create_shared(int p_length) {
	
	shared = new Shared;
	shared->len=p_length;
	shared->data=(CharType*)malloc(sizeof(CharType)*(p_length+1));
	shared->data[p_length]=0; //append 0 at the end so it's compatible to a cstring
	shared->refcount=1;	
	
}

void String::free_shared() {
	
	if (shared->refcount==1) { //only us using it
		free(shared->data);
		delete shared;
		shared=NULL;
	} else {
		
		shared->refcount--;
	}
	
}
	
void String::copy_from(String& p_string) {
	
	if (p_string.shared==shared)
		return; //nothing to copy
	free_shared(); //if we have data, free it
	shared=p_string.shared; // copy the shared data
	shared->refcount++; // Increase refcount in shared
		
}

void String::copy_from(const char *p_cstr) {
	
	int len=0;
	const char *ptr=p_cstr;
	while (*(ptr++)!=0) 
		len++;
	
	if (shared!=NULL)
		free_shared();

	create_shared(len);
	
	for (int i=0;i<len;i++) {
				
		shared->data[i]=p_cstr[i];
	}
	shared->len=len;
	
}
void String::copy_from(const CharType *p_cstr) {
	
	
	int len=0;
	const CharType *ptr=p_cstr;
	while (*(ptr++)!=0) 
		len++;
	
	if (shared!=NULL)
		free_shared();

	create_shared(len);
	
	for (int i=0;i<len;i++) {
				
		shared->data[i]=p_cstr[i];
	}
	shared->len=len;
}

void String::copy_from(const CharType& p_char) {
	
	
	if (shared!=NULL)
		free_shared();
	create_shared(1); //@TODO@ create in a certain size must be allowed
	shared->data[0]=p_char;
	
}

bool String::operator=(String p_str) {
	
	copy_from(p_str);
	return length()>0; //true if not empty
}


bool String::operator==(String p_str) const {
	
	/* speed up comparison */
	if (shared==p_str.shared)
		return true; //no doubt
	if (shared->len!=p_str.shared->len)
		return false; // no need for weird tests
		
	/* Compare char by char */
	for (int i=0;i<shared->len;i++) {
		
		if (shared->data[i]!=p_str.shared->data[i])
			return false;
	}
	
	return true;
}

bool String::operator!=(String p_str) const {
	
	return !(*this==p_str);
}

const String::CharType& String::operator[](int p_idx) const {
	
	static CharType errv=0;
	ERR_FAIL_INDEX_V(p_idx,shared->len,errv);

	return shared->data[p_idx];
}

String::CharType& String::operator[](int p_idx) {
	
	static CharType errv=0;
	errv=0; //dont change it, damnit
	ERR_FAIL_INDEX_V(p_idx,shared->len,errv);
	copy_on_write();
	return shared->data[p_idx];
}

String String::operator+(const String &p_str)  const {
	
	String res=*this;
	res+=p_str;
	return res;
}

String String::operator+(CharType p_chr)  const {
	
	String res=*this;
	res+=p_chr;
	return res;
}
	
String& String::operator+=(const String &p_str) {
	
	//printf("check empty for %lls, len is %i\n",p_str.c_str(),shared->len);	
	if (p_str.empty()) {

		return *this;
	}

	copy_on_write(); /* DATA IS MODIFIED, COPY ON WRITE! */
	
	//printf("1 %lls\n",shared->data);
	

	int old_len=shared->len;
	//printf("2 len is now %i\n",shared->len);
	resize_shared(p_str.shared->len+shared->len);
	//printf("3 len is now %i\n",shared->len);
	//printf("4 adding: ");
	
	for (int i=0;i<p_str.shared->len;i++) {
		
		shared->data[old_len+i]=p_str.shared->data[i];
		//printf("%c ",shared->data[old_len+i]);
		
	}

	//printf("5-result is %lls\n",shared->data);

	return *this;
	
}

String& String::operator+=(const CharType *p_str) {
	
	

	*this+=String(p_str);
	return *this;
}

String&  String::operator+=(CharType p_char) {
	
	

	copy_on_write(); ///< DATA IS MODIFIED, COPY ON WRITE! 
	
	resize_shared(shared->len+1);
	shared->data[shared->len-1]=p_char;
	return *this;
	
}

String& String::operator+=(const char * p_str) {
	
	if (p_str[0]==0)
		return *this;
	
	
	copy_on_write(); ///< DATA IS MODIFIED, COPY ON WRITE! 
	
	
	
	int src_len=0;
	const char *ptr=p_str;
	while (*(ptr++)!=0) 
		src_len++;
	
	
	int old_len=shared->len;
	
	
	resize_shared(src_len+shared->len);
	

	for (int i=0;i<src_len;i++) {
		
		shared->data[old_len+i]=p_str[i];
	}
	
	return *this;
}

void String::operator=(const char *p_str) {
	
	copy_from(p_str);
	
}

void String::operator=(const CharType *p_str) {
	
	copy_from(p_str);

}

bool String::operator=(CharType p_chr) {
	
	copy_from(p_chr);
	return !empty();
}

bool String::operator==(const char *p_str) const {
	
	int len=0;
	const char *aux=p_str;
	
	while (*(aux++)!=0)
		len++;
	
	if (len!=shared->len)
		return false;
	
	for (int i=0;i<len;i++)
		if (p_str[i]!=shared->data[i])
			return false;
	
	return true;
	
	
}

bool String::operator==(const CharType *p_str) const {

	int len=0;
	const CharType *aux=p_str;
	
	while (*(aux++)!=0)
		len++;
	
	if (len!=shared->len)
		return false;
	
	for (int i=0;i<len;i++)
		if (p_str[i]!=shared->data[i])
			return false;
	
	return true;

}

bool String::operator!=(const char *p_str) const {
	
	return (! ( *this==p_str ) );
}
bool String::operator!=(const CharType *p_str) const {
	
	return (! ( *this==p_str ) );
	
}

int String::length() const {
	
	return shared->len;
}
int String::size() const {
	
	return shared->len;
}

bool String::empty() const {
	
	return shared->len==0;
}

const String::CharType * String::c_str() const {
	
	return shared->data;	
}

String String::num(double p_num,int p_digits) {

	String s;
	/* integer part */
	
	bool neg=p_num<0;
	p_num=fabs(p_num);
	int intn=(int)p_num;
	
	if (intn==0)
		
		s="0";
	else {
		while(intn) {
			
			CharType num='0'+(intn%10);
			intn/=10;
			
			//printf("s pre %c\n",num);
			s=num+s;
			
		}
	}
	//printf("int - %lls\n",s.c_str());			
	/* decimal part */

	
	if (p_digits>0 || (p_digits==-1 && (int)p_num!=p_num)) {
		
		s+=".";
		
		double dec=p_num-floor(p_num);
		
		int digit=0;
		if (p_digits>MAX_DIGITS)
			p_digits=MAX_DIGITS;
		
		while (true) {
			
			dec*=10;
			CharType num='0'+(int)dec%10;
			digit++;
			s+=num;
			//printf("s post %c\n",num);
			
			if (p_digits==-1) {
				
				if (digit==MAX_DIGITS) //no point in going to infinite
					break;
				
				if ((dec-floor(dec))<1e-6)
					break;
			}
							
			if (digit==p_digits) 
				break;
			
			
		}
	
	}

	if (neg)
		s="-"+s;
	return s;
	
}

String::String(CharType p_char) {
	
	shared=NULL;
	copy_from(p_char);	
}

String::String(const String &p_string) {
	
	create_shared();
	String &str=(String&)p_string; /* remove Const-ness */
	copy_from(str);	
}

String::String(const char *p_str) {
	
	shared=NULL;
	copy_from(p_str);
}
String::String(const CharType *p_str) {
	
	shared=NULL;
	copy_from(p_str);
}

String::String() {
		
	create_shared();
}

String::~String() {
	
	free_shared();
}
bool operator==(const char*p_chr, const String& p_str) {
	
	return p_str==p_chr;	
}

String operator+(const char*p_chr, const String& p_str) {
	
	String tmp=p_chr;
	tmp+=p_str;
	return tmp;
		
}
String operator+(String::CharType p_chr, const String& p_str) {

	String tmp(p_chr);
	tmp+=p_str;
	return tmp;
	
}


}
