#pragma once
#ifndef __SMODUDLE_H__
#define __SMODULE_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct ESSAYMachine;
	typedef struct ESSAYModule {
		s_size ocupiedSize;
		struct ESSAYMachine* machine;
		s_size typeCount;
		SType types[];
	}SModule;


#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

