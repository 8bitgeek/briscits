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
#include "ftoa.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#if XPRINTF_USE_FLOAT
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

void ftoa(float f, char *str, size_t str_max, uint8_t precision) 
{
  uint8_t i, j, divisor = 1;
  int8_t log_f;
  int32_t int_digits = (int)f;             //store the integer digits
  float decimals;
  char s1[12];

  memset(str, 0, str_max);  
  memset(s1, 0, 10);

  if (f < 0) {                             //if a negative number 
    str[0] = '-';                          //start the char array with '-'
    f = abs((int)f);                       //store its positive absolute value
  }
  log_f = (int8_t)ceil(log10(f));          //get number of digits before the decimal
  if (log_f > 0) {                         //log value > 0 indicates a number > 1
    if (log_f == precision) {              //if number of digits = significant figures
      f += 0.5;                            //add 0.5 to round up decimals >= 0.5
      itoa((int)f, s1, 10);                //itoa converts the number to a char array
      strcat(str, s1);                     //add to the number string
    }
    else if ((log_f - precision) > 0) {    //if more integer digits than significant digits
      i = log_f - precision;               //count digits to discard
      divisor = 10;
      for (j = 0; j < i; j++) divisor *= 10;    //divisor isolates our desired integer digits 
      f /= divisor;                             //divide
      f += 0.5;                            //round when converting to int
      int_digits = (int)f;
      int_digits *= divisor;               //and multiply back to the adjusted value
      itoa(int_digits, s1, 10);
      strcat(str, s1);
    }
    else {                                 //if more precision specified than integer digits,
      itoa(int_digits, s1, 10);            //convert
      strcat(str, s1);                     //and append
    }
  }

  else {                                   //decimal fractions between 0 and 1: leading 0
    s1[0] = '0';
    strcat(str, s1);
  }

  if (log_f < precision) {                 //if precision exceeds number of integer digits,
    decimals = f - (int)f;                 //get decimal value as float
    strcat(str, ".");                      //append decimal point to char array

    i = precision - log_f;                 //number of decimals to read
    for (j = 0; j < i; j++) {              //for each,
      decimals *= 10;                      //multiply decimals by 10
      if (j == (i-1)) decimals += 0.5;     //and if it's the last, add 0.5 to round it
      itoa((int)decimals, s1, 10);         //convert as integer to character array
      strcat(str, s1);                     //append to string
      decimals -= (int)decimals;           //and remove, moving to the next
    }
  }
}

#if XPRINTF_USE_FLOAT
	#pragma GCC diagnostic pop
#endif
