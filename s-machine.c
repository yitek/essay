#include "s-machine.h"
#include <malloc.h>
#include "s-type.h"



SMachine* s_machine_create(void* configuration) {
	SRef* r_machine = (SRef*)malloc(sizeof(SRef) + sizeof(SMachine));
	if (!r_machine) return 0;
	SMachine* machine= (SMachine*)r_machine->value;
	machine->heap = s_defaultHeap;
	return machine;
}