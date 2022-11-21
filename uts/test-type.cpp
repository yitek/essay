#include "test-type.h"
#include "../s-type.h"
#include <malloc.h>


void testSType_ref(SExpect expect) {
	SType type = {
		sizeof(int) // size
		, STypeFlags_Arr// flags
		,0 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	void* obj = s_testNewobj(&type,40);
	const SRef* ref = s_ref(obj);
	expect(ref).toBe((SRef*)obj - 1).message("Ref struct is before the object pointer.");
	expect(ref->type).toBe(&type).message("Ref struct contains the type.");
	expect(ref->value).toBe(obj).message("Ref.value == object's address");
	s_testDelobj(obj);
}


void testSType_type(SExpect expect) {
	SType sType = {
		sizeof(int) // size
		, STypeFlags_Arr// flags
		,0 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	void* obj = s_testNewobj(&sType, 40);
	const SType* type = s_type(obj);
	expect(&sType).toBe(*((SType**)obj - 1)).message("type is before the object pointer.");
	expect(type).toBe(&sType).message("s_type()  return the type.");
	s_testDelobj(obj);
}


void testSType_itemType(SExpect expect) {
	const SType* arrType = s_testCreateArrayType(sizeof(int), STypeFlags_Ref, 0);
	const SType* itemType = s_type_itemType(arrType);
	expect(itemType).toBe(*((SArrayHeader**)arrType->genericArguments + 1)).message("The item type is at the arr type's genericTypes[0]");
	expect(itemType->size).toBe(sizeof(int)).message("size to be 4");
	expect((int)s_type_flagArray(arrType)).toBeTruthy().message("arr type has Arr flags");
}

void testSType_stackSize(SExpect expect) {
	SType type = {
		sizeof(int)*10 // size
		, STypeFlags_Ref// flags
		,0 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	s_size refSize = s_type_stackSize(&type);
	expect(refSize).toBe(sizeof(void*)).message("The byref's size = sizeof(void*)");
	*(int*)&type.flags = 0;
	s_size valSize = s_type_stackSize(&type);
	expect(valSize).toBe(type.size).message("The byval's size = type.size");
}

void testSType_assignableFrom(SExpect expect) {
	SType t1 = {
		sizeof(int) * 10 // size
		, STypeFlags_Ref// flags
		,0 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	SType t2 = {
		sizeof(int) * 10 // size
		, STypeFlags_Ref// flags
		,& t1 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	SType t3 = {
		sizeof(int) * 10 // size
		, STypeFlags_Ref// flags
		,&t2 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	SType t4 = {
		sizeof(int) * 10 // size
		, STypeFlags_Ref// flags
		,&t1 // baseType
		,0 // genericTyeps
		,0 // module
		,0 // name
#ifndef __SMACHINE_H__
		,0
#endif
		,0 // members
	};
	expect << "defined t2 extends t1,t3 extends t2; t4 extends t1.";
	expect(s_type_assignableFrom(&t1, &t3)).toBe(1).message("t1 can assignable from t3, t1=t3");
	expect(s_type_assignableFrom(&t1, &t2)).toBe(1).message("t1 can assignable from t2, t1=t2");
	expect(s_type_assignableFrom(&t2, &t3)).toBe(1).message("t2 can assignable from t3, t2=t3");
	expect(s_type_assignableFrom(&t1, &t4)).toBe(1).message("t1 can assignable from t4, t1=t4");
	expect(s_type_assignableFrom(&t4, &t2)).toBe(0).message("t4 can NOT assignable from t2, t4 x= t2");
	expect(s_type_assignableFrom(&t4, &t3)).toBe(0).message("t4 can NOT assignable from t3, t4 x= t3");
	expect(s_type_assignableFrom(&t3, &t2)).toBe(0).message("t3 can NOT assignable from t2, t4 x= t2");
}




