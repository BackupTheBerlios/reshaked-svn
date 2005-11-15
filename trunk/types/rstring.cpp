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

void String::create_shared() {
	
	shared = new Shared;
	shared->len=0;
	shared->data=(CharType*)malloc(sizeof(CharType));
	shared->data[0]=0; //append 0 at the end so it's compatible to a cstring
	shared->refcount=1;	
	
}

void String::free_shared() {
	
	if (shared->refcount==1) { //only us using it
		free(shared->data);
		delete shared;
	} else {
		
		shared->refcount--;
	}
	
}
	
void String::copy_from(String& p_string) {
	
	if (p_string.shared==shared)
		return; //nothing to copy
	free_shared();
	shared=p_string.shared;
	shared->refcount++;
		
}


bool String::operator=(String p_str) {
	
	copy_from(p_str);
	return *this!="";	
}


bool String::operator==(String p_str) const {
	
	if (shared==p_str.shared)
		return true; //no doubt
	if (shared->len!=p_str.shared->len)
		return false; // no need for weird tests
		
	for (int i=0;i<shared->len;i++) {
		
		if (shared->data[i]!=p_str.shared->data[i])
			return false;
	}
	
	return true;
}

bool String::operator!=(String p_str) const {
	
	if (shared==p_str.shared)
		return false; //no doubt
	if (shared->len!=p_str.shared->len)
		return true; // no need for weird tests
		
	for (int i=0;i<shared->len;i++) {
		
		if (shared->data[i]!=p_str.shared->data[i])
			return true;
	}
	
	return false;
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

String String::operator+(String p_str) {
	
	copy_on_write();
	int old_len=shared->len;
	resize_shared(p_str.shared->len+shared->len);
	for (int i=0;i<p_str.shared->len;i++) {
		
		shared->data[old_len+i]=p_str.shared->data[i];
	}
	return *this;
}

String String::operator+(CharType p_chr) {
	
	copy_on_write();
	int old_len=shared->len;
	resize_shared(shared->len+1);
	shared->data[old_len]=p_chr;
	return *this;
}

String String::operator+=(String p_str) {
	
	*this = *this + p_str;
	return *this;
}
String String::operator+=(CharType p_char) {
	
	*this = *this + p_char;
	return *this;
	
}

void String::operator=(const char *p_str) {
	
	copy_on_write();
	int len=0;
	const char *aux=p_str;
	
	while (*(aux++)!=0)
		len++;
	
	resize_shared(len);
	
	for (int i=0;i<len;i++)
		shared->data[i]=p_str[i];
	
}

void String::operator=(const CharType *p_str) {
	
	if (*this==p_str)
		return; //avoid unnecesary copy
	
	copy_on_write();
	
	int len=0;
	const CharType *aux=p_str;
	
	while (*(aux++)!=0)
		len++;
	
	resize_shared(len);
	
	for (int i=0;i<len;i++)
		shared->data[i]=p_str[i];
	
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

const String::CharType * String::c_str() const {
	
	return shared->data;	
}
String::String(double p_fromnum) {
	
	create_shared();
	
	/* only works with wchar_t! */
	const int maxlen=40;
	CharType buff[maxlen+1];
	swprintf(buff,maxlen,L"%g",p_fromnum);
	*this=buff;
}

String::String(const String &p_string) {
	
	create_shared();
	String &str=(String&)p_string; //blehness
	copy_from(str);	
}

String::String(const char *p_str) {
	
	create_shared();
	*this=p_str;
}
String::String(const CharType *p_str) {
	
	create_shared();
	*this=p_str;
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


}
