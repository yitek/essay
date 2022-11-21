#pragma once
#ifndef __TESTHEAP_H__
#define __TESTHEAP_H__
#include "test-common.h"
#include "../s-heap.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif



void testSHeap_default_newobj(SExpect expect);
void testSHeap_default_newarr(SExpect expect, int flag);
inline void testSHeap_default_newarr_byval_item(SExpect expect) {
	testSHeap_default_newarr(expect,0);
}
inline void testSHeap_default_newarr_byref_item(SExpect expect) {
	testSHeap_default_newarr(expect, STypeFlags_Ref);
}

inline STestManager& testSHeap() {
	s_unit("SHeap", [](STest test) {
		test("s_defaultHeap", [](STest subscribe) {
			subscribe("newobj", testSHeap_default_newobj);
			subscribe("newarr", [](STest subscribe) {
				subscribe("byval-item", testSHeap_default_newarr_byval_item);
				subscribe("byref-item", testSHeap_default_newarr_byref_item);
			});
	}	);
	});
	return s_unit;
};







#endif