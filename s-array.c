#include "s-array.h"
#include "s-module.h"
#include "s-machine.h"
SArray* s_array_set(SArray* const self, s_size index, void* item) {
	if (index >= self->length) return 0;
	const SType* itemType = s_type_itemType(s_type((void*)self));
	s_byte* pValue = (s_byte*)(self->buffer + s_type_stackSize(itemType) * index);
	if (s_type_flagByRef(itemType)) {
		*(void**)pValue = item;
	}
	else {
		s_mem_copy(item, pValue, itemType->size);
	}
}

s_size s_array_index(SArray* const self, void* item) {
	return -1;
}


SArray* s_array_concat(SArray* const self, SArray* other) {
	if (self == 0) return other;
	if (other == 0) return self;
	s_size length = self->length + other->length;
	const SType* selfType = s_type(self);
	SHeap* heap = s_type_heap(selfType);
	const SType* selfItemType = s_type_itemType(selfType);
	const SType* otherItemType = s_type_itemType(s_type(other));
	if (!s_type_assignableFrom(selfItemType, otherItemType)) return 0;
	SArray* arr = (SArray*)heap->newarr(heap, selfType, length);
	int needStep = 0;
	if (s_type_flagByVal(selfItemType)) {
		if (selfItemType->size != otherItemType->size) needStep = 1;
	}	
	s_size itemSize = s_type_stackSize(selfItemType);
	s_size bytes = itemSize * self->length;
	s_mem_copy(self->buffer, arr->buffer, bytes);

	if (!needStep) {
		s_mem_copy(other->buffer, arr->buffer + bytes, itemSize * other->length);
	}
	else {
		s_size otherSize = otherItemType->size;
		s_byte* at = arr->buffer + bytes;
		s_size rest = itemSize - otherSize;
		if (rest>0) {
			for (int i = 0; i < other->length; i++) {
				
				s_mem_copy(other->buffer + otherSize * i, at, otherSize);
				s_mem_zero(at + otherSize, rest);
			}
		}
		else {
			for (int i = 0; i < other->length; i++) {
				at += itemSize * i;
				s_mem_copy(other->buffer + otherSize * i, at, otherSize);
				s_mem_zero(at + otherSize, rest);
			}
		}
		
	}
	
	return arr;
}

SArray* s_array_slice(SArray* const self, s_size start, s_size length) {
	if (self == 0) return 0;
	if (start + length >= self->length) return 0;
	const SType* type = s_type(self);
	s_size itemSize = s_type_stackSize(s_type_itemType(type));
	SHeap* heap = s_type_heap(type);
	SArray* arr = (SArray*)heap->newobj(heap,type,length);
	s_mem_copy(arr->buffer,self->buffer + start* itemSize,length* itemSize);

	return arr;
}