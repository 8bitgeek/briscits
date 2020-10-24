/*
October 22nd, 2020
----------------------------------------------------------------------------
"THE BEER-WARE LICENSE" (Version 42):
<mike@pikeaero.com> wrote this file. As long as you retain this notice you
can do whatever you want with this stuff. If we meet some day, and you think
this stuff is worth it, you can buy me a beer in return ~ Mike Sharkey
---------------------------------------------------------------------------- 
*/
#ifndef BRISC_HEAP_H
#define BRISC_HEAP_H

#include <brisc_thread.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(BRISC_ANSI_HEAP)
    #define BRISC_ANSI_HEAP 1
#endif

#if (!defined(BRISC_NUMHEAPS) || BRISC_NUMHEAPS == 0)
    #define BRISC_NUMHEAPS 1
#endif

/** BITMAP_HEAP state variables. */
typedef struct
{
    /** Pointer to the bitmap of free blocks, 0=free, 1=used */
    uint32_t*       heap_free_bitmap;       
        
    /** Pointer to the bitmap marking the last contiguous block in each allocation */
    uint32_t*       heap_last_bitmap;           

    /** The size of each heap bitmap in bytes */
    int32_t         heap_bitmap_size;           

    /** Pointer to the first bytes of the usable heap memory */
    uint8_t*        heap_area;                  

    /** The size of the heap in blocks */
    int32_t         heap_blocks;
        
    /** The number of blocks in use (for fast statistics) */
    int32_t         heap_blocks_allocated;

    /** The mpu heap region flags */
    uint32_t        heap_flags;

} brisc_heap_state_t;

#if !defined(BRISC_NULL_HEAP)
    extern brisc_heap_state_t brisc_heap_state[];
#endif



/* FIXME - per thread initializer (prepare for PMP integration */
extern void     brisc_heap_init(void* heap_base, void* heap_end);



extern void*    brisc_heap_malloc(size_t sz);
extern void*    brisc_heap_realloc(void* p, size_t sz);
extern void*    brisc_heap_calloc(size_t nmemb, size_t size);
extern void     brisc_heap_free(void* p);
extern int32_t  brisc_heap_block_size(void);
extern int32_t  brisc_heap_blocks_allocated();
extern int32_t  brisc_heap_bytes_used();
extern int32_t  brisc_heap_bytes_free();
extern size_t   brisc_heap_sizeof(void* p);

#if BRISC_ANSI_HEAP
    extern void*    malloc(size_t sz);
    extern void*    realloc(void* p, size_t sz);
    extern void*    calloc(size_t nmemb, size_t size);
    extern void     free(void* p);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BRISC_HEAP_H */
