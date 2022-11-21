#pragma once
#ifndef __TESTSTYPE_H__
#define __TESTSTYPE_H__
#include "../s-unit.h"
#include "../s-type.h"
#include "test-common.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif



void testSType_ref(SExpect expect);
void testSType_type(SExpect expect);
void testSType_itemType(SExpect expect);
void testSType_stackSize(SExpect expect);
void testSType_assignableFrom(SExpect expect);

inline STestManager& testSType() {
	s_unit("SType", [](STest test) {
		test("s_ref", testSType_ref);
		test("s_type", testSType_type);
		test("s_type_itemType", testSType_itemType);
		test("s_type_stackSize", testSType_stackSize);
		test("s_type_assignableFrom", testSType_assignableFrom);
	});
	return s_unit;
};






#endif