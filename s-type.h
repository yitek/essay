#pragma once
#ifndef __STYPE_H__
#define __STYPE_H__

#ifdef __cplusplus 
extern "C" {
#endif

// #define s_typeHeap(type) (type->module->machine->heap)

#define s_size unsigned long long
#define s_byte unsigned char
#define s_char unsigned char

// �������������
#define STypeFlags_Define		0
// ���õ�����
#define STypeFlags_Primary		1

// ֵ����(In Stack)
#define STypeFlags_ByVal		(0<<1)
// ��������(In Heap)
#define STypeFlags_ByRef		(1<<1)

// ��ָ����
#define STypeFlags_NonPointer	(0<<2)
// ָ��
#define STypeFlags_Pointer		(1<<2)
// ����
#define STypeFlags_Func		(1<<3 | STypeKinds_Pointer)
// �Ƿ���/���麯��
#define STypeFlags_Virtual		(1<<4)
// �Ƿ����Զ����ɵ�
#define STypeFlags_Generated		(1<<4)
// �ǿ����±����(c�������,a[2] == a + 2)
#define STypeFlags_Indexable		(1<<4)

	struct ESSAYRefMeta;
	struct ESSAYRef;
	struct ESSAYModule;
	struct ESSAYType;
	
	struct ESSAYTypeArray;
	struct ESSAYTypeStr;
	struct ESSAYMember;

	typedef struct ESSAYRefMeta {
		void* gc;
		const struct ESSAYType* type;
	}SRefMeta;
	

	typedef struct ESSAYRef {
		void* gc;
		const struct ESSAYType* type;
		s_byte value[0];
	}SRef;


	typedef struct ESSAYType {
		const s_size size;
		const unsigned int flags;
		const struct ESSAYType* baseType; // ����
		const struct ESSAYTypeArray* genericArguments; //���Ͳ���
		const struct ESSAYModule* module;
		const struct ESSAYTypeStr*const name; //����
		const struct ESSAYMember** members;
	}SType;

	extern const SType* const sVoid; 
	extern const SType* const sUnknown; // δ֪������
	extern const SType* const sStruct; // ����Ա������
	extern const SType* const sInt;
	extern const SType* const sChar;
	extern const SType* const sPointer;
	extern const SType* const sObject; // ���麯���Ķ���
	
	

	inline const SRef* s_ref(const void* const any) { return ((SRef*)any) - 1; };

	inline const SType* const  s_type(const void* const any) { return (((SRef*)any) - 1)->type; };
	
	inline const SType* const s_typeItemType(const SType* const type) {
		return (SType*)(((s_size*)type->genericArguments) + 1);
	};





#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__STYPE_H__

