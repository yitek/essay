#pragma once
#ifndef __TESTCOMMON_H__
#define __TESTCOMMON_H__
#include "../s-unit.h"
#include "../s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif

#define STEST

typedef struct {
	int id;
	int gender;
	int age;
}STestPerson;

typedef struct {
	SRefMeta meta;
	STestPerson person;
}STestHeapPerson;


typedef struct {
	s_size length;
	SType* items[2];
}STestArrayGenericTypesType;

const SType* s_testCreateArrayType(size_t itemSize, unsigned int itemByValOrRef, struct ESSAYHeap*);
void s_testDestroyArrayType(const SType* type);
void* s_testNewobj(SType* type, s_size size);
void s_testDelobj(void *p);




#endif