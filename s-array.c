#include "s-array.h"
#include "s-module.h"
#include "s-machine.h"
SArray* s_array_set(SArray* const self, s_size index, void* item) {
	if (index >= self->length) return 0;
	const SType* itemType = s_typeItemType(s_type((void*)self));
	s_byte* pValue = (s_byte*)(self->buffer + s_type_byvalSize(itemType) * index);
	if (itemType->flags && STypeFlags_ByVal) {
		s_mem_copy(item, pValue, itemType->size);
	}
	else {
		*(void**)pValue = item;
	}
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
	heap->copyobj(heap, other, arr->buffer + self->length*s_type_byvalSize(selfItemType));
	heap->copyobj(heap, other,arr->buffer);
	return arr;
}

SArray* s_array_slice(SArray* const self, s_size start, s_size length) {
	if (self == 0) return 0;
	if (start + length >= self->length) return 0;
	const SType* type = s_type(self);
	s_size itemSize = s_type_byvalSize(s_type_itemType(type));
	SHeap* heap = s_type_heap(type);

	SArray* arr = (SArray*)heap->newobj(heap,type,length);
	s_mem_copy(arr->buffer,self->buffer + start* itemSize,length* itemSize);

	return arr;
}