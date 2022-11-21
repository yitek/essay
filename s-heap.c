#include <malloc.h>
#include <string.h>
#include "s-heap.h"

#ifdef STEST_INSPECT
s_size s_testHeapAllocSize = -1;
s_pointer s_testHeapAllocPointer = 0;
#endif

void* defaultNewObj(SHeap* self, const SType*const type) {
	s_size size = sizeof(SRef) + type->size;
	void* p = malloc(size);
#ifdef STEST_INSPECT
	s_testHeapAllocSize = size;
	s_testHeapAllocPointer = p;
#endif
	if (p == 0) return 0;
	//((SRefMeta*)p)->gc = 1;
	((SRef*)p)->type = type;
	return ((SRef*)p)->value;
}

void* defaultNewArr(SHeap* self, const SType* const type, s_size length) {
	const SType* itemType = s_type_itemType(type);
	s_size itemSize = s_type_stackSize(itemType);
	s_size size = sizeof(SRefMeta) + type->size + itemSize*length;
	void* p = malloc(size);
#ifdef STEST_INSPECT
	s_testHeapAllocSize = size;
	s_testHeapAllocPointer = p;
#endif

	if (p == 0) return 0;
	//((SRefMeta*)p)->gc = 1;
	((SRef*)p)->type = type;
	*((s_size*)((SRef*)p)->value) = length;
	/*if (isStr) {
		*((s_char*)((s_byte*)p + size) - 1) = 0;
	}*/
	return ((SRef*)p)->value;
}

void* defaultCopyObj(struct ESSAYHeap* const self, void* src, void* dest) {
	const SType* type = s_type(self);
	s_size size = type->size;
	if (s_type_flagArray(type)) {
		const SType* itemType = s_type_itemType(type);
		s_size itemSize = 
		size += *(s_size*)self * (s_type_itemType(type)->size);
	}
	memcpy_s(dest, size, src, size);
	return self;
}


void defaultDelobj(SHeap* self, void* any) {
	free((SRefMeta*)any-1);
}

void s_mem_copy( void* src, void* dest, s_size size) {
	memcpy_s(dest, size, src, size);
}

void s_mem_zero(void* target, s_size size) {
	memset(target, 0, size);
}

SHeap defaultHeap = { defaultDelobj, defaultNewObj ,defaultNewArr,defaultCopyObj };
SHeap* const s_defaultHeap = &defaultHeap;