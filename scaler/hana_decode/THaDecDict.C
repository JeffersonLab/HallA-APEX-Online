// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME THaDecDict

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
#include "THaUsrstrutils.h"
#include "THaCrateMap.h"
#include "THaCodaData.h"
#include "THaHelicity.h"
#include "THaEpics.h"
#include "THaFastBusWord.h"
#include "THaCodaFile.h"
#include "THaSlotData.h"
#include "THaEvData.h"
#include "evio.h"
#include "THaCodaDecoder.h"
#include "THaBenchmark.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_THaUsrstrutils(void *p = 0);
   static void *newArray_THaUsrstrutils(Long_t size, void *p);
   static void delete_THaUsrstrutils(void *p);
   static void deleteArray_THaUsrstrutils(void *p);
   static void destruct_THaUsrstrutils(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaUsrstrutils*)
   {
      ::THaUsrstrutils *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaUsrstrutils >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaUsrstrutils", ::THaUsrstrutils::Class_Version(), "THaUsrstrutils.h", 22,
                  typeid(::THaUsrstrutils), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaUsrstrutils::Dictionary, isa_proxy, 4,
                  sizeof(::THaUsrstrutils) );
      instance.SetNew(&new_THaUsrstrutils);
      instance.SetNewArray(&newArray_THaUsrstrutils);
      instance.SetDelete(&delete_THaUsrstrutils);
      instance.SetDeleteArray(&deleteArray_THaUsrstrutils);
      instance.SetDestructor(&destruct_THaUsrstrutils);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaUsrstrutils*)
   {
      return GenerateInitInstanceLocal((::THaUsrstrutils*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaUsrstrutils*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaCrateMap(void *p = 0);
   static void *newArray_THaCrateMap(Long_t size, void *p);
   static void delete_THaCrateMap(void *p);
   static void deleteArray_THaCrateMap(void *p);
   static void destruct_THaCrateMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaCrateMap*)
   {
      ::THaCrateMap *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaCrateMap >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaCrateMap", ::THaCrateMap::Class_Version(), "THaCrateMap.h", 26,
                  typeid(::THaCrateMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaCrateMap::Dictionary, isa_proxy, 4,
                  sizeof(::THaCrateMap) );
      instance.SetNew(&new_THaCrateMap);
      instance.SetNewArray(&newArray_THaCrateMap);
      instance.SetDelete(&delete_THaCrateMap);
      instance.SetDeleteArray(&deleteArray_THaCrateMap);
      instance.SetDestructor(&destruct_THaCrateMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaCrateMap*)
   {
      return GenerateInitInstanceLocal((::THaCrateMap*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaCrateMap*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_THaCodaData(void *p);
   static void deleteArray_THaCodaData(void *p);
   static void destruct_THaCodaData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaCodaData*)
   {
      ::THaCodaData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaCodaData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaCodaData", ::THaCodaData::Class_Version(), "THaCodaData.h", 27,
                  typeid(::THaCodaData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaCodaData::Dictionary, isa_proxy, 4,
                  sizeof(::THaCodaData) );
      instance.SetDelete(&delete_THaCodaData);
      instance.SetDeleteArray(&deleteArray_THaCodaData);
      instance.SetDestructor(&destruct_THaCodaData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaCodaData*)
   {
      return GenerateInitInstanceLocal((::THaCodaData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaCodaData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaHelicity(void *p = 0);
   static void *newArray_THaHelicity(Long_t size, void *p);
   static void delete_THaHelicity(void *p);
   static void deleteArray_THaHelicity(void *p);
   static void destruct_THaHelicity(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaHelicity*)
   {
      ::THaHelicity *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaHelicity >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaHelicity", ::THaHelicity::Class_Version(), "THaHelicity.h", 21,
                  typeid(::THaHelicity), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaHelicity::Dictionary, isa_proxy, 4,
                  sizeof(::THaHelicity) );
      instance.SetNew(&new_THaHelicity);
      instance.SetNewArray(&newArray_THaHelicity);
      instance.SetDelete(&delete_THaHelicity);
      instance.SetDeleteArray(&deleteArray_THaHelicity);
      instance.SetDestructor(&destruct_THaHelicity);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaHelicity*)
   {
      return GenerateInitInstanceLocal((::THaHelicity*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaHelicity*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaEpics(void *p = 0);
   static void *newArray_THaEpics(Long_t size, void *p);
   static void delete_THaEpics(void *p);
   static void deleteArray_THaEpics(void *p);
   static void destruct_THaEpics(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaEpics*)
   {
      ::THaEpics *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaEpics >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaEpics", ::THaEpics::Class_Version(), "THaEpics.h", 55,
                  typeid(::THaEpics), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaEpics::Dictionary, isa_proxy, 4,
                  sizeof(::THaEpics) );
      instance.SetNew(&new_THaEpics);
      instance.SetNewArray(&newArray_THaEpics);
      instance.SetDelete(&delete_THaEpics);
      instance.SetDeleteArray(&deleteArray_THaEpics);
      instance.SetDestructor(&destruct_THaEpics);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaEpics*)
   {
      return GenerateInitInstanceLocal((::THaEpics*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaEpics*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaFastBusWord(void *p = 0);
   static void *newArray_THaFastBusWord(Long_t size, void *p);
   static void delete_THaFastBusWord(void *p);
   static void deleteArray_THaFastBusWord(void *p);
   static void destruct_THaFastBusWord(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaFastBusWord*)
   {
      ::THaFastBusWord *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaFastBusWord >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaFastBusWord", ::THaFastBusWord::Class_Version(), "THaFastBusWord.h", 24,
                  typeid(::THaFastBusWord), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaFastBusWord::Dictionary, isa_proxy, 4,
                  sizeof(::THaFastBusWord) );
      instance.SetNew(&new_THaFastBusWord);
      instance.SetNewArray(&newArray_THaFastBusWord);
      instance.SetDelete(&delete_THaFastBusWord);
      instance.SetDeleteArray(&deleteArray_THaFastBusWord);
      instance.SetDestructor(&destruct_THaFastBusWord);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaFastBusWord*)
   {
      return GenerateInitInstanceLocal((::THaFastBusWord*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaFastBusWord*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaCodaFile(void *p = 0);
   static void *newArray_THaCodaFile(Long_t size, void *p);
   static void delete_THaCodaFile(void *p);
   static void deleteArray_THaCodaFile(void *p);
   static void destruct_THaCodaFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaCodaFile*)
   {
      ::THaCodaFile *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaCodaFile >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaCodaFile", ::THaCodaFile::Class_Version(), "THaCodaFile.h", 24,
                  typeid(::THaCodaFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaCodaFile::Dictionary, isa_proxy, 4,
                  sizeof(::THaCodaFile) );
      instance.SetNew(&new_THaCodaFile);
      instance.SetNewArray(&newArray_THaCodaFile);
      instance.SetDelete(&delete_THaCodaFile);
      instance.SetDeleteArray(&deleteArray_THaCodaFile);
      instance.SetDestructor(&destruct_THaCodaFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaCodaFile*)
   {
      return GenerateInitInstanceLocal((::THaCodaFile*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaCodaFile*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaSlotData(void *p = 0);
   static void *newArray_THaSlotData(Long_t size, void *p);
   static void delete_THaSlotData(void *p);
   static void deleteArray_THaSlotData(void *p);
   static void destruct_THaSlotData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaSlotData*)
   {
      ::THaSlotData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaSlotData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaSlotData", ::THaSlotData::Class_Version(), "THaSlotData.h", 27,
                  typeid(::THaSlotData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaSlotData::Dictionary, isa_proxy, 4,
                  sizeof(::THaSlotData) );
      instance.SetNew(&new_THaSlotData);
      instance.SetNewArray(&newArray_THaSlotData);
      instance.SetDelete(&delete_THaSlotData);
      instance.SetDeleteArray(&deleteArray_THaSlotData);
      instance.SetDestructor(&destruct_THaSlotData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaSlotData*)
   {
      return GenerateInitInstanceLocal((::THaSlotData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaSlotData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_THaEvData(void *p);
   static void deleteArray_THaEvData(void *p);
   static void destruct_THaEvData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaEvData*)
   {
      ::THaEvData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaEvData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaEvData", ::THaEvData::Class_Version(), "THaEvData.h", 28,
                  typeid(::THaEvData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaEvData::Dictionary, isa_proxy, 4,
                  sizeof(::THaEvData) );
      instance.SetDelete(&delete_THaEvData);
      instance.SetDeleteArray(&deleteArray_THaEvData);
      instance.SetDestructor(&destruct_THaEvData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaEvData*)
   {
      return GenerateInitInstanceLocal((::THaEvData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaEvData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaCodaDecoder(void *p = 0);
   static void *newArray_THaCodaDecoder(Long_t size, void *p);
   static void delete_THaCodaDecoder(void *p);
   static void deleteArray_THaCodaDecoder(void *p);
   static void destruct_THaCodaDecoder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaCodaDecoder*)
   {
      ::THaCodaDecoder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaCodaDecoder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaCodaDecoder", ::THaCodaDecoder::Class_Version(), "THaCodaDecoder.h", 18,
                  typeid(::THaCodaDecoder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaCodaDecoder::Dictionary, isa_proxy, 4,
                  sizeof(::THaCodaDecoder) );
      instance.SetNew(&new_THaCodaDecoder);
      instance.SetNewArray(&newArray_THaCodaDecoder);
      instance.SetDelete(&delete_THaCodaDecoder);
      instance.SetDeleteArray(&deleteArray_THaCodaDecoder);
      instance.SetDestructor(&destruct_THaCodaDecoder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaCodaDecoder*)
   {
      return GenerateInitInstanceLocal((::THaCodaDecoder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaCodaDecoder*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr THaUsrstrutils::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaUsrstrutils::Class_Name()
{
   return "THaUsrstrutils";
}

//______________________________________________________________________________
const char *THaUsrstrutils::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaUsrstrutils*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaUsrstrutils::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaUsrstrutils*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaUsrstrutils::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaUsrstrutils*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaUsrstrutils::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaUsrstrutils*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaCrateMap::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCrateMap::Class_Name()
{
   return "THaCrateMap";
}

//______________________________________________________________________________
const char *THaCrateMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCrateMap*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaCrateMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCrateMap*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCrateMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCrateMap*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCrateMap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCrateMap*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaCodaData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCodaData::Class_Name()
{
   return "THaCodaData";
}

//______________________________________________________________________________
const char *THaCodaData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCodaData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaCodaData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCodaData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCodaData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCodaData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCodaData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCodaData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaHelicity::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaHelicity::Class_Name()
{
   return "THaHelicity";
}

//______________________________________________________________________________
const char *THaHelicity::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaHelicity*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaHelicity::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaHelicity*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaHelicity::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaHelicity*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaHelicity::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaHelicity*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaEpics::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaEpics::Class_Name()
{
   return "THaEpics";
}

//______________________________________________________________________________
const char *THaEpics::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaEpics*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaEpics::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaEpics*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaEpics::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaEpics*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaEpics::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaEpics*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaFastBusWord::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaFastBusWord::Class_Name()
{
   return "THaFastBusWord";
}

//______________________________________________________________________________
const char *THaFastBusWord::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaFastBusWord*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaFastBusWord::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaFastBusWord*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaFastBusWord::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaFastBusWord*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaFastBusWord::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaFastBusWord*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaCodaFile::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCodaFile::Class_Name()
{
   return "THaCodaFile";
}

//______________________________________________________________________________
const char *THaCodaFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCodaFile*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaCodaFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCodaFile*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCodaFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCodaFile*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCodaFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCodaFile*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaSlotData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaSlotData::Class_Name()
{
   return "THaSlotData";
}

//______________________________________________________________________________
const char *THaSlotData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaSlotData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaSlotData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaSlotData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaSlotData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaSlotData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaSlotData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaSlotData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaEvData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaEvData::Class_Name()
{
   return "THaEvData";
}

//______________________________________________________________________________
const char *THaEvData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaEvData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaEvData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaEvData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr THaCodaDecoder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCodaDecoder::Class_Name()
{
   return "THaCodaDecoder";
}

//______________________________________________________________________________
const char *THaCodaDecoder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCodaDecoder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaCodaDecoder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaCodaDecoder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCodaDecoder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCodaDecoder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCodaDecoder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaCodaDecoder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void THaUsrstrutils::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaUsrstrutils.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaUsrstrutils::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaUsrstrutils::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaUsrstrutils(void *p) {
      return  p ? new(p) ::THaUsrstrutils : new ::THaUsrstrutils;
   }
   static void *newArray_THaUsrstrutils(Long_t nElements, void *p) {
      return p ? new(p) ::THaUsrstrutils[nElements] : new ::THaUsrstrutils[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaUsrstrutils(void *p) {
      delete ((::THaUsrstrutils*)p);
   }
   static void deleteArray_THaUsrstrutils(void *p) {
      delete [] ((::THaUsrstrutils*)p);
   }
   static void destruct_THaUsrstrutils(void *p) {
      typedef ::THaUsrstrutils current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaUsrstrutils

//______________________________________________________________________________
void THaCrateMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaCrateMap.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaCrateMap::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaCrateMap::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaCrateMap(void *p) {
      return  p ? new(p) ::THaCrateMap : new ::THaCrateMap;
   }
   static void *newArray_THaCrateMap(Long_t nElements, void *p) {
      return p ? new(p) ::THaCrateMap[nElements] : new ::THaCrateMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaCrateMap(void *p) {
      delete ((::THaCrateMap*)p);
   }
   static void deleteArray_THaCrateMap(void *p) {
      delete [] ((::THaCrateMap*)p);
   }
   static void destruct_THaCrateMap(void *p) {
      typedef ::THaCrateMap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaCrateMap

//______________________________________________________________________________
void THaCodaData::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaCodaData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaCodaData::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaCodaData::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_THaCodaData(void *p) {
      delete ((::THaCodaData*)p);
   }
   static void deleteArray_THaCodaData(void *p) {
      delete [] ((::THaCodaData*)p);
   }
   static void destruct_THaCodaData(void *p) {
      typedef ::THaCodaData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaCodaData

//______________________________________________________________________________
void THaHelicity::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaHelicity.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaHelicity::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaHelicity::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaHelicity(void *p) {
      return  p ? new(p) ::THaHelicity : new ::THaHelicity;
   }
   static void *newArray_THaHelicity(Long_t nElements, void *p) {
      return p ? new(p) ::THaHelicity[nElements] : new ::THaHelicity[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaHelicity(void *p) {
      delete ((::THaHelicity*)p);
   }
   static void deleteArray_THaHelicity(void *p) {
      delete [] ((::THaHelicity*)p);
   }
   static void destruct_THaHelicity(void *p) {
      typedef ::THaHelicity current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaHelicity

//______________________________________________________________________________
void THaEpics::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaEpics.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaEpics::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaEpics::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaEpics(void *p) {
      return  p ? new(p) ::THaEpics : new ::THaEpics;
   }
   static void *newArray_THaEpics(Long_t nElements, void *p) {
      return p ? new(p) ::THaEpics[nElements] : new ::THaEpics[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaEpics(void *p) {
      delete ((::THaEpics*)p);
   }
   static void deleteArray_THaEpics(void *p) {
      delete [] ((::THaEpics*)p);
   }
   static void destruct_THaEpics(void *p) {
      typedef ::THaEpics current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaEpics

//______________________________________________________________________________
void THaFastBusWord::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaFastBusWord.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaFastBusWord::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaFastBusWord::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaFastBusWord(void *p) {
      return  p ? new(p) ::THaFastBusWord : new ::THaFastBusWord;
   }
   static void *newArray_THaFastBusWord(Long_t nElements, void *p) {
      return p ? new(p) ::THaFastBusWord[nElements] : new ::THaFastBusWord[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaFastBusWord(void *p) {
      delete ((::THaFastBusWord*)p);
   }
   static void deleteArray_THaFastBusWord(void *p) {
      delete [] ((::THaFastBusWord*)p);
   }
   static void destruct_THaFastBusWord(void *p) {
      typedef ::THaFastBusWord current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaFastBusWord

//______________________________________________________________________________
void THaCodaFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaCodaFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaCodaFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaCodaFile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaCodaFile(void *p) {
      return  p ? new(p) ::THaCodaFile : new ::THaCodaFile;
   }
   static void *newArray_THaCodaFile(Long_t nElements, void *p) {
      return p ? new(p) ::THaCodaFile[nElements] : new ::THaCodaFile[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaCodaFile(void *p) {
      delete ((::THaCodaFile*)p);
   }
   static void deleteArray_THaCodaFile(void *p) {
      delete [] ((::THaCodaFile*)p);
   }
   static void destruct_THaCodaFile(void *p) {
      typedef ::THaCodaFile current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaCodaFile

//______________________________________________________________________________
void THaSlotData::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaSlotData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaSlotData::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaSlotData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaSlotData(void *p) {
      return  p ? new(p) ::THaSlotData : new ::THaSlotData;
   }
   static void *newArray_THaSlotData(Long_t nElements, void *p) {
      return p ? new(p) ::THaSlotData[nElements] : new ::THaSlotData[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaSlotData(void *p) {
      delete ((::THaSlotData*)p);
   }
   static void deleteArray_THaSlotData(void *p) {
      delete [] ((::THaSlotData*)p);
   }
   static void destruct_THaSlotData(void *p) {
      typedef ::THaSlotData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaSlotData

//______________________________________________________________________________
void THaEvData::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaEvData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaEvData::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaEvData::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_THaEvData(void *p) {
      delete ((::THaEvData*)p);
   }
   static void deleteArray_THaEvData(void *p) {
      delete [] ((::THaEvData*)p);
   }
   static void destruct_THaEvData(void *p) {
      typedef ::THaEvData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaEvData

//______________________________________________________________________________
void THaCodaDecoder::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaCodaDecoder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaCodaDecoder::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaCodaDecoder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaCodaDecoder(void *p) {
      return  p ? new(p) ::THaCodaDecoder : new ::THaCodaDecoder;
   }
   static void *newArray_THaCodaDecoder(Long_t nElements, void *p) {
      return p ? new(p) ::THaCodaDecoder[nElements] : new ::THaCodaDecoder[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaCodaDecoder(void *p) {
      delete ((::THaCodaDecoder*)p);
   }
   static void deleteArray_THaCodaDecoder(void *p) {
      delete [] ((::THaCodaDecoder*)p);
   }
   static void destruct_THaCodaDecoder(void *p) {
      typedef ::THaCodaDecoder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaCodaDecoder

namespace {
  void TriggerDictionaryInitialization_THaDecDict_Impl() {
    static const char* headers[] = {
"THaUsrstrutils.h",
"THaCrateMap.h",
"THaCodaData.h",
"THaHelicity.h",
"THaEpics.h",
"THaFastBusWord.h",
"THaCodaFile.h",
"THaSlotData.h",
"THaEvData.h",
"evio.h",
"THaCodaDecoder.h",
"THaBenchmark.h",
0
    };
    static const char* includePaths[] = {
"/adaqfs/apps/ROOT/pro/include",
"/adaqfs/apps/ROOT/pro/include",
"/adaqfs/apps/ROOT/6.12-06/include",
"/adaqfs/home/a-onl/tritium/scaler/hana_decode/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "THaDecDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(User string utilities, DAQ parsing code.)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaUsrstrutils.h")))  THaUsrstrutils;
class __attribute__((annotate(R"ATTRDUMP(Map of modules in DAQ crates)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaCrateMap.h")))  THaCrateMap;
class __attribute__((annotate(R"ATTRDUMP(Base class of CODA data (file, ET conn, etc))ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaCodaData.h")))  THaCodaData;
class __attribute__((annotate(R"ATTRDUMP(Beam helicity information.)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaHelicity.h")))  THaHelicity;
class __attribute__((annotate(R"ATTRDUMP(EPICS data)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaEpics.h")))  THaEpics;
class __attribute__((annotate(R"ATTRDUMP(Definitions for fastbus data standard)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaFastBusWord.h")))  THaFastBusWord;
class __attribute__((annotate(R"ATTRDUMP(File of CODA data)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaCodaFile.h")))  THaCodaFile;
class __attribute__((annotate(R"ATTRDUMP(Data in one slot of fastbus, vme, camac)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaSlotData.h")))  THaSlotData;
class __attribute__((annotate(R"ATTRDUMP(Decoder for CODA event buffer)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaEvData.h")))  THaEvData;
class __attribute__((annotate(R"ATTRDUMP(Decoder for CODA event buffer)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaCodaDecoder.h")))  THaCodaDecoder;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "THaDecDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef STANDALONE
  #define STANDALONE 1
#endif
#ifndef LINUXVERS
  #define LINUXVERS 1
#endif
#ifndef STANDALONE
  #define STANDALONE 1
#endif
#ifndef LINUXVERS
  #define LINUXVERS 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "THaUsrstrutils.h"
#include "THaCrateMap.h"
#include "THaCodaData.h"
#include "THaHelicity.h"
#include "THaEpics.h"
#include "THaFastBusWord.h"
#include "THaCodaFile.h"
#include "THaSlotData.h"
#include "THaEvData.h"
#include "evio.h"
#include "THaCodaDecoder.h"
#include "THaBenchmark.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"THaCodaData", payloadCode, "@",
"THaCodaDecoder", payloadCode, "@",
"THaCodaFile", payloadCode, "@",
"THaCrateMap", payloadCode, "@",
"THaEpics", payloadCode, "@",
"THaEvData", payloadCode, "@",
"THaFastBusWord", payloadCode, "@",
"THaHelicity", payloadCode, "@",
"THaSlotData", payloadCode, "@",
"THaUsrstrutils", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("THaDecDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_THaDecDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_THaDecDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_THaDecDict() {
  TriggerDictionaryInitialization_THaDecDict_Impl();
}
