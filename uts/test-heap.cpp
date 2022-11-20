#include "test-heap.h"
#include "../s-heap.h"

void testHeapDefaultInstance(SExpect expect) {
	/*int* pInt = (int*)s_defaultHeap->newobj(s_defaultHeap,);
	expect(pInt == 0 ? 0 : 1).toBe(1).message("Can allocate a block memory from heap and return the allocated memory address.");
	*pInt = 87 * 23;
	expect(*pInt).toBe(87 * 23).message("The address is a valid address.");
	s_defaultHeap->delobj(s_defaultHeap,pInt);
	expect(1).toBe(1).message("Can release the memory to heap by the address");*/
}

void testSHeap_default_newobj(SExpect expect) {
	SType intType = { sizeof(int) };
	int* obj = (int*)s_defaultHeap->newobj(s_defaultHeap,&intType);
	expect((s_size)s_testHeapAllocSize).toBe(intType.size + sizeof(SRef)).message("Allocated size=SRef + Type.size");
	expect(obj).toBe((SRef*)s_testHeapAllocPointer + 1).message("Object address =Allocated address + SRef");

	
	expect(obj == 0 ? 0 : 1).toBe(1).message("Can allocate a block memory from heap and return the allocated memory address.");
	if (!obj) return;
	expect((void*)s_type(obj)).toBe(&intType).message("The object type is Int.");
	*obj = 87 * 23;
	expect(*obj).toBe(87 * 23).message("The address is a valid address.");
	s_defaultHeap->delobj(s_defaultHeap, obj);
	expect(1).toBe(1).message("Can release the memory to heap by the address");
}

#define STEST_HEAP_ARRAY_LENGTH 5


typedef struct {
	s_size length;
	int items[5];
}STestIntArray5Items;

const SType* s_testCreateIntArrayType(size_t itemSize, unsigned int itemByValOrRef) {
	SType* itemType = new SType{ itemSize ,itemByValOrRef,0,0,0,0,0 };
	STestArrayGenericTypesType* genericTypes = new STestArrayGenericTypesType{ 1, {itemType} };
	SType* arrType = new SType{
		sizeof(s_size) // size
		, STypeFlags_Array // flags
		,0 // baseType
		,(const SArrayHeader*)genericTypes // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,s_defaultHeap
#endif
		,0 // members
	};
	return arrType;
}

void s_testDestroyIntArrayType(const SType* type) {
	delete ((STestArrayGenericTypesType*)type->genericArguments)->items[0];
	delete ((STestArrayGenericTypesType*)type->genericArguments);
	delete type;
}

void testSHeap_default_newarr(SExpect expect,int flag) {
	s_size itemSize = sizeof(void*)*3;
	const SType* arrType = s_testCreateIntArrayType(itemSize, flag);
	
	
	void* obj = (int*)s_defaultHeap->newarr(s_defaultHeap, arrType, STEST_HEAP_ARRAY_LENGTH);

	s_size expectedSize = sizeof(SRef) + sizeof(s_size) + STEST_HEAP_ARRAY_LENGTH * (flag && STypeFlags_ByRef?sizeof(void*):itemSize);

	if (flag && STypeFlags_ByRef) {
		expect((s_size)s_testHeapAllocSize).toBe(expectedSize).message("Allocated size(byref-item)=sizeof(SRef) + sizeof(Array) + item-count * sizeof(void*)");
	}
	else {
		expect((s_size)s_testHeapAllocSize).toBe(expectedSize).message("Allocated size(byval-item)=sizeof(SRef) + sizeof(Array) + item-count * sizeItem");
	}
	expect(obj).toBe((SRef*)s_testHeapAllocPointer+1).message("Object address =Allocated address + SRef");
	expect(((SArrayHeader*)obj)->length).toBe(STEST_HEAP_ARRAY_LENGTH).message("The const attribute length is preset.");

	s_defaultHeap->delobj(s_defaultHeap, obj);
	expect(1).toBe(1).message("Can release the memory to heap by the address");
	s_testDestroyIntArrayType(arrType);
}



