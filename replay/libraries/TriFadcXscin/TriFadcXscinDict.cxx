// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TriFadcXscinDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TriFadcXscin.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TriFadcXscin(void *p = 0);
   static void *newArray_TriFadcXscin(Long_t size, void *p);
   static void delete_TriFadcXscin(void *p);
   static void deleteArray_TriFadcXscin(void *p);
   static void destruct_TriFadcXscin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcXscin*)
   {
      ::TriFadcXscin *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriFadcXscin >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcXscin", ::TriFadcXscin::Class_Version(), "TriFadcXscin.h", 15,
                  typeid(::TriFadcXscin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriFadcXscin::Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcXscin) );
      instance.SetNew(&new_TriFadcXscin);
      instance.SetNewArray(&newArray_TriFadcXscin);
      instance.SetDelete(&delete_TriFadcXscin);
      instance.SetDeleteArray(&deleteArray_TriFadcXscin);
      instance.SetDestructor(&destruct_TriFadcXscin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcXscin*)
   {
      return GenerateInitInstanceLocal((::TriFadcXscin*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TriFadcXscin*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TriFadcXscin::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriFadcXscin::Class_Name()
{
   return "TriFadcXscin";
}

//______________________________________________________________________________
const char *TriFadcXscin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcXscin*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriFadcXscin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcXscin*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriFadcXscin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcXscin*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriFadcXscin::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcXscin*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TriFadcXscin::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriFadcXscin.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriFadcXscin::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriFadcXscin::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TriFadcXscin(void *p) {
      return  p ? new(p) ::TriFadcXscin : new ::TriFadcXscin;
   }
   static void *newArray_TriFadcXscin(Long_t nElements, void *p) {
      return p ? new(p) ::TriFadcXscin[nElements] : new ::TriFadcXscin[nElements];
   }
   // Wrapper around operator delete
   static void delete_TriFadcXscin(void *p) {
      delete ((::TriFadcXscin*)p);
   }
   static void deleteArray_TriFadcXscin(void *p) {
      delete [] ((::TriFadcXscin*)p);
   }
   static void destruct_TriFadcXscin(void *p) {
      typedef ::TriFadcXscin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcXscin

namespace {
  void TriggerDictionaryInitialization_TriFadcXscinDict_Impl() {
    static const char* headers[] = {
"TriFadcXscin.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/TriFadcXscin",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/TriFadcXscin/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TriFadcXscinDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Generic scintillator class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcXscin.h")))  TriFadcXscin;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TriFadcXscinDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TriFadcXscin.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TriFadcXscin", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TriFadcXscinDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TriFadcXscinDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TriFadcXscinDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TriFadcXscinDict() {
  TriggerDictionaryInitialization_TriFadcXscinDict_Impl();
}
