#pragma once
#ifndef __TESTARRAY_H__
#define __TESTARRAY_H__
#include "../s-unit.h"
#include "../s-array.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif
//
void testSArrayConcat(SExpect expect);
void testSArraySlice(SExpect expect);

inline STestManager& testSArray() {
	s_unit("SArray", [](STest subscribe) {
		subscribe("concat",testSArrayConcat);
		subscribe("slice", testSArraySlice);
	});
	return s_unit;
};



#endif