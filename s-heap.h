#pragma once
#ifndef __SHEAP_H__
#define __SHEAP_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif
	struct ESSAYHeap;

	typedef void* (*SHeapAllocFunc)(struct ESSAYHeap*const self, size_t size);
	typedef void* (*SHeapFreeFunc)(struct ESSAYHeap*const self, void* any);

	typedef struct ESSAYHeap {
		SHeapAllocFunc alloc;
		SHeapFreeFunc free;
	}SHeap;
	extern SHeap*const s_defaultHeap;


#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

