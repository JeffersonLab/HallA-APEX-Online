#ifndef ROOT_TriVarDef
#define ROOT_TriVarDef

//////////////////////////////////////////////////////////////////////////
//
// TriVarDef
//
// Structures for calls to THaVarList::DefineVariables() and derived
// functions.
// 
// Also defines structure for THaAnalysisModule::LoadDB().
//
//////////////////////////////////////////////////////////////////////////

#include "VarType.h"


struct TagDef {
  const char*      name;     // Key name
  void*            var;      // Destination of result (default to Double*)
  Int_t            fatal;    // optional: Error to return if tag not found (0=ignore)
  UInt_t           expected; // optional: number of elements to write/read (0/1 scaler)
  VarType          type;     // optional(default kDouble): data type (kInt, etc.)
};


#endif
