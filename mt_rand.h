/*
   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __MERSENNE_TWISTER_MT_RAND_H
#define __MERSENNE_TWISTER_MT_RAND_H

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint32_t */

#define MT_RAND_N 624

typedef struct {
  /** The array for the state vector. */
  unsigned long mt[MT_RAND_N];

  /** mti==MT_RAND_N+1 means mt[MT_RAND_N] is not initialized */
  int mti;
} MTRandState;

MTRandState MTRandStateNew(unsigned long init_key[], int key_length);

/* generates a random number on [0,0xffffffff]-interval */
uint32_t genrand_int32(MTRandState *state);

/* generates a random number on [0,0x7fffffff]-interval */
int32_t genrand_int31(MTRandState *state);

/* generates a random number on [0,1]-real-interval */
double genrand_real1(MTRandState *state);

/* generates a random number on [0,1)-real-interval */
double genrand_real2(MTRandState *state);

/* generates a random number on (0,1)-real-interval */
double genrand_real3(MTRandState *state);

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(MTRandState *state);

/* These real versions are due to Isaku Wada, 2002/01/09 added */

#endif /* __MERSENNE_TWISTER_MT_RAND_H */
