// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Tritium_TSScalerDict

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
#include "Tritium_TSScaler.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_Tritium_TSScaler(void *p);
   static void deleteArray_Tritium_TSScaler(void *p);
   static void destruct_Tritium_TSScaler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tritium_TSScaler*)
   {
      ::Tritium_TSScaler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Tritium_TSScaler >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Tritium_TSScaler", ::Tritium_TSScaler::Class_Version(), "Tritium_TSScaler.h", 28,
                  typeid(::Tritium_TSScaler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Tritium_TSScaler::Dictionary, isa_proxy, 4,
                  sizeof(::Tritium_TSScaler) );
      instance.SetDelete(&delete_Tritium_TSScaler);
      instance.SetDeleteArray(&deleteArray_Tritium_TSScaler);
      instance.SetDestructor(&destruct_Tritium_TSScaler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tritium_TSScaler*)
   {
      return GenerateInitInstanceLocal((::Tritium_TSScaler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tritium_TSScaler*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Tritium_TSScaler::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Tritium_TSScaler::Class_Name()
{
   return "Tritium_TSScaler";
}

//______________________________________________________________________________
const char *Tritium_TSScaler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_TSScaler*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Tritium_TSScaler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_TSScaler*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Tritium_TSScaler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_TSScaler*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Tritium_TSScaler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_TSScaler*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Tritium_TSScaler::Streamer(TBuffer &R__b)
{
   // Stream an object of class Tritium_TSScaler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Tritium_TSScaler::Class(),this);
   } else {
      R__b.WriteClassBuffer(Tritium_TSScaler::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Tritium_TSScaler(void *p) {
      delete ((::Tritium_TSScaler*)p);
   }
   static void deleteArray_Tritium_TSScaler(void *p) {
      delete [] ((::Tritium_TSScaler*)p);
   }
   static void destruct_Tritium_TSScaler(void *p) {
      typedef ::Tritium_TSScaler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tritium_TSScaler

namespace {
  void TriggerDictionaryInitialization_Tritium_TSScalerDict_Impl() {
    static const char* headers[] = {
"Tritium_TSScaler.h",
0
    };
    static const char* includePaths[] = {
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_TSScaler",
"./Tritium_Xscin",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_TSScaler/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Tritium_TSScalerDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Scaler Event handler)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$Tritium_TSScaler.h")))  Tritium_TSScaler;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Tritium_TSScalerDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Tritium_TSScaler.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Tritium_TSScaler", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Tritium_TSScalerDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Tritium_TSScalerDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Tritium_TSScalerDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Tritium_TSScalerDict() {
  TriggerDictionaryInitialization_Tritium_TSScalerDict_Impl();
}
