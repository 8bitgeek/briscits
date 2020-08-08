#include "cpu.h"

void* __attribute__((naked)) cpu_rd_sp(void)
{
	__asm ( " mv 	a0, sp			\n"
			" ret   		\n" );
	return 0;
}
