#pragma once
#ifndef __SMACHINE_H__
#define __SMACHINE_H__

#include "s-heap.h"

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct ESSAYMachine{
		SHeap* heap;

	}SMachine;

	inline SMachine* s_machine_create(void* configuration);


#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__SMACHINE_H__

