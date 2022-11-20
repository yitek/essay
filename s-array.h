#pragma once
#ifndef __SARRAY_H__
#define __SARRAY_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct ESSAYArray {
		const s_size length;
		s_byte buffer[0];
	}SArray;

	inline s_size s_array_length(const SArray* const self) {
		return self->length;
	}

	inline void* s_array_at(SArray* const self, s_size at) {
		return (at >= self->length)?0:(void*)(self->buffer + s_typeItemType(s_type((void*)self))->size * at);
	}

	SArray* s_array_concat(SArray* const self, SArray* other);
	SArray* s_array_slice(SArray* const self, s_size start, s_size length);
#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

