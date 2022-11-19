#include "s-heap.h"
#include <malloc.h>

void* defaultAlloc(SHeap* self, size_t size) {
	return malloc(size);
}

void* defaultFree(SHeap* self, void* any) {
	free(any);
}

SHeap defaultHeap = { defaultAlloc,defaultFree };
SHeap* const s_defaultHeap = &defaultHeap;