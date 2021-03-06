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
#include <brisc_swi.h>
#include <brisc_sched.h>

static cpu_reg_t brisc_swi_nop(cpu_reg_t arg);
static cpu_reg_t brisc_swi_get_pattr(cpu_reg_t arg);
static cpu_reg_t brisc_swi_set_pattr(cpu_reg_t arg);

/**
 * @brief Callback from the SVC Interrupt Service Routine. We enter
 *        this function in Interrupt Context.
 * @param fn The function code.
 * @param arg The function code argument.
 */
extern cpu_reg_t brisc_swi_service(cpu_reg_t reg_fn,cpu_reg_t reg_arg)
{
    brisc_swi_fn_t fn = (brisc_swi_fn_t)reg_fn;
    cpu_reg_t arg = reg_arg;
	switch(fn)
	{
        case BRISC_SWI_NOP:         return brisc_swi_nop(arg);
        case BRISC_GET_PATTR:       return brisc_swi_get_pattr(arg);
        case BRISC_SET_PATTR:       return brisc_swi_set_pattr(arg);
	}
	return -1;
}

extern cpu_reg_t brisc_swi(brisc_swi_fn_t fn, void* arg)
{
	return cpu_swi((cpu_reg_t)fn,arg);
}


static cpu_reg_t brisc_swi_nop(cpu_reg_t arg)
{
	return arg;
}

static cpu_reg_t brisc_swi_get_pattr(cpu_reg_t arg)
{
	return 0;
}

static cpu_reg_t brisc_swi_set_pattr(cpu_reg_t arg)
{
	return 0;
}

