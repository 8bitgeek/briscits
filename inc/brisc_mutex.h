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
#ifndef _BRISC_MUTEX_H_
#define _BRISC_MUTEX_H_

#include <cpu.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef cpu_reg_t   brisc_mutex_t;

#define BRISC_MUTEX_DECL(n)  brisc_mutex_t n = 0

/** ***************************************************************************
 * @brief Bring a mutex to it's initial state (unlocked).
******************************************************************************/
#define b_mutex_init(mutex)  b_mutex_unlock((mutex))

/** ***************************************************************************
 * @brief Block while acquiring mutex lock.
 * @param mutex pointer to an initialized @ref brisc_mutex_t variable. 
******************************************************************************/
extern void b_mutex_lock( brisc_mutex_t* mutex );

/** ***************************************************************************
 * @brief Non-vlocking acquiring mutex lock.
 * @param mutex pointer to an initialized @ref brisc_mutex_t variable. 
 * @return true if the lock was acquired.
******************************************************************************/
extern bool b_mutex_try_lock( brisc_mutex_t* mutex );

/** ***************************************************************************
 * @brief Un-lock a mutex.
 * @param mutex pointer to an initialized @ref brisc_mutex_t variable. 
******************************************************************************/
extern void b_mutex_unlock( brisc_mutex_t* mutex );

/** ***************************************************************************
 * @brief Un-lock a mutex with yield CPU after unlock.
 * @param mutex pointer to an initialized @ref brisc_mutex_t variable. 
******************************************************************************/
extern void b_mutex_release( brisc_mutex_t* mutex );

#ifdef __cplusplus
}
#endif

#endif
