#include "test-heap.h"
#include "../s-heap.h"

void testHeapDefaultInstance(YExpect expect) {
	/*int* pInt = (int*)s_defaultHeap->newobj(s_defaultHeap,);
	expect(pInt == 0 ? 0 : 1).toBe(1).message("Can allocate a block memory from heap and return the allocated memory address.");
	*pInt = 87 * 23;
	expect(*pInt).toBe(87 * 23).message("The address is a valid address.");
	s_defaultHeap->delobj(s_defaultHeap,pInt);
	expect(1).toBe(1).message("Can release the memory to heap by the address");*/
}

void testDefaultHeapNewObj(YExpect expect) {
	SType intType = { sizeof(int) };
	int* obj = (int*)s_defaultHeap->newobj(s_defaultHeap,&intType);
	
	expect(obj == 0 ? 0 : 1).toBe(1).message("Can allocate a block memory from heap and return the allocated memory address.");
	if (!obj) return;
	expect((void*)s_type(obj)).toBe(&intType).message("The object type is Int.");
	*obj = 87 * 23;
	expect(*obj).toBe(87 * 23).message("The address is a valid address.");
	s_defaultHeap->delobj(s_defaultHeap, obj);
	expect(1).toBe(1).message("Can release the memory to heap by the address");
}
typedef struct  {
	s_size length;
	s_byte buffer[0];
}STestArrayType;
void testDefaultHeapNewArr(YExpect expect) {
	SType intType = { sizeof(int) };

	SType intArrType = { sizeof(STestArrayType), STypeFlags_Indexable, };
	// int* obj = (int*)s_defaultHeap->newobj(s_defaultHeap, &intType);
}

