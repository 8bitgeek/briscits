#include <brisc_board.h>
#include <string.h>

// Pre-defined memory locations for program initialization.
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

void bss_init( void ) 
{
    // Copy initialized data from .sidata (Flash) to .data (RAM)
    memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
    // Clear the .bss RAM section.
    memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );
}