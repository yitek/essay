#pragma once
#ifndef __STYPE_H__
#define __STYPE_H__

#ifdef __cplusplus 
extern "C" {
#endif
#define STEST_INSPECT


#define s_size unsigned long long
#define s_byte unsigned char
#define s_char unsigned char
#define s_pointer void *

	// 值类型(In Stack)
#define STypeFlags_Ref		1


// 定义出来的类型
#define STypeFlags_Def		(1<<1)


// 指针
#define STypeFlags_Ptr		(1<<2)


// 函数
#define STypeFlags_Fnc		(1<<3 | STypeFlags_Ptr)
#define s_type_flagFnc(flag) (flag & STypeFlags_Fnc)

// 是否有/是虚函数
#define STypeFlags_Vir		(1<<4)
#define s_type_flagVir(flag) (flag & STypeFlags_Vir)

// 是可以下标访问(c风格数组,a[2] == a + 2)
#define STypeFlags_Arr		(1<<5)
#define s_type_flagArr(flag) (flag & STypeFlags_Arr)

// 是否是自动生成的
#define STypeFlags_Gen		(1<<6)
#define s_type_flagGen(flag) (flag & STypeFlags_Arr)

	struct ESSAYRefMeta;
	struct ESSAYRef;
	struct ESSAYModule;
	struct ESSAYType;
	
	struct ESSAYArrayMeta;
	struct ESSAYMachine;
	struct ESSAYHeap;
	

	typedef struct ESSAYRefMeta {
		void* gc;
		const struct ESSAYType* type;
	}SRefMeta;
	

	typedef struct ESSAYRef {
		void* gc;
		const struct ESSAYType* type;
		s_byte value[0];
	}SRef;

	typedef struct ESSAYArrayHeader {
		s_size length;
	}SArrayHeader;

	


	typedef struct ESSAYType {
		const s_size size;
		const unsigned int flags;
		const struct ESSAYType* baseType; // 基类
		const SArrayHeader* genericArguments; //泛型参数
		const struct ESSAYModule* module;
		const struct ESSAYTypeStr*const name; //类名
#ifndef __SMACHINE_H__
		struct ESSAYHeap* heap;
#endif
		const struct ESSAYMember** members;
	}SType;

#define s_type_flagByVal(type) ((type->flags & STypeFlags_Ref)==0)
#define s_type_flagByRef(type) (type->flags & STypeFlags_Ref)
#define s_type_flagDefined(type) (type->flags & STypeFlags_Def)
#define s_type_flagPrimitive(type) ((type->flags & STypeFlags_Def)==0)
#define s_type_flagPointer(type) (type->flags & STypeFlags_Ptr)
#define s_type_flagFunction(type) (type->flags & STypeFlags_Fnc)
#define s_type_flagVirtual(type) (type->flags & STypeFlags_Vir)
#define s_type_flagArray(type) (type->flags & STypeFlags_Arr)
#define s_type_flagGenerated(type) (type->flags & STypeFlags_Arr)

	extern const SType* const sVoid; 
	extern const SType* const sUnknown; // 未知的类型
	extern const SType* const sStruct; // 带成员的类型
	extern const SType* const sInt;
	extern const SType* const sChar;
	extern const SType* const sPointer;
	extern const SType* const sObject; // 带虚函数的对象
	
	

	inline const SRef* s_ref(const void* const any) { return ((SRef*)any) - 1; };

	inline const SType* s_type(const void* const any) { return (((SRef*)any) - 1)->type; };

	inline const int s_type_assignableFrom(const SType* const self, const SType* const other) { 
		if (other == 0) return 0;
		const SType* pType = other;
		while (pType) {
			if (pType == self) return 1;
			pType = pType->baseType;
		}
		return 0;
	};
	inline const SType* s_type_itemType(const SType* const type) {
		return *(SType**)(((SArrayHeader*)type->genericArguments) + 1);
	};
	
	inline const s_size s_type_stackSize(const SType* const type) {
		return s_type_flagByVal(type) ? type->size : sizeof(s_pointer);
	};



#ifdef __SMACHINE_H__
#define s_type_heap(type) (type->module->machine->heap)
#else
#define s_type_heap(type) (type->heap)
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__STYPE_H__

