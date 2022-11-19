#pragma once
#ifndef __STYPE_H__
#define __STYPE_H__

#ifdef __cplusplus 
extern "C" {
#endif


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

	
	typedef struct ESSAYType;
	typedef struct ESSAYRef;
	typedef struct ESSAYTypeArray;
	typedef struct ESSAYTypeStr;
	typedef struct ESSAYMember;

	typedef struct ESSAYType {
		const s_size size;
		const unsigned int flags;
		const struct ESSAYTypeStr*const name; //����
		const struct ESSAYTypeArray* genericArguments; //���Ͳ���
		const struct ESSAYType* baseType; // ����
		const struct ESSAYMember** members;
	}SType;

	extern const SType* const sVoid; 
	extern const SType* const sUnknown; // δ֪������
	extern const SType* const sStruct; // ����Ա������
	extern const SType* const sInt;
	extern const SType* const sChar;
	extern const SType* const sPointer;
	extern const SType* const sObject; // ���麯���Ķ���
	

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

