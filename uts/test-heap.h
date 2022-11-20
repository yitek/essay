#pragma once
#ifndef __TESTHEAP_H__
#define __TESTHEAP_H__
#include "../s-unit.h"
#include "../s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif



void testSHeap_default_newobj(SExpect expect);
void testSHeap_default_newarr(SExpect expect);

inline STestManager& testSHeap() {
	s_unit("SHeap", [](STest test) {
		test("s_defaultHeap", [](STest subscribe) {
			subscribe("newobj", testSHeap_default_newobj);
			subscribe("newarr", testSHeap_default_newarr);
	}	);
	});
	return s_unit;
};

typedef struct {
	s_size length;
	SType* items[1];
}STestArrayGenericTypesType;

const SType* s_testCreateIntArrayType();
void s_testDestroyIntArrayType(const SType* type);






#endif