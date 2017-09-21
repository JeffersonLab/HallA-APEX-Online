// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Tritium_THaScaler100EvtHandlerDict

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
#include "Tritium_THaScaler100EvtHandler.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_Tritium_THaScaler100EvtHandler(void *p);
   static void deleteArray_Tritium_THaScaler100EvtHandler(void *p);
   static void destruct_Tritium_THaScaler100EvtHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tritium_THaScaler100EvtHandler*)
   {
      ::Tritium_THaScaler100EvtHandler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Tritium_THaScaler100EvtHandler >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Tritium_THaScaler100EvtHandler", ::Tritium_THaScaler100EvtHandler::Class_Version(), "Tritium_THaScaler100EvtHandler.h", 28,
                  typeid(::Tritium_THaScaler100EvtHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Tritium_THaScaler100EvtHandler::Dictionary, isa_proxy, 4,
                  sizeof(::Tritium_THaScaler100EvtHandler) );
      instance.SetDelete(&delete_Tritium_THaScaler100EvtHandler);
      instance.SetDeleteArray(&deleteArray_Tritium_THaScaler100EvtHandler);
      instance.SetDestructor(&destruct_Tritium_THaScaler100EvtHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tritium_THaScaler100EvtHandler*)
   {
      return GenerateInitInstanceLocal((::Tritium_THaScaler100EvtHandler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tritium_THaScaler100EvtHandler*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Tritium_THaScaler100EvtHandler::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Tritium_THaScaler100EvtHandler::Class_Name()
{
   return "Tritium_THaScaler100EvtHandler";
}

//______________________________________________________________________________
const char *Tritium_THaScaler100EvtHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_THaScaler100EvtHandler*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Tritium_THaScaler100EvtHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tritium_THaScaler100EvtHandler*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Tritium_THaScaler100EvtHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_THaScaler100EvtHandler*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Tritium_THaScaler100EvtHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tritium_THaScaler100EvtHandler*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Tritium_THaScaler100EvtHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class Tritium_THaScaler100EvtHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Tritium_THaScaler100EvtHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(Tritium_THaScaler100EvtHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Tritium_THaScaler100EvtHandler(void *p) {
      delete ((::Tritium_THaScaler100EvtHandler*)p);
   }
   static void deleteArray_Tritium_THaScaler100EvtHandler(void *p) {
      delete [] ((::Tritium_THaScaler100EvtHandler*)p);
   }
   static void destruct_Tritium_THaScaler100EvtHandler(void *p) {
      typedef ::Tritium_THaScaler100EvtHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tritium_THaScaler100EvtHandler

namespace {
  void TriggerDictionaryInitialization_Tritium_THaScaler100EvtHandlerDict_Impl() {
    static const char* headers[] = {
"Tritium_THaScaler100EvtHandler.h",
0
    };
    static const char* includePaths[] = {
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Evt100",
"./Tritium_Xscin",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Evt100/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Tritium_THaScaler100EvtHandlerDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Scaler Event handler)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$Tritium_THaScaler100EvtHandler.h")))  Tritium_THaScaler100EvtHandler;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Tritium_THaScaler100EvtHandlerDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Tritium_THaScaler100EvtHandler.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Tritium_THaScaler100EvtHandler", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Tritium_THaScaler100EvtHandlerDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Tritium_THaScaler100EvtHandlerDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Tritium_THaScaler100EvtHandlerDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Tritium_THaScaler100EvtHandlerDict() {
  TriggerDictionaryInitialization_Tritium_THaScaler100EvtHandlerDict_Impl();
}
