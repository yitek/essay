#pragma once
#ifndef __SARRAY_H__
#define __SARRAY_H__

#include "s-type.h"

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct ESSAYArray {
		size_t length;
		byte_t buffer[0];
	}SArray;

	inline size_t array_length(const SArray* const self) {
		return self->length;
	}

	inline void* array_at(SArray* const self, size_t at) {
		return (at >= self->length)?0:(void*)(self->buffer + ((SType*)((SArray*)s_type((void*)self)->genericArguments)->buffer)->size * at);
	}

	inline SArray* array_concat(SArray* const self, SArray* other) {
		if (self == 0) return other;
		if (other == 0) return self;
		size_t length = self->length + other->length;
		Array* arr = (Array*)heap->alloc(sizeof(Array) + length * elemSize + additionSize);
		arr->length = length;
		size_t selfSize = self->length * elemSize;
		heap->copy(self->buffer, arr->buffer, selfSize);
		heap->copy(other->buffer, arr->buffer + selfSize, other->length * elemSize);
		//*(byte*)(str->buffer + length) = 0;
		return arr;
	}

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

