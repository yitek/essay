#pragma once
#ifndef __SHEAP_H__
#define __SHEAP_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif
	struct ESSAYHeap;
	typedef void (*SHeapDeleteFunc)(struct ESSAYHeap* const self, void* any);
	typedef void* (*SHeapNewObjFunc)(struct ESSAYHeap*const self, const SType*const objType);
	typedef void* (*SHeapNewArrFunc)(struct ESSAYHeap* const self, const SType* const objType, s_size length);
	
	typedef void* (*SHeapCopyObjFunc)(struct ESSAYHeap* const self, void* source,void* dest);

	typedef struct ESSAYHeap {
		SHeapDeleteFunc delobj;
		SHeapNewObjFunc newobj;
		SHeapNewArrFunc newarr;
		SHeapCopyObjFunc copyobj;
	}SHeap;

	void s_mem_copy(void* src, void* des , s_size size);

	extern SHeap* const s_defaultHeap;

#ifdef STEST_INSPECT
	extern s_size s_testHeapAllocSize;
	extern s_pointer s_testHeapAllocPointer;
#endif

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

