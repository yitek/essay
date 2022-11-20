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
#define STypeFlags_Array		(1<<4)
#define STypeFlags_String		(1<<5)

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
		const struct ESSAYType* baseType; // ����
		const SArrayHeader* genericArguments; //���Ͳ���
		const struct ESSAYModule* module;
		const struct ESSAYTypeStr*const name; //����
#ifndef __SMACHINE_H__
		struct ESSAYHeap* heap;
#endif
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
		return (SType*)(((s_size*)type->genericArguments) + 1);
	};

	inline const s_size s_type_byvalSize(const SType* const type) {
		return (type->flags & STypeFlags_ByVal) ? type->size : sizeof(s_pointer);
	};
	
	inline const SType* s_typeItemType(const SType* const type) {
		return *(SType**)(((s_size*)type->genericArguments) + 1);
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

