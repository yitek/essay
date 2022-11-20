#include "s-array.h"
#include "s-module.h"
#include "s-machine.h"

SArray* array_concat(SArray* const self, SArray* other,s_size additionalLength) {
	if (self == 0) return other;
	if (other == 0) return self;
	s_size length = self->length + other->length;
	SType* type = s_type(self);
	SHeap* heap = s_typeHeap(type);
	SArray* arr = (SArray*)heap->newobj(heap, type, length + additionalLength);
	arr->length = length;
	//s_size selfSize = self->length * itemSize;
	//s_mem_copy(self->buffer, arr->buffer, selfSize);
	//s_mem_copy(other->buffer, arr->buffer + selfSize, other->length * itemSize);
	//*(byte*)(str->buffer + length) = 0;
	return arr;
}

SArray* array_slice(SArray* const self, s_size start, s_size length, s_size additionSize) {
	if (self == 0) return 0;
	if (start + length >= self->length) return 0;
	SType* type = s_type(self);
	s_size itemSize = ((SType*)((SArray*)type->genericArguments)->buffer)->size;
	SHeap* heap = s_typeHeap(type);

	SArray* arr = (SArray*)heap->newobj(heap,type,length + additionSize);
	arr->length = length;
	//size_t sliceSize = length * elemSize;
	// heap->copy(self->buffer + start * elemSize, arr->buffer, length * elemSize);
	//*(char*)(arr->buffer + str->length) = 0;
	return arr;
}