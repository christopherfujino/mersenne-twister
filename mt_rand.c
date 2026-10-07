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

#include "mt_rand.h"

MTRandState MTRandStateNew() {
  return (MTRandState){
      .mti = MT_RAND_N + 1,
  };
}

void init_genrand(MTRandState *state, unsigned long s) {
  state->mt[0] = s & 0xffffffffUL;
  for (state->mti = 1; state->mti < MT_RAND_N; state->mti++) {
    state->mt[state->mti] =
        (1812433253UL *
             (state->mt[state->mti - 1] ^ (state->mt[state->mti - 1] >> 30)) +
         state->mti);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    state->mt[state->mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

void init_by_array(MTRandState *state, unsigned long init_key[],
                   int key_length) {
  unsigned int i = 1;
  int j = 0;
  int k = (MT_RAND_N > (unsigned int)key_length ? MT_RAND_N : key_length);
  init_genrand(state, 19650218UL);
  for (; k; k--) {
    state->mt[i] =
        (state->mt[i] ^
         ((state->mt[i - 1] ^ (state->mt[i - 1] >> 30)) * 1664525UL)) +
        init_key[j] + j;          /* non linear */
    state->mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    i++;
    j++;
    if (i >= MT_RAND_N) {
      state->mt[0] = state->mt[MT_RAND_N - 1];
      i = 1;
    }
    if (j >= key_length)
      j = 0;
  }
  for (k = MT_RAND_N - 1; k; k--) {
    state->mt[i] =
        (state->mt[i] ^
         ((state->mt[i - 1] ^ (state->mt[i - 1] >> 30)) * 1566083941UL)) -
        i;                        /* non linear */
    state->mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    i++;
    if (i >= MT_RAND_N) {
      state->mt[0] = state->mt[MT_RAND_N - 1];
      i = 1;
    }
  }

  state->mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

unsigned long genrand_int32(MTRandState *state) {
  unsigned long y;
  static unsigned long mag01[2] = {0x0UL, MATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (state->mti >= MT_RAND_N) { /* generate N words at one time */
    int kk;

    if (state->mti ==
        MT_RAND_N + 1)             /* if init_genrand() has not been called, */
      init_genrand(state, 5489UL); /* a default initial seed is used */

    for (kk = 0; kk < MT_RAND_N - M; kk++) {
      y = (state->mt[kk] & UPPER_MASK) | (state->mt[kk + 1] & LOWER_MASK);
      state->mt[kk] = state->mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < MT_RAND_N - 1; kk++) {
      y = (state->mt[kk] & UPPER_MASK) | (state->mt[kk + 1] & LOWER_MASK);
      state->mt[kk] =
          state->mt[kk + (M - MT_RAND_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (state->mt[MT_RAND_N - 1] & UPPER_MASK) | (state->mt[0] & LOWER_MASK);
    state->mt[MT_RAND_N - 1] = state->mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    state->mti = 0;
  }

  y = state->mt[state->mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

long genrand_int31(MTRandState *state) {
  return (long)(genrand_int32(state) >> 1);
}

double genrand_real1(MTRandState *state) {
  return genrand_int32(state) * (1.0 / 4294967295.0);
  /* divided by 2^32-1 */
}

double genrand_real2(MTRandState *state) {
  return genrand_int32(state) * (1.0 / 4294967296.0);
  /* divided by 2^32 */
}

double genrand_real3(MTRandState *state) {
  return (((double)genrand_int32(state)) + 0.5) * (1.0 / 4294967296.0);
  /* divided by 2^32 */
}

double genrand_res53(MTRandState *state) {
  unsigned long a = genrand_int32(state) >> 5, b = genrand_int32(state) >> 6;
  return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}
