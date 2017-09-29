// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TriFadcScinDict

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
#include "TriFadcScin.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TriFadcScin(void *p = 0);
   static void *newArray_TriFadcScin(Long_t size, void *p);
   static void delete_TriFadcScin(void *p);
   static void deleteArray_TriFadcScin(void *p);
   static void destruct_TriFadcScin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcScin*)
   {
      ::TriFadcScin *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriFadcScin >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcScin", ::TriFadcScin::Class_Version(), "TriFadcScin.h", 15,
                  typeid(::TriFadcScin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriFadcScin::Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcScin) );
      instance.SetNew(&new_TriFadcScin);
      instance.SetNewArray(&newArray_TriFadcScin);
      instance.SetDelete(&delete_TriFadcScin);
      instance.SetDeleteArray(&deleteArray_TriFadcScin);
      instance.SetDestructor(&destruct_TriFadcScin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcScin*)
   {
      return GenerateInitInstanceLocal((::TriFadcScin*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TriFadcScin*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TriFadcScincLcLDataDest_Dictionary();
   static void TriFadcScincLcLDataDest_TClassManip(TClass*);
   static void *new_TriFadcScincLcLDataDest(void *p = 0);
   static void *newArray_TriFadcScincLcLDataDest(Long_t size, void *p);
   static void delete_TriFadcScincLcLDataDest(void *p);
   static void deleteArray_TriFadcScincLcLDataDest(void *p);
   static void destruct_TriFadcScincLcLDataDest(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcScin::DataDest*)
   {
      ::TriFadcScin::DataDest *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TriFadcScin::DataDest));
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcScin::DataDest", "TriFadcScin.h", 39,
                  typeid(::TriFadcScin::DataDest), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TriFadcScincLcLDataDest_Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcScin::DataDest) );
      instance.SetNew(&new_TriFadcScincLcLDataDest);
      instance.SetNewArray(&newArray_TriFadcScincLcLDataDest);
      instance.SetDelete(&delete_TriFadcScincLcLDataDest);
      instance.SetDeleteArray(&deleteArray_TriFadcScincLcLDataDest);
      instance.SetDestructor(&destruct_TriFadcScincLcLDataDest);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcScin::DataDest*)
   {
      return GenerateInitInstanceLocal((::TriFadcScin::DataDest*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TriFadcScin::DataDest*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TriFadcScincLcLDataDest_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TriFadcScin::DataDest*)0x0)->GetClass();
      TriFadcScincLcLDataDest_TClassManip(theClass);
   return theClass;
   }

   static void TriFadcScincLcLDataDest_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TriFadcScin::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriFadcScin::Class_Name()
{
   return "TriFadcScin";
}

//______________________________________________________________________________
const char *TriFadcScin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcScin*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriFadcScin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcScin*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriFadcScin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcScin*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriFadcScin::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcScin*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TriFadcScin::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriFadcScin.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriFadcScin::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriFadcScin::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TriFadcScin(void *p) {
      return  p ? new(p) ::TriFadcScin : new ::TriFadcScin;
   }
   static void *newArray_TriFadcScin(Long_t nElements, void *p) {
      return p ? new(p) ::TriFadcScin[nElements] : new ::TriFadcScin[nElements];
   }
   // Wrapper around operator delete
   static void delete_TriFadcScin(void *p) {
      delete ((::TriFadcScin*)p);
   }
   static void deleteArray_TriFadcScin(void *p) {
      delete [] ((::TriFadcScin*)p);
   }
   static void destruct_TriFadcScin(void *p) {
      typedef ::TriFadcScin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcScin

namespace ROOT {
   // Wrappers around operator new
   static void *new_TriFadcScincLcLDataDest(void *p) {
      return  p ? new(p) ::TriFadcScin::DataDest : new ::TriFadcScin::DataDest;
   }
   static void *newArray_TriFadcScincLcLDataDest(Long_t nElements, void *p) {
      return p ? new(p) ::TriFadcScin::DataDest[nElements] : new ::TriFadcScin::DataDest[nElements];
   }
   // Wrapper around operator delete
   static void delete_TriFadcScincLcLDataDest(void *p) {
      delete ((::TriFadcScin::DataDest*)p);
   }
   static void deleteArray_TriFadcScincLcLDataDest(void *p) {
      delete [] ((::TriFadcScin::DataDest*)p);
   }
   static void destruct_TriFadcScincLcLDataDest(void *p) {
      typedef ::TriFadcScin::DataDest current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcScin::DataDest

namespace {
  void TriggerDictionaryInitialization_TriFadcScinDict_Impl() {
    static const char* headers[] = {
"TriFadcScin.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/TriFadcScin",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/TriFadcScin/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TriFadcScinDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Generic scintillator class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcScin.h")))  TriFadcScin;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TriFadcScinDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TriFadcScin.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TriFadcScin", payloadCode, "@",
"TriFadcScin::DataDest", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TriFadcScinDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TriFadcScinDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TriFadcScinDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TriFadcScinDict() {
  TriggerDictionaryInitialization_TriFadcScinDict_Impl();
}
