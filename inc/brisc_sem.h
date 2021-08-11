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
#ifndef _BRISC_SEM_H_
#define _BRISC_SEM_H_

#include <cpu.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
    cpu_reg_t   lock;
    int32_t     count;
} brisc_sem_t;

#define BRISC_SEM_DECL(n,cnt)  brisc_sem_t n = {0,(cnt)}

/** ***************************************************************************
 * @brief Bring a sem to it's initial state (unlocked).
 * @param sem pointer to an initialized @ref brisc_sem_t variable. 
 * @param cnt initial token count. 
******************************************************************************/
#define b_sem_init(sem,cnt) { (sem)->lock=0; (sem)->count=(cnt); }

/** ***************************************************************************
 * @brief Block while acquiring sem token.
 * @param sem pointer to an initialized @ref brisc_sem_t variable. 
******************************************************************************/
extern void b_sem_acquire( brisc_sem_t* sem );

/** ***************************************************************************
 * @brief Non-vlocking acquiring sem token.
 * @param sem pointer to an initialized @ref brisc_sem_t variable. 
 * @return true if the token was acquired.
******************************************************************************/
extern bool b_sem_try_acquire( brisc_sem_t* sem );

/** ***************************************************************************
 * @brief Release a semaphore token.
 * @param sem pointer to an initialized @ref brisc_sem_t variable. 
******************************************************************************/
extern void b_sem_release( brisc_sem_t* sem );


#ifdef __cplusplus
}
#endif

#endif
