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
#include "TritiumDetMap.h"
#include "TritiumDetectorBase.h"
#include "TritiumDetector.h"
#include "TritiumSpectrometerDetector.h"
#include "TritiumNonTrackingDetector.h"
#include "TriFadcScin.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TritiumDetMap(void *p = 0);
   static void *newArray_TritiumDetMap(Long_t size, void *p);
   static void delete_TritiumDetMap(void *p);
   static void deleteArray_TritiumDetMap(void *p);
   static void destruct_TritiumDetMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TritiumDetMap*)
   {
      ::TritiumDetMap *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TritiumDetMap >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TritiumDetMap", ::TritiumDetMap::Class_Version(), "TritiumDetMap.h", 25,
                  typeid(::TritiumDetMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TritiumDetMap::Dictionary, isa_proxy, 4,
                  sizeof(::TritiumDetMap) );
      instance.SetNew(&new_TritiumDetMap);
      instance.SetNewArray(&newArray_TritiumDetMap);
      instance.SetDelete(&delete_TritiumDetMap);
      instance.SetDeleteArray(&deleteArray_TritiumDetMap);
      instance.SetDestructor(&destruct_TritiumDetMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TritiumDetMap*)
   {
      return GenerateInitInstanceLocal((::TritiumDetMap*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TritiumDetMap*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TritiumDetectorBase(void *p);
   static void deleteArray_TritiumDetectorBase(void *p);
   static void destruct_TritiumDetectorBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TritiumDetectorBase*)
   {
      ::TritiumDetectorBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TritiumDetectorBase >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TritiumDetectorBase", ::TritiumDetectorBase::Class_Version(), "TritiumDetectorBase.h", 16,
                  typeid(::TritiumDetectorBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TritiumDetectorBase::Dictionary, isa_proxy, 4,
                  sizeof(::TritiumDetectorBase) );
      instance.SetDelete(&delete_TritiumDetectorBase);
      instance.SetDeleteArray(&deleteArray_TritiumDetectorBase);
      instance.SetDestructor(&destruct_TritiumDetectorBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TritiumDetectorBase*)
   {
      return GenerateInitInstanceLocal((::TritiumDetectorBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TritiumDetectorBase*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TritiumDetector(void *p);
   static void deleteArray_TritiumDetector(void *p);
   static void destruct_TritiumDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TritiumDetector*)
   {
      ::TritiumDetector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TritiumDetector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TritiumDetector", ::TritiumDetector::Class_Version(), "TritiumDetector.h", 19,
                  typeid(::TritiumDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TritiumDetector::Dictionary, isa_proxy, 4,
                  sizeof(::TritiumDetector) );
      instance.SetDelete(&delete_TritiumDetector);
      instance.SetDeleteArray(&deleteArray_TritiumDetector);
      instance.SetDestructor(&destruct_TritiumDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TritiumDetector*)
   {
      return GenerateInitInstanceLocal((::TritiumDetector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TritiumDetector*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TritiumSpectrometerDetector(void *p);
   static void deleteArray_TritiumSpectrometerDetector(void *p);
   static void destruct_TritiumSpectrometerDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TritiumSpectrometerDetector*)
   {
      ::TritiumSpectrometerDetector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TritiumSpectrometerDetector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TritiumSpectrometerDetector", ::TritiumSpectrometerDetector::Class_Version(), "TritiumSpectrometerDetector.h", 19,
                  typeid(::TritiumSpectrometerDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TritiumSpectrometerDetector::Dictionary, isa_proxy, 4,
                  sizeof(::TritiumSpectrometerDetector) );
      instance.SetDelete(&delete_TritiumSpectrometerDetector);
      instance.SetDeleteArray(&deleteArray_TritiumSpectrometerDetector);
      instance.SetDestructor(&destruct_TritiumSpectrometerDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TritiumSpectrometerDetector*)
   {
      return GenerateInitInstanceLocal((::TritiumSpectrometerDetector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TritiumSpectrometerDetector*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TritiumNonTrackingDetector(void *p);
   static void deleteArray_TritiumNonTrackingDetector(void *p);
   static void destruct_TritiumNonTrackingDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TritiumNonTrackingDetector*)
   {
      ::TritiumNonTrackingDetector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TritiumNonTrackingDetector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TritiumNonTrackingDetector", ::TritiumNonTrackingDetector::Class_Version(), "TritiumNonTrackingDetector.h", 14,
                  typeid(::TritiumNonTrackingDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TritiumNonTrackingDetector::Dictionary, isa_proxy, 4,
                  sizeof(::TritiumNonTrackingDetector) );
      instance.SetDelete(&delete_TritiumNonTrackingDetector);
      instance.SetDeleteArray(&deleteArray_TritiumNonTrackingDetector);
      instance.SetDestructor(&destruct_TritiumNonTrackingDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TritiumNonTrackingDetector*)
   {
      return GenerateInitInstanceLocal((::TritiumNonTrackingDetector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TritiumNonTrackingDetector*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

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
atomic_TClass_ptr TritiumDetMap::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TritiumDetMap::Class_Name()
{
   return "TritiumDetMap";
}

//______________________________________________________________________________
const char *TritiumDetMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetMap*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TritiumDetMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetMap*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TritiumDetMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetMap*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TritiumDetMap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetMap*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TritiumDetectorBase::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TritiumDetectorBase::Class_Name()
{
   return "TritiumDetectorBase";
}

//______________________________________________________________________________
const char *TritiumDetectorBase::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetectorBase*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TritiumDetectorBase::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetectorBase*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TritiumDetectorBase::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetectorBase*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TritiumDetectorBase::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetectorBase*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TritiumDetector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TritiumDetector::Class_Name()
{
   return "TritiumDetector";
}

//______________________________________________________________________________
const char *TritiumDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TritiumDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TritiumDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TritiumDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumDetector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TritiumSpectrometerDetector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TritiumSpectrometerDetector::Class_Name()
{
   return "TritiumSpectrometerDetector";
}

//______________________________________________________________________________
const char *TritiumSpectrometerDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumSpectrometerDetector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TritiumSpectrometerDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumSpectrometerDetector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TritiumSpectrometerDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumSpectrometerDetector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TritiumSpectrometerDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumSpectrometerDetector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TritiumNonTrackingDetector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TritiumNonTrackingDetector::Class_Name()
{
   return "TritiumNonTrackingDetector";
}

//______________________________________________________________________________
const char *TritiumNonTrackingDetector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumNonTrackingDetector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TritiumNonTrackingDetector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TritiumNonTrackingDetector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TritiumNonTrackingDetector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumNonTrackingDetector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TritiumNonTrackingDetector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TritiumNonTrackingDetector*)0x0)->GetClass(); }
   return fgIsA;
}

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
void TritiumDetMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class TritiumDetMap.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TritiumDetMap::Class(),this);
   } else {
      R__b.WriteClassBuffer(TritiumDetMap::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TritiumDetMap(void *p) {
      return  p ? new(p) ::TritiumDetMap : new ::TritiumDetMap;
   }
   static void *newArray_TritiumDetMap(Long_t nElements, void *p) {
      return p ? new(p) ::TritiumDetMap[nElements] : new ::TritiumDetMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_TritiumDetMap(void *p) {
      delete ((::TritiumDetMap*)p);
   }
   static void deleteArray_TritiumDetMap(void *p) {
      delete [] ((::TritiumDetMap*)p);
   }
   static void destruct_TritiumDetMap(void *p) {
      typedef ::TritiumDetMap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TritiumDetMap

//______________________________________________________________________________
void TritiumDetectorBase::Streamer(TBuffer &R__b)
{
   // Stream an object of class TritiumDetectorBase.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TritiumDetectorBase::Class(),this);
   } else {
      R__b.WriteClassBuffer(TritiumDetectorBase::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TritiumDetectorBase(void *p) {
      delete ((::TritiumDetectorBase*)p);
   }
   static void deleteArray_TritiumDetectorBase(void *p) {
      delete [] ((::TritiumDetectorBase*)p);
   }
   static void destruct_TritiumDetectorBase(void *p) {
      typedef ::TritiumDetectorBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TritiumDetectorBase

//______________________________________________________________________________
void TritiumDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class TritiumDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TritiumDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(TritiumDetector::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TritiumDetector(void *p) {
      delete ((::TritiumDetector*)p);
   }
   static void deleteArray_TritiumDetector(void *p) {
      delete [] ((::TritiumDetector*)p);
   }
   static void destruct_TritiumDetector(void *p) {
      typedef ::TritiumDetector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TritiumDetector

//______________________________________________________________________________
void TritiumSpectrometerDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class TritiumSpectrometerDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TritiumSpectrometerDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(TritiumSpectrometerDetector::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TritiumSpectrometerDetector(void *p) {
      delete ((::TritiumSpectrometerDetector*)p);
   }
   static void deleteArray_TritiumSpectrometerDetector(void *p) {
      delete [] ((::TritiumSpectrometerDetector*)p);
   }
   static void destruct_TritiumSpectrometerDetector(void *p) {
      typedef ::TritiumSpectrometerDetector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TritiumSpectrometerDetector

//______________________________________________________________________________
void TritiumNonTrackingDetector::Streamer(TBuffer &R__b)
{
   // Stream an object of class TritiumNonTrackingDetector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TritiumNonTrackingDetector::Class(),this);
   } else {
      R__b.WriteClassBuffer(TritiumNonTrackingDetector::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TritiumNonTrackingDetector(void *p) {
      delete ((::TritiumNonTrackingDetector*)p);
   }
   static void deleteArray_TritiumNonTrackingDetector(void *p) {
      delete [] ((::TritiumNonTrackingDetector*)p);
   }
   static void destruct_TritiumNonTrackingDetector(void *p) {
      typedef ::TritiumNonTrackingDetector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TritiumNonTrackingDetector

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
"TritiumDetMap.h",
"TritiumDetectorBase.h",
"TritiumDetector.h",
"TritiumSpectrometerDetector.h",
"TritiumNonTrackingDetector.h",
"TriFadcScin.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/me/analyzer/src",
"/home/me/analyzer/hana_decode",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Hanjie_Scin",
"/usr/include/root",
"/home/me/tritium/HallA-Online-Tritium/replay/libraries/Tritium_Hanjie_Scin/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TriFadcScinDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(The standard detector map)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TritiumDetMap.h")))  TritiumDetMap;
class __attribute__((annotate(R"ATTRDUMP(ABC for a detector or subdetector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TritiumDetectorBase.h")))  TritiumDetectorBase;
class __attribute__((annotate(R"ATTRDUMP(Abstract base class for a Hall A detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TritiumDetector.h")))  TritiumDetector;
class __attribute__((annotate(R"ATTRDUMP(ABC for a spectrometer detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TritiumSpectrometerDetector.h")))  TritiumSpectrometerDetector;
class __attribute__((annotate(R"ATTRDUMP(ABC for a non-tracking spectrometer detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TritiumNonTrackingDetector.h")))  TritiumNonTrackingDetector;
class __attribute__((annotate(R"ATTRDUMP(Generic scintillator class)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TriFadcScin.h")))  TriFadcScin;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TriFadcScinDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TritiumDetMap.h"
#include "TritiumDetectorBase.h"
#include "TritiumDetector.h"
#include "TritiumSpectrometerDetector.h"
#include "TritiumNonTrackingDetector.h"
#include "TriFadcScin.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TriFadcScin", payloadCode, "@",
"TriFadcScin::DataDest", payloadCode, "@",
"TritiumDetMap", payloadCode, "@",
"TritiumDetector", payloadCode, "@",
"TritiumDetectorBase", payloadCode, "@",
"TritiumNonTrackingDetector", payloadCode, "@",
"TritiumSpectrometerDetector", payloadCode, "@",
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
