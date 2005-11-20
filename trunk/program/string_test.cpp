#include "rstring.h"
#include <stdio.h>
#include <wchar.h>
#include <string>


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
  
  return 0;  
}


