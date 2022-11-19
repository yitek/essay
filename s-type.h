#pragma once
#ifndef __STYPE_H__
#define __STYPE_H__

#ifdef __cplusplus 
extern "C" {
#endif


#define s_size unsigned long long
#define s_byte unsigned char
#define s_char unsigned char

// 定义出来的类型
#define STypeFlags_Define		0
// 内置的类型
#define STypeFlags_Primary		1

// 值类型(In Stack)
#define STypeFlags_ByVal		(0<<1)
// 引用类型(In Heap)
#define STypeFlags_ByRef		(1<<1)

// 非指针数
#define STypeFlags_NonPointer	(0<<2)
// 指针
#define STypeFlags_Pointer		(1<<2)
// 函数
#define STypeFlags_Func		(1<<3 | STypeKinds_Pointer)
// 是否有/是虚函数
#define STypeFlags_Virtual		(1<<4)
// 是否是自动生成的
#define STypeFlags_Generated		(1<<4)
// 是可以下标访问(c风格数组,a[2] == a + 2)
#define STypeFlags_Indexable		(1<<4)

	
	typedef struct ESSAYType;
	typedef struct ESSAYRef;
	typedef struct ESSAYTypeArray;
	typedef struct ESSAYTypeStr;
	typedef struct ESSAYMember;

	typedef struct ESSAYType {
		const s_size size;
		const unsigned int flags;
		const struct ESSAYTypeStr*const name; //类名
		const struct ESSAYTypeArray* genericArguments; //泛型参数
		const struct ESSAYType* baseType; // 基类
		const struct ESSAYMember** members;
	}SType;

	extern const SType* const sVoid; 
	extern const SType* const sUnknown; // 未知的类型
	extern const SType* const sStruct; // 带成员的类型
	extern const SType* const sInt;
	extern const SType* const sChar;
	extern const SType* const sPointer;
	extern const SType* const sObject; // 带虚函数的对象
	

	typedef struct ESSAYRef {
		void* gc;
		const SType* type;
		s_byte value[];
	}SRef;


	inline const SRef* s_ref(const void*const any) { return ((SRef*)any) - 1;}

	inline const SType*  s_type(const void*const any) { return (((SRef*)any) - 1)->type; }
	






#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__STYPE_H__

