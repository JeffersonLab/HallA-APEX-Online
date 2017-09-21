// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Tritium_Beam_ElossDict

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
#include "Tritium_Beam_Eloss.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_Tritium_Beam_Eloss(void *p);
   static void deleteArray_Tritium_Beam_Eloss(void *p);
   static void destruct_Tritium_Beam_Eloss(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tritium_Beam_Eloss*)
   {
      ::Tritium_Beam_Eloss *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Tritium_Beam_Eloss >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Tritium_Beam_Eloss", ::Tritium_Beam_Eloss::Class_Version(), "Tritium_Beam_Eloss.h", 13,
                  typeid(::Tritium_Beam_Eloss), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Tritium_Beam_Eloss::Dictionary, isa_proxy, 4,
                  sizeof(::Tritium_Beam_Eloss) );
      instance.SetDelete(&delete_Tritium_Beam_Eloss);
      instance.SetDeleteArray(&deleteArray_Tritium_Beam_Eloss);
      instance.SetDestructor(&destruct_Tritium_Beam_Eloss);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tritium_Beam_Eloss*)
   {
      return GenerateInitInstanceLocal((::Tritium_Beam_Eloss*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tritium_Beam_Eloss*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Tritium_Beam_Eloss::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Tritium_Beam_Eloss::Class_Name()
{
   return "Tritium_Beam_Eloss";
}

//______________________________________________________________________________
const char *Tritium_Beam_Eloss::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Beam_Eloss*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Tritium_Beam_Eloss::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Beam_Eloss*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Tritium_Beam_Eloss::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Beam_Eloss*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Tritium_Beam_Eloss::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_Beam_Eloss*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Tritium_Beam_Eloss::Streamer(TBuffer &R__b)
{
   // Stream an object of class Tritium_Beam_Eloss.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Tritium_Beam_Eloss::Class(),this);
   } else {
      R__b.WriteClassBuffer(Tritium_Beam_Eloss::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Tritium_Beam_Eloss(void *p) {
      delete ((::Tritium_Beam_Eloss*)p);
   }
   static void deleteArray_Tritium_Beam_Eloss(void *p) {
      delete [] ((::Tritium_Beam_Eloss*)p);
   }
   static void destruct_Tritium_Beam_Eloss(void *p) {
      typedef ::Tritium_Beam_Eloss current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tritium_Beam_Eloss

namespace {
  void TriggerDictionaryInitialization_Tritium_Beam_ElossDict_Impl() {
    static const char* headers[] = {
"Tritium_Beam_Eloss.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Beam_Eloss",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Beam_Eloss/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Tritium_Beam_ElossDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Beam energy loss correction module)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$Tritium_Beam_Eloss.h")))  Tritium_Beam_Eloss;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Tritium_Beam_ElossDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Tritium_Beam_Eloss.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Tritium_Beam_Eloss", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Tritium_Beam_ElossDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Tritium_Beam_ElossDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Tritium_Beam_ElossDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Tritium_Beam_ElossDict() {
  TriggerDictionaryInitialization_Tritium_Beam_ElossDict_Impl();
}
