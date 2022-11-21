#include "test-common.h"

#include <malloc.h>

void* s_testNewobj(SType* type, s_size size) {
	SRef* p = (SRef*)malloc(size);
	if (p == 0) return 0;
	p->gc = 0;
	p->type = type;
	return p->value;
}
void s_testDelobj(void* p) {
	free((SRefMeta*)p - 1);
}

const SType* s_testCreateArrayType(size_t itemSize, unsigned int itemByValOrRef, struct ESSAYHeap* heap) {
	SType* itemType = new SType{ itemSize ,itemByValOrRef,0,0,0,0,0 };
	STestArrayGenericTypesType* genericTypes = new STestArrayGenericTypesType{ 1, {itemType} };
	SType* arrType = new SType{
		sizeof(s_size) // size
		, STypeFlags_Arr// flags
		,0 // baseType
		,(const SArrayHeader*)genericTypes // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,heap
#endif
		,0 // members
	};
	return arrType;
}

void s_testDestroyArrayType(const SType* type) {
	delete ((STestArrayGenericTypesType*)type->genericArguments)->items[0];
	delete ((STestArrayGenericTypesType*)type->genericArguments);
	delete type;
}
