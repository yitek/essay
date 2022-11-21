#pragma once
#ifndef __TESTARRAY_H__
#define __TESTARRAY_H__
#include "test-common.h"
#include "../s-array.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif
//
void testSArrayGetSet(SExpect expect ,int flag);
// void testSArrayConcat(SExpect expect);
// void testSArraySlice(SExpect expect);

inline STestManager& testSArray() {
	s_unit("SArray", [](STest subscribe) {
		subscribe("get/set", [](STest subscribe) {
			subscribe("byval", [](SExpect expect) {
				testSArrayGetSet(expect,0);
			});
			subscribe("byref", [](SExpect expect) {
				testSArrayGetSet(expect, STypeFlags_Ref);
				});
		});
		//subscribe("concat",testSArrayConcat);
		//subscribe("slice", testSArraySlice);
	});
	return s_unit;
};



#endif