// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TriScalerDict

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
#include "TriScalerEvtHandler.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_TriScalerEvtHandler(void *p);
   static void deleteArray_TriScalerEvtHandler(void *p);
   static void destruct_TriScalerEvtHandler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriScalerEvtHandler*)
   {
      ::TriScalerEvtHandler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriScalerEvtHandler >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriScalerEvtHandler", ::TriScalerEvtHandler::Class_Version(), "TriScalerEvtHandler.h", 29,
                  typeid(::TriScalerEvtHandler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriScalerEvtHandler::Dictionary, isa_proxy, 4,
                  sizeof(::TriScalerEvtHandler) );
      instance.SetDelete(&delete_TriScalerEvtHandler);
      instance.SetDeleteArray(&deleteArray_TriScalerEvtHandler);
      instance.SetDestructor(&destruct_TriScalerEvtHandler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriScalerEvtHandler*)
   {
      return GenerateInitInstanceLocal((::TriScalerEvtHandler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TriScalerEvtHandler*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TriScalerEvtHandler::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriScalerEvtHandler::Class_Name()
{
   return "TriScalerEvtHandler";
}

//______________________________________________________________________________
const char *TriScalerEvtHandler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriScalerEvtHandler*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriScalerEvtHandler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriScalerEvtHandler*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriScalerEvtHandler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriScalerEvtHandler*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriScalerEvtHandler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriScalerEvtHandler*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TriScalerEvtHandler::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriScalerEvtHandler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriScalerEvtHandler::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriScalerEvtHandler::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TriScalerEvtHandler(void *p) {
      delete ((::TriScalerEvtHandler*)p);
   }
   static void deleteArray_TriScalerEvtHandler(void *p) {
      delete [] ((::TriScalerEvtHandler*)p);
   }
   static void destruct_TriScalerEvtHandler(void *p) {
      typedef ::TriScalerEvtHandler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriScalerEvtHandler

namespace {
  void TriggerDictionaryInitialization_TriScalerDict_Impl() {
    static const char* headers[] = {
"TriScalerEvtHandler.h",
0
    };
    static const char* includePaths[] = {
"/adaqfs/apps/ROOT/pro/include",
"/adaqfs/home/a-onl/tritium/Tong/aonl_root6/analyzer_1.6/analyzer/src",
"/adaqfs/home/a-onl/tritium/Tong/aonl_root6/analyzer_1.6/analyzer/hana_decode",
"/adaqfs/home/a-onl/tritium/hanjie/HallA-Online-Tritium/replay/libraries/TriScalerEvtHandler",
"/adaqfs/apps/ROOT/6.10-08/include",
"/adaqfs/home/a-onl/tritium/hanjie/HallA-Online-Tritium/replay/libraries/TriScalerEvtHandler/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TriScalerDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Scaler Event handler)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriScalerEvtHandler.h")))  TriScalerEvtHandler;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TriScalerDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef NDEBUG
  #define NDEBUG 1
#endif
#ifndef WITH_DEBUG
  #define WITH_DEBUG 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TriScalerEvtHandler.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TriScalerEvtHandler", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TriScalerDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TriScalerDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TriScalerDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TriScalerDict() {
  TriggerDictionaryInitialization_TriScalerDict_Impl();
}
