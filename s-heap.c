#include "s-heap.h"
#include <malloc.h>
#include <string.h>

void* defaultNewObj(SHeap* self, const SType*const type) {
	//s_size size = sizeof(SRefMeta) + type->size;
	void* p = malloc(sizeof(SRefMeta) + type->size);
	if (p == 0) return 0;
	//((SRefMeta*)p)->gc = 1;
	((SRef*)p)->type = type;
	return ((SRef*)p)->value;
}

void* defaultNewArr(SHeap* self, const SType* const type, s_size length) {
	void* p = malloc(sizeof(SRefMeta) + type->size + s_typeItemType(type)->size * length);
	if (p == 0) return 0;
	//((SRefMeta*)p)->gc = 1;
	((SRef*)p)->type = type;
	*((s_size*)((SRef*)p)->value) = length;
	return ((SRef*)p)->value;
}


void defaultDelobj(SHeap* self, void* any) {
	free((SRefMeta*)any-1);
}

void s_mem_copy(void* dest, void* src,s_size size) {
	memcpy_s(dest, size, src, size);
}

SHeap defaultHeap = { defaultDelobj, defaultNewObj ,defaultNewArr };
SHeap* const s_defaultHeap = &defaultHeap;