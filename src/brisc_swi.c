/******************************************************************************
 ________  ________  ___  ________  ________  ___  _________  ________      
|\   __  \|\   __  \|\  \|\   ____\|\   ____\|\  \|\___   ___\\   ____\     
\ \  \|\ /\ \  \|\  \ \  \ \  \___|\ \  \___|\ \  \|___ \  \_\ \  \___|_    
 \ \   __  \ \   _  _\ \  \ \_____  \ \  \    \ \  \   \ \  \ \ \_____  \   
  \ \  \|\  \ \  \\  \\ \  \|____|\  \ \  \____\ \  \   \ \  \ \|____|\  \  
   \ \_______\ \__\\ _\\ \__\____\_\  \ \_______\ \__\   \ \__\  ____\_\  \ 
    \|_______|\|__|\|__|\|__|\_________\|_______|\|__|    \|__| |\_________\
                            \|_________|                        \|_________|
                            
MIT License

Copyright (c) 2021 Mike Sharkey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/
#include <brisc_irq.h>
#include <brisc_sched.h>

static int kcall_service(kcall_t fn,void* arg);


int brisc_swi(kcall_t fn, void* arg)
{
	return cpu_swi((cpu_reg_t)fn,(cpu_reg_t)arg);
}

static int brisc_swi_nop(void* arg)
{
	return 0;
}

static int brisc_swi_shmem_palloc(void* arg)
{
	return 0;
}

static int brisc_swi_shmem_pfree(void* arg)
{
	return 0;
}

static int brisc_swi_get_pattr(void* arg)
{
	return 0;
}

static int brisc_swi_set_pattr(void* arg)
{
	return 0;
}

extern int brisc_swi_service(cpu_reg_t reg_fn,cpu_reg_t reg_arg)
{
    brisc_swi_fn_t fn = (brisc_swi_fn_t)reg_fn;
    void* arg = (void*)reg_arg;
	switch(fn)
	{
        case BRISC_SWI_NOP=0:       return brisc_swi_nop(arg);
        case BRISC_SHMEM_PALLOC:    return brisc_swi_shmem_palloc(arg);
        case BRISC_SHMEM_PFREE:     return brisc_swi_shmem_pfree(arg);
        case BRISC_GET_PATTR:       return brisc_swi_get_pattr(arg);
        case BRISC_SET_PATTR:       return brisc_swi_set_pattr(arg);
	}
	return -1;
}

