
#include "test-array.h"

#include "test-heap.h"

typedef struct {
	s_size length;
	int items[5];
} IntArrayWith5Items;

typedef struct {
	s_size length;
	int items[10];
} IntArrayWith10Items;

void testSArrayConcat(SExpect expect) {
	//const SType* intArrayType = s_testCreateIntArrayType();
	//s_defaultHeap->newarr(s_defaultHeap,intArrayType,5);

}
void testSArraySlice(SExpect expect) {

}