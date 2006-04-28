#include "rstring.h"
#include <stdio.h>
#include <wchar.h>
#include <string>
#include <vector>



//#define DEBUG

#ifdef DEBUG
#define DEBUGPRINT(m_val) wprintf(m_val.c_str()); wprintf(L"\n");
#else
#define DEBUGPRINT(m_val)
#endif

template<class T>
T test_string() {

  T popo="Hello";
  DEBUGPRINT(popo);
  popo+=" Pepe ";
  DEBUGPRINT(popo);
  T pipi= " Guh " +popo;
  DEBUGPRINT(pipi);
  pipi=T("Two: ") + pipi;
  DEBUGPRINT(pipi);
  pipi=T("One: ") + T(pipi);
  DEBUGPRINT(pipi);
  
  if (pipi[0]=='O')
     return pipi;
  else 
     return popo;
}


int main() {
	/*
  ReShaked::String own=test_string<ReShaked::String>();
  wprintf(L"Own:\n");
  own+="\n";
  wprintf(own.c_str());
#ifndef DEBUG
  std::string correct=test_string<std::string>();
  wprintf(L"Correct:\n");
  correct+="\n";
  wprintf(L"%s",correct.c_str());
#endif
  
  std::vector<ReShaked::String> string_vec;
  string_vec.push_back("Hoe");
  string_vec.push_back("hie");
  string_vec.push_back("torem");
  string_vec.push_back("pohoi");
  string_vec.erase(string_vec.begin()+2);
  string_vec.insert(string_vec.begin()+1,"toadd!");
  
  */
  ReShaked::String merk="Las chicas del gato me dan una mamada";
  printf("before: %lls\n",merk.c_str());
  merk.replace("g","p");
  merk.replace("mamada","naranja");
  merk.replace("chicas","verduledas");
  
  printf("after: %lls\n",merk.c_str());
  /*
  for (int i=0;i<string_vec.size();i++) {
	  
	  wprintf(L"%i: %lls\n",i,string_vec[i].c_str());	  
  }
  */
  
  return 0;  
}


