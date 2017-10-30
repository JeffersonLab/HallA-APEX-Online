// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TriFadcRasteredBeamDict

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
#include "TriFadcRasteredBeam.h"
#include "TriFadcRaster.h"
#include "TriFadcBPM.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_TriFadcRasteredBeam(void *p);
   static void deleteArray_TriFadcRasteredBeam(void *p);
   static void destruct_TriFadcRasteredBeam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcRasteredBeam*)
   {
      ::TriFadcRasteredBeam *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriFadcRasteredBeam >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcRasteredBeam", ::TriFadcRasteredBeam::Class_Version(), "TriFadcRasteredBeam.h", 12,
                  typeid(::TriFadcRasteredBeam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriFadcRasteredBeam::Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcRasteredBeam) );
      instance.SetDelete(&delete_TriFadcRasteredBeam);
      instance.SetDeleteArray(&deleteArray_TriFadcRasteredBeam);
      instance.SetDestructor(&destruct_TriFadcRasteredBeam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcRasteredBeam*)
   {
      return GenerateInitInstanceLocal((::TriFadcRasteredBeam*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TriFadcRasteredBeam*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TriFadcRaster(void *p);
   static void deleteArray_TriFadcRaster(void *p);
   static void destruct_TriFadcRaster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcRaster*)
   {
      ::TriFadcRaster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriFadcRaster >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcRaster", ::TriFadcRaster::Class_Version(), "TriFadcRaster.h", 13,
                  typeid(::TriFadcRaster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriFadcRaster::Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcRaster) );
      instance.SetDelete(&delete_TriFadcRaster);
      instance.SetDeleteArray(&deleteArray_TriFadcRaster);
      instance.SetDestructor(&destruct_TriFadcRaster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcRaster*)
   {
      return GenerateInitInstanceLocal((::TriFadcRaster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TriFadcRaster*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TriFadcBPM(void *p);
   static void deleteArray_TriFadcBPM(void *p);
   static void destruct_TriFadcBPM(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TriFadcBPM*)
   {
      ::TriFadcBPM *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TriFadcBPM >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TriFadcBPM", ::TriFadcBPM::Class_Version(), "TriFadcBPM.h", 13,
                  typeid(::TriFadcBPM), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TriFadcBPM::Dictionary, isa_proxy, 4,
                  sizeof(::TriFadcBPM) );
      instance.SetDelete(&delete_TriFadcBPM);
      instance.SetDeleteArray(&deleteArray_TriFadcBPM);
      instance.SetDestructor(&destruct_TriFadcBPM);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TriFadcBPM*)
   {
      return GenerateInitInstanceLocal((::TriFadcBPM*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TriFadcBPM*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TriFadcRasteredBeam::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriFadcRasteredBeam::Class_Name()
{
   return "TriFadcRasteredBeam";
}

//______________________________________________________________________________
const char *TriFadcRasteredBeam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRasteredBeam*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriFadcRasteredBeam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRasteredBeam*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriFadcRasteredBeam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRasteredBeam*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriFadcRasteredBeam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRasteredBeam*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TriFadcRaster::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriFadcRaster::Class_Name()
{
   return "TriFadcRaster";
}

//______________________________________________________________________________
const char *TriFadcRaster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRaster*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriFadcRaster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRaster*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriFadcRaster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRaster*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriFadcRaster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcRaster*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TriFadcBPM::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TriFadcBPM::Class_Name()
{
   return "TriFadcBPM";
}

//______________________________________________________________________________
const char *TriFadcBPM::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcBPM*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TriFadcBPM::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TriFadcBPM*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TriFadcBPM::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcBPM*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TriFadcBPM::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TriFadcBPM*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TriFadcRasteredBeam::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriFadcRasteredBeam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriFadcRasteredBeam::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriFadcRasteredBeam::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TriFadcRasteredBeam(void *p) {
      delete ((::TriFadcRasteredBeam*)p);
   }
   static void deleteArray_TriFadcRasteredBeam(void *p) {
      delete [] ((::TriFadcRasteredBeam*)p);
   }
   static void destruct_TriFadcRasteredBeam(void *p) {
      typedef ::TriFadcRasteredBeam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcRasteredBeam

//______________________________________________________________________________
void TriFadcRaster::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriFadcRaster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriFadcRaster::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriFadcRaster::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TriFadcRaster(void *p) {
      delete ((::TriFadcRaster*)p);
   }
   static void deleteArray_TriFadcRaster(void *p) {
      delete [] ((::TriFadcRaster*)p);
   }
   static void destruct_TriFadcRaster(void *p) {
      typedef ::TriFadcRaster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcRaster

//______________________________________________________________________________
void TriFadcBPM::Streamer(TBuffer &R__b)
{
   // Stream an object of class TriFadcBPM.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TriFadcBPM::Class(),this);
   } else {
      R__b.WriteClassBuffer(TriFadcBPM::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TriFadcBPM(void *p) {
      delete ((::TriFadcBPM*)p);
   }
   static void deleteArray_TriFadcBPM(void *p) {
      delete [] ((::TriFadcBPM*)p);
   }
   static void destruct_TriFadcBPM(void *p) {
      typedef ::TriFadcBPM current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TriFadcBPM

namespace {
  void TriggerDictionaryInitialization_TriFadcRasteredBeamDict_Impl() {
    static const char* headers[] = {
"TriFadcRasteredBeam.h",
"TriFadcRaster.h",
"TriFadcBPM.h",
0
    };
    static const char* includePaths[] = {
"/adaqfs/apps/ROOT/pro/include",
"/adaqfs/apps/analyzer/include",
"/adaqfs/apps/analyzer/src",
"/adaqfs/home/a-onl/tritium_work/hanjie/HallA-Online-Tritium/replay/libraries/TriFadcRasteredBeam",
"/adaqfs/apps/ROOT/6.10-08/include",
"/adaqfs/home/a-onl/tritium_work/hanjie/HallA-Online-Tritium/replay/libraries/TriFadcRasteredBeam/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TriFadcRasteredBeamDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(A beam with rastered beam, analyzed event by event using raster currents)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcRasteredBeam.h")))  TriFadcRasteredBeam;
class __attribute__((annotate(R"ATTRDUMP(Generic Raster class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcRaster.h")))  TriFadcRaster;
class __attribute__((annotate(R"ATTRDUMP(Generic BPM class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcBPM.h")))  TriFadcBPM;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TriFadcRasteredBeamDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TriFadcRasteredBeam.h"
#include "TriFadcRaster.h"
#include "TriFadcBPM.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TriFadcBPM", payloadCode, "@",
"TriFadcRaster", payloadCode, "@",
"TriFadcRasteredBeam", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TriFadcRasteredBeamDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TriFadcRasteredBeamDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TriFadcRasteredBeamDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TriFadcRasteredBeamDict() {
  TriggerDictionaryInitialization_TriFadcRasteredBeamDict_Impl();
}
