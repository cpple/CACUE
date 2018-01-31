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

#include "math_definition.h"
#include "../config.h"
#include "../definition.h"

#include "cuda/activation_functions_cuda.h"
#include "cpu/activation_functions_cpu.h"

namespace cacu {

/**
 * @cacu_relu
 * math x[i] = max(0,x[i]) :
 * for activation use relu functions.
 */
inline void cacu_relu(float_t *x, int length) {

#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_relu_cuda(x, length);
#endif
#else
	cacu_relu_cpu(x,length);
#endif

}

/**
 * @cacu_relu_grad
 * math if(x[i]<0)?g[i] = g[i]:g[i] = 0;
 * gradient for activation use relu functions.
 */
inline void cacu_relu_grad(float_t *x, float_t *g, int length) {

#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_relu_grad_cuda(x, g, length);
#endif
#else
	cacu_relu_grad_cpu(x,g,length);
#endif

}

/**
 * @cacu_leaky_relu
 * math if(x[i]<0)?x[i] = x[i]:x[i] *= a;
 * for activation use leaky_relu functions.
 */
inline void cacu_leaky_relu(float_t *x, float_t a, int length) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_leaky_relu_cuda(x, a, length);
#endif
#else
	cacu_leaky_relu_cpu(x, a, length);
#endif

}

/**
 * @cacu_leaky_relu_grad
 * math if(x[i]<0)?g[i] = g[i]:g[i] *= a;
 * gradient for activation use leaky_relu functions.
 */
inline void cacu_leaky_relu_grad(float_t *x, float_t *g, float_t a,
		int length) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_leaky_relu_grad_cuda(x, g, a, length);
#endif
#else
	cacu_leaky_relu_grad_cpu(x, g, a, length);
#endif

}

/**
 * @cacu_softmax
 * math softmax;
 * for activation use softmax functions.
 */
inline void cacu_softmax(float_t *x, int num, int length, float_t *y) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_softmax_cuda(x, num, length, y);
#endif
#else
	cacu_softmax_cpu(x, num, length, y);
#endif

}

/**
 * @cacu_tanh
 * math tanh;
 * for activation use tanh functions.
 */
inline void cacu_tanh(float_t *x, int length, float_t *y) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_tanh_cuda(x, length, y);
#endif
#else
	cacu_tanh_cpu(x, length, y);
#endif

}

/**
 * @cacu_tanh_grad
 * math tanh;
 * gradient for activation use tanh functions.
 */
inline void cacu_tanh_grad(float_t *x, float_t *g, int length, float_t *y) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_tanh_grad_cuda(x, g, length, y);
#endif
#else
	cacu_tanh_grad_cpu(x, g, length, y);
#endif

}

/**
 * @cacu_sigmoid
 * math sigmoid;
 * for activation use sigmoid functions.
 */
inline void cacu_sigmoid(float_t *x, int length, float_t *y) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_sigmoid_cuda(x, length, y);
#endif
#else
	cacu_sigmoid_cpu(x, length, y);
#endif

}

/**
 * @cacu_tanh_grad
 * math tanh;
 * gradient for activation use tanh functions.
 */
inline void cacu_sigmoid_grad(float_t *x, float_t *g, int length, float_t *y) {
#if __USE_DEVICE__ == ON
#if __PARALLELTYPE__ == __CUDA__
	cacu_sigmoid_grad_cuda(x, g, length, y);
#endif
#else
	cacu_sigmoid_grad_cpu(x, g, length, y);
#endif

}

}