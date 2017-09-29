// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TriFadcCherenkovDict

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
#include "TriFadcCherenkov.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TriFadcCherenkov(void *p = 0);
   static void *newArray_TriFadcCherenkov(Long_t size, void *p);
   static void delete_TriFadcCherenkov(void *p);
   static void deleteArray_TriFadcCherenkov(void *p);
   static void destruct_TriFadcCherenkov(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcCherenkov*)
   {
      ::TriFadcCherenkov *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriFadcCherenkov >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcCherenkov", ::TriFadcCherenkov::Class_Version(), "TriFadcCherenkov.h", 16,
                  typeid(::TriFadcCherenkov), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriFadcCherenkov::Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcCherenkov) );
      instance.SetNew(&new_TriFadcCherenkov);
      instance.SetNewArray(&newArray_TriFadcCherenkov);
      instance.SetDelete(&delete_TriFadcCherenkov);
      instance.SetDeleteArray(&deleteArray_TriFadcCherenkov);
      instance.SetDestructor(&destruct_TriFadcCherenkov);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcCherenkov*)
   {
      return GenerateInitInstanceLocal((::TriFadcCherenkov*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TriFadcCherenkov*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TriFadcCherenkov::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriFadcCherenkov::Class_Name()
{
   return "TriFadcCherenkov";
}

//______________________________________________________________________________
const char *TriFadcCherenkov::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcCherenkov*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriFadcCherenkov::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcCherenkov*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriFadcCherenkov::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcCherenkov*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriFadcCherenkov::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcCherenkov*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TriFadcCherenkov::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriFadcCherenkov.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriFadcCherenkov::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriFadcCherenkov::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TriFadcCherenkov(void *p) {
      return  p ? new(p) ::TriFadcCherenkov : new ::TriFadcCherenkov;
   }
   static void *newArray_TriFadcCherenkov(Long_t nElements, void *p) {
      return p ? new(p) ::TriFadcCherenkov[nElements] : new ::TriFadcCherenkov[nElements];
   }
   // Wrapper around operator delete
   static void delete_TriFadcCherenkov(void *p) {
      delete ((::TriFadcCherenkov*)p);
   }
   static void deleteArray_TriFadcCherenkov(void *p) {
      delete [] ((::TriFadcCherenkov*)p);
   }
   static void destruct_TriFadcCherenkov(void *p) {
      typedef ::TriFadcCherenkov current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcCherenkov

namespace {
  void TriggerDictionaryInitialization_TriFadcCherenkovDict_Impl() {
    static const char* headers[] = {
"TriFadcCherenkov.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/TriFadcCherenkov",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/TriFadcCherenkov/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TriFadcCherenkovDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Generic Cherenkov class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcCherenkov.h")))  TriFadcCherenkov;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TriFadcCherenkovDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TriFadcCherenkov.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TriFadcCherenkov", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TriFadcCherenkovDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TriFadcCherenkovDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TriFadcCherenkovDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TriFadcCherenkovDict() {
  TriggerDictionaryInitialization_TriFadcCherenkovDict_Impl();
}
