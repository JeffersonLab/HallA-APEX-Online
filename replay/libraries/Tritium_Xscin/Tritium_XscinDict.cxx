// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Tritium_XscinDict

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
#include "Tritium_Xscin.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Tritium_Xscin(void *p = 0);
   static void *newArray_Tritium_Xscin(Long_t size, void *p);
   static void delete_Tritium_Xscin(void *p);
   static void deleteArray_Tritium_Xscin(void *p);
   static void destruct_Tritium_Xscin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tritium_Xscin*)
   {
      ::Tritium_Xscin *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Tritium_Xscin >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Tritium_Xscin", ::Tritium_Xscin::Class_Version(), "Tritium_Xscin.h", 23,
                  typeid(::Tritium_Xscin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Tritium_Xscin::Dictionary, isa_proxy, 4,
                  sizeof(::Tritium_Xscin) );
      instance.SetNew(&new_Tritium_Xscin);
      instance.SetNewArray(&newArray_Tritium_Xscin);
      instance.SetDelete(&delete_Tritium_Xscin);
      instance.SetDeleteArray(&deleteArray_Tritium_Xscin);
      instance.SetDestructor(&destruct_Tritium_Xscin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tritium_Xscin*)
   {
      return GenerateInitInstanceLocal((::Tritium_Xscin*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tritium_Xscin*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Tritium_Xscin::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Tritium_Xscin::Class_Name()
{
   return "Tritium_Xscin";
}

//______________________________________________________________________________
const char *Tritium_Xscin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Xscin*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Tritium_Xscin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Xscin*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Tritium_Xscin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Xscin*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Tritium_Xscin::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Xscin*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Tritium_Xscin::Streamer(TBuffer &R__b)
{
   // Stream an object of class Tritium_Xscin.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Tritium_Xscin::Class(),this);
   } else {
      R__b.WriteClassBuffer(Tritium_Xscin::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Tritium_Xscin(void *p) {
      return  p ? new(p) ::Tritium_Xscin : new ::Tritium_Xscin;
   }
   static void *newArray_Tritium_Xscin(Long_t nElements, void *p) {
      return p ? new(p) ::Tritium_Xscin[nElements] : new ::Tritium_Xscin[nElements];
   }
   // Wrapper around operator delete
   static void delete_Tritium_Xscin(void *p) {
      delete ((::Tritium_Xscin*)p);
   }
   static void deleteArray_Tritium_Xscin(void *p) {
      delete [] ((::Tritium_Xscin*)p);
   }
   static void destruct_Tritium_Xscin(void *p) {
      typedef ::Tritium_Xscin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tritium_Xscin

namespace {
  void TriggerDictionaryInitialization_Tritium_XscinDict_Impl() {
    static const char* headers[] = {
"Tritium_Xscin.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Xscin",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Xscin/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Tritium_XscinDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Generic scintillator class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$Tritium_Xscin.h")))  Tritium_Xscin;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Tritium_XscinDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Tritium_Xscin.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Tritium_Xscin", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Tritium_XscinDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Tritium_XscinDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Tritium_XscinDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Tritium_XscinDict() {
  TriggerDictionaryInitialization_Tritium_XscinDict_Impl();
}
