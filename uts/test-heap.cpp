#include "test-heap.h"
#include "../s-heap.h"

void testHeapDefaultInstance(YExpect expect) {
	int* pInt = (int*)s_defaultHeap->alloc(s_defaultHeap,sizeof(int));
	expect(pInt == 0 ? 0 : 1).toBe(1).message("Can allocate a block memory from heap and return the allocated memory address.");
	*pInt = 87 * 23;
	expect(*pInt).toBe(87 * 23).message("The address is a valid address.");
	s_defaultHeap->free(s_defaultHeap,pInt);
	expect(1).toBe(1).message("Can release the memory to heap by the address");
}


