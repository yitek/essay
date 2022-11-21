
#include "test-array.h"

#include "test-heap.h"

typedef struct {
	int id;
	int gender;
	int age;
} SPerson;

typedef struct {
	s_size length;
	SPerson items[5];
} ArrayWith5Items;

typedef struct {
	s_size length;
	int items[10];
} IntArrayWith10Items;

void testSArrayGetSet(SExpect expect, int flag) {
	const SType* arrayType = s_testCreateArrayType(sizeof(SPerson), flag,s_defaultHeap);
	SArray* arr = (SArray*)s_defaultHeap->newarr(s_defaultHeap, arrayType, 2);
	SPerson people1[2] = {
		{1,2,3},{4,5,6}
	};
	s_array_set(arr, 0, &people1[1]);
	s_array_set(arr, 1, &people1[0]);

	SPerson* v0 = (SPerson*)s_array_get(arr, 0);
	SPerson* v1 = (SPerson*)s_array_get(arr, 1);
	expect(v0).toBe(arr+1).message("The first item's address is follow the field length.");
	if (flag & STypeFlags_Ref) {
		expect(v0 == &people1[1]).toBeTruthy().message("The GetValue return the original pointer");
		expect(v1 == &people1[0]).toBeTruthy().message("The  GetValue return the original pointer");
	}
	else {
		expect(v0 != &people1[1]).toBe(1).message("The address in array is different to orignal value");
		expect(v0->id).toBe(4).message("The v0.id == 4");
		expect(v0->gender).toBe(5).message("The v0.gender == 5");
		expect(v0->age).toBe(6).message("The v0.age == 6");

		expect(v1).toBe(arr->buffer + sizeof(SPerson)).message("The second item's address is follow the first item.");
		expect(v1->id).toBe(1).message("The v1.id == 1");
		expect(v1->gender).toBe(2).message("The v1.gender == 2");
		expect(v1->age).toBe(3).message("The v1.age == 3");
	}

	s_testDestroyArrayType(arrayType);

}

void testSArrayConcat(SExpect expect,int flag) {
	const SType* arrayType = s_testCreateArrayType(sizeof(SPerson),STypeFlags_Ref,s_defaultHeap);
	s_defaultHeap->newarr(s_defaultHeap,arrayType,5);
	s_testDestroyArrayType(arrayType);

}
void testSArraySlice(SExpect expect) {

}