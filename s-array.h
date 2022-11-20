#pragma once
#ifndef __SARRAY_H__
#define __SARRAY_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct ESSAYArray {
		s_size length;
		s_byte buffer[0];
	}SArray;

	inline s_size array_length(const SArray* const self) {
		return self->length;
	}

	inline void* array_at(SArray* const self, s_size at) {
		return (at >= self->length)?0:(void*)(self->buffer + ((SType*)((SArray*)s_type((void*)self)->genericArguments)->buffer)->size * at);
	}

	SArray* array_concat(SArray* const self, SArray* other);

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

