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

	inline void* s_array_get(SArray* const self, s_size index) {
		if (index >= self->length) return 0;
		const SType* itemType = s_type_itemType(s_type(self));
		if (s_type_flagByRef(itemType)) return (void*)(self->buffer + sizeof(void*) * index);
		return  (void*)(self->buffer + s_type_stackSize(itemType) * index);
	}

	SArray* s_array_set(SArray* const self, s_size index, void* item);

	SArray* s_array_concat(SArray* const self, SArray* other);
	SArray* s_array_slice(SArray* const self, s_size start, s_size length);
#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

