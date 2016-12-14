/*
Copyright (c) 2016, David lu
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once


extern "C" void cacu_sumbysize_gpu(SUM SUMTYPE ,float_t *x, int length, float_t *y, int width);

extern "C" void cacu_cxsize_gpu(float_t *x, int length, float_t *a, int size,float_t *y);

extern "C" void cacu_sxsize(float_t *x, int length, float_t a, float_t *y);

extern "C" void cacu_cdxsize(float_t *x, int length, float_t *a, int size, float_t *y);

extern "C" void cacu_sdxsize(float_t *x, int length, float_t a, float_t *y);

extern "C" void cacu_ssxpy(float_t *x, float_t a, int size, float_t *y, float_t b, int length, float_t *z);

extern "C" void cacu_sqr(float_t *x, int length, float_t *y);

extern "C" void cacu_root(float_t *x, int length, float_t *y);

extern "C" void cacu_stdbychannel(float_t *x, int length, float_t *y, float_t epsilon);


