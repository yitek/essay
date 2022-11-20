#pragma once
#ifndef __TESTARRAY_H__
#define __TESTARRAY_H__
#include "../s-unit.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif
//
void testSArrayConcat(SExpect expect);
void testSArraySlice(SExpect expect);

inline STestManager& testArray() {
	s_unit("Array", [](STest subscribe) {
		subscribe("Concat",testSArrayConcat);
		subscribe("Slice", testSArraySlice);
	});
	return s_unit;
};



#endif