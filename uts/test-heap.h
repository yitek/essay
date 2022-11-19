#pragma once
#ifndef __TESTHEAP_H__
#define __TESTHEAP_H__
#include "../y-unit.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif

void testHeapDefaultInstance(YExpect expect);

inline YTestManager& testHeap() {
	y_unit("Heap", [](YTest test) {
		test("s_defaultHeap", testHeapDefaultInstance);
		});
	return y_unit;
};



#endif