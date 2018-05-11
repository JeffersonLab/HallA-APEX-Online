// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME THaScalDict

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
#include "THaScaler.h"
#include "THaScalerDB.h"
#include "THaScalerGui.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *THaScaler_Dictionary();
   static void THaScaler_TClassManip(TClass*);
   static void delete_THaScaler(void *p);
   static void deleteArray_THaScaler(void *p);
   static void destruct_THaScaler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaScaler*)
   {
      ::THaScaler *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::THaScaler));
      static ::ROOT::TGenericClassInfo 
         instance("THaScaler", "THaScaler.h", 38,
                  typeid(::THaScaler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &THaScaler_Dictionary, isa_proxy, 4,
                  sizeof(::THaScaler) );
      instance.SetDelete(&delete_THaScaler);
      instance.SetDeleteArray(&deleteArray_THaScaler);
      instance.SetDestructor(&destruct_THaScaler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaScaler*)
   {
      return GenerateInitInstanceLocal((::THaScaler*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaScaler*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *THaScaler_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::THaScaler*)0x0)->GetClass();
      THaScaler_TClassManip(theClass);
   return theClass;
   }

   static void THaScaler_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_THaScalerDB(void *p = 0);
   static void *newArray_THaScalerDB(Long_t size, void *p);
   static void delete_THaScalerDB(void *p);
   static void deleteArray_THaScalerDB(void *p);
   static void destruct_THaScalerDB(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaScalerDB*)
   {
      ::THaScalerDB *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaScalerDB >(0);
      static ::ROOT::TGenericClassInfo 
         instance("THaScalerDB", ::THaScalerDB::Class_Version(), "THaScalerDB.h", 230,
                  typeid(::THaScalerDB), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaScalerDB::Dictionary, isa_proxy, 4,
                  sizeof(::THaScalerDB) );
      instance.SetNew(&new_THaScalerDB);
      instance.SetNewArray(&newArray_THaScalerDB);
      instance.SetDelete(&delete_THaScalerDB);
      instance.SetDeleteArray(&deleteArray_THaScalerDB);
      instance.SetDestructor(&destruct_THaScalerDB);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaScalerDB*)
   {
      return GenerateInitInstanceLocal((::THaScalerDB*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::THaScalerDB*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr THaScalerDB::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *THaScalerDB::Class_Name()
{
   return "THaScalerDB";
}

//______________________________________________________________________________
const char *THaScalerDB::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaScalerDB*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int THaScalerDB::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaScalerDB*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaScalerDB::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaScalerDB*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaScalerDB::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaScalerDB*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_THaScaler(void *p) {
      delete ((::THaScaler*)p);
   }
   static void deleteArray_THaScaler(void *p) {
      delete [] ((::THaScaler*)p);
   }
   static void destruct_THaScaler(void *p) {
      typedef ::THaScaler current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaScaler

//______________________________________________________________________________
void THaScalerDB::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaScalerDB.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaScalerDB::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaScalerDB::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaScalerDB(void *p) {
      return  p ? new(p) ::THaScalerDB : new ::THaScalerDB;
   }
   static void *newArray_THaScalerDB(Long_t nElements, void *p) {
      return p ? new(p) ::THaScalerDB[nElements] : new ::THaScalerDB[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaScalerDB(void *p) {
      delete ((::THaScalerDB*)p);
   }
   static void deleteArray_THaScalerDB(void *p) {
      delete [] ((::THaScalerDB*)p);
   }
   static void destruct_THaScalerDB(void *p) {
      typedef ::THaScalerDB current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::THaScalerDB

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 210,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *multimaplEstringcOintgR_Dictionary();
   static void multimaplEstringcOintgR_TClassManip(TClass*);
   static void *new_multimaplEstringcOintgR(void *p = 0);
   static void *newArray_multimaplEstringcOintgR(Long_t size, void *p);
   static void delete_multimaplEstringcOintgR(void *p);
   static void deleteArray_multimaplEstringcOintgR(void *p);
   static void destruct_multimaplEstringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const multimap<string,int>*)
   {
      multimap<string,int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(multimap<string,int>));
      static ::ROOT::TGenericClassInfo 
         instance("multimap<string,int>", -2, "map", 95,
                  typeid(multimap<string,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &multimaplEstringcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(multimap<string,int>) );
      instance.SetNew(&new_multimaplEstringcOintgR);
      instance.SetNewArray(&newArray_multimaplEstringcOintgR);
      instance.SetDelete(&delete_multimaplEstringcOintgR);
      instance.SetDeleteArray(&deleteArray_multimaplEstringcOintgR);
      instance.SetDestructor(&destruct_multimaplEstringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< multimap<string,int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const multimap<string,int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *multimaplEstringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const multimap<string,int>*)0x0)->GetClass();
      multimaplEstringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void multimaplEstringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_multimaplEstringcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<string,int> : new multimap<string,int>;
   }
   static void *newArray_multimaplEstringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) multimap<string,int>[nElements] : new multimap<string,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_multimaplEstringcOintgR(void *p) {
      delete ((multimap<string,int>*)p);
   }
   static void deleteArray_multimaplEstringcOintgR(void *p) {
      delete [] ((multimap<string,int>*)p);
   }
   static void destruct_multimaplEstringcOintgR(void *p) {
      typedef multimap<string,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class multimap<string,int>

namespace {
  void TriggerDictionaryInitialization_THaScalDict_Impl() {
    static const char* headers[] = {
"THaScaler.h",
"THaScalerDB.h",
"THaScalerGui.h",
0
    };
    static const char* includePaths[] = {
"/adaqfs/apps/ROOT/pro/include",
"../hana_decode",
"/adaqfs/apps/ROOT/6.12-06/include",
"/adaqfs/home/a-onl/tritium/scaler/hana_scaler/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "THaScalDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$THaScaler.h")))  THaScaler;
class __attribute__((annotate(R"ATTRDUMP(Text-based time-dependent database for scaler map and directives)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$THaScalerDB.h")))  THaScalerDB;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "THaScalDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "THaScaler.h"
#include "THaScalerDB.h"
#include "THaScalerGui.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"THaScaler", payloadCode, "@",
"THaScalerDB", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("THaScalDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_THaScalDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_THaScalDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_THaScalDict() {
  TriggerDictionaryInitialization_THaScalDict_Impl();
}
