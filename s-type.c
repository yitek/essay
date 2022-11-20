#include "s-type.h"


typedef struct ESSAYTypeStrRef {
	void* gc;
	const SType* const type;
	s_size length;
	s_char chars[4];
}STypeStrRef4;

SType voidType;
const SType* const sVoid = &voidType;

SType unknowType;
const SType* const sUnknown = &voidType;

SType structType;
const SType* const sStruct = &structType;

SType classType;
const SType* const sClass = &classType;


SType intType = {.size = sizeof(int)};
const SType*const sInt = &intType;
STypeStrRef4 intName;


SType charType;
const SType* const sChar = &charType;

SType pointerType;
const SType* const sPointer = &pointerType;

SType strType;
const SType* const sStr = &strType;

SType arrType;
const SType* const sArray = &arrType;

SType listType;
const SType* const sList = &listType;

SType mapType;
const SType* const sMap = &mapType;

void init_types() {
	
}