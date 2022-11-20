#pragma once
#ifndef __SHEAP_H__
#define __SHEAP_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif
	struct ESSAYHeap;

	typedef void* (*SHeapNewObjFunc)(struct ESSAYHeap*const self, const SType*const objType);
	typedef void* (*SHeapNewArrFunc)(struct ESSAYHeap* const self, const SType* const arrType,s_size length);
	//typedef void* (*SHeapNewstrFunc)(struct ESSAYHeap* const self, s_size length);
	typedef void (*SHeapDeleteFunc)(struct ESSAYHeap*const self, void* any);

	typedef struct ESSAYHeap {
		SHeapDeleteFunc delobj;
		SHeapNewObjFunc newobj;
		SHeapNewArrFunc newarr;
		
	}SHeap;

	void s_mem_copy(void* des, void* src, s_size size);

	extern SHeap* const s_defaultHeap;
	

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

