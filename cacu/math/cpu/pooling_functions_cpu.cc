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


#include "pooling_functions_cpu.h"

namespace cacu {

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_max_pooling_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int output_dim,
		const int channel, float_t *y, unsigned int* index) {
	int cout_length = output_dim * output_dim;
	int cin_length = input_dim * input_dim;
	float_t xd;
	int outset;
	int in_start, out_start;
	int i, j, c, ki, kj;
#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(i,j,c,ki,kj,in_start, out_start,outset,xd,xp)
#endif
	for (i = 0; i < output_dim; ++i)
		for (j = 0; j < output_dim; ++j) {
			for (c = 0; c < channel; ++c) {
				out_start = (i * output_dim + j);
				in_start = (i * input_dim + j) * stride;
				outset = c * cout_length + out_start;
				y[outset] = x[c * cin_length + in_start];
				index[outset] = (unsigned int) (in_start);
				for (ki = 0; ki < kernel_size && (ki + i * stride) < input_dim;
						++ki)
					for (kj = 0;
							kj < kernel_size && (kj + j * stride) < input_dim;
							++kj) {
						xd = x[ki * input_dim + kj + c * cin_length + in_start];
						if (y[outset] < xd) {
							y[outset] = xd;
							index[outset] = (unsigned int) (in_start
									+ ki * input_dim + kj);
						}
					}
			}
		}
}

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_max_pooling_grad_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int output_dim,
		const int channel, float_t *y, const unsigned int* index) {
	int sd_out;
	unsigned int _index;

	int cout_length = output_dim * output_dim;
	int cin_length = input_dim * input_dim;

	int i, j, c;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(i,j,c,_index, sd_out)
#endif
	for (i = 0; i < output_dim; ++i)
		for (j = 0; j < output_dim; ++j) {
			sd_out = (i * output_dim + j);
			for (c = 0; c < channel; ++c) {
				_index = index[sd_out + c * cout_length];
				y[_index + c * cin_length] += x[sd_out + c * cout_length];
			}
		}
}

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_average_pooling_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int output_dim,
		const int channel, float_t *y) {
	int block_size = output_dim * output_dim;
	float_t *yp;
	int in_start, out_start;
	int count;

	int i, j, c, ki, kj;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(i,j,c,ki,kj,in_start, out_start,count,xp,yp)
#endif
	for (c = 0; c < channel; ++c) {
		yp = y + c * block_size;
		for (i = 0; i < output_dim; ++i)
			for (j = 0; j < output_dim; ++j) {
				out_start = (i * output_dim + j);
				in_start = (i * input_dim + j) * stride;
				count = 0;
				for (ki = 0; ki < kernel_size && (ki + i * stride) < input_dim;
						ki++)
					for (kj = 0;
							kj < kernel_size && (kj + j * stride) < input_dim;
							kj++) {
						yp[out_start] += x[in_start + ki * input_dim + kj
								+ c * input_dim * input_dim];
						count++;
					}
				yp[out_start] /= count;
			}
	}
}

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_average_pooling_grad_cpu(const float_t *x,
		const int kernel_size, const int stride, const int input_dim,
		const int output_dim, const int channel, float_t *y) {
	int sd_out, sn_out, param_w, param_h;
	float_t *sn_out_cp;
	float_t diff_data;
	int flag = output_dim - 1;
	int pad = abs(input_dim - (output_dim - 1) * stride - kernel_size);

	int cin_length = input_dim * input_dim;
	int cout_length = output_dim * output_dim;

	int i, j, c, ki, kj;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(i,j,c,ki,kj,diff_data,sd_out_cp,sn_out_cp,param_w,param_h)
#endif
	for (i = 0; i < output_dim; ++i)
		for (j = 0; j < output_dim; ++j) {
			sd_out = (i * output_dim + j);
			sn_out = (i * input_dim + j) * stride;
			for (c = 0; c < channel; ++c) {
				//mean
				if (pad == 0) {
					diff_data = x[sd_out + c * cout_length]
							/ (float_t) (kernel_size * kernel_size);
					for (ki = 0; ki < kernel_size; ++ki)
						for (kj = 0; kj < kernel_size; ++kj) {
							sn_out_cp = y + sn_out + (ki * input_dim + kj)
									+ c * cin_length;
							*sn_out_cp += diff_data;
						}
				} else {
					param_w = kernel_size, param_h = kernel_size;
					if (i == flag)
						param_w = kernel_size - pad;
					if (j == flag)
						param_h = kernel_size - pad;
					diff_data = x[sd_out + c * cout_length]
							/ (float_t) (param_w * param_h);
					for (ki = 0; ki < param_w; ++ki)
						for (kj = 0; kj < param_h; ++kj) {
							sn_out_cp = y + sn_out + (ki * input_dim + kj)
									+ c * cin_length;
							*sn_out_cp += diff_data;
						}
				}

			}
		}
}

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *stride: stride move of the kernel
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_img2col_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int channel,
		const int output_dim, float_t *y) {
	int cin_length = input_dim * input_dim;
	int kernel_length = kernel_size * kernel_size;
	int block_size = kernel_length * channel;
	float_t *yp;
	int in_start, out_start;
	int i, j, c, ki, kj;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(i,j,c,ki,kj,yp,xp,out_start,in_start)
#endif
	for (i = 0; i < output_dim; ++i)
		for (j = 0; j < output_dim; ++j) {
			out_start = (i * output_dim + j) * block_size;
			in_start = (i * input_dim + j) * stride;

			for (c = 0; c < channel; ++c) {
				yp = y + out_start + c * kernel_length;

				for (ki = 0; ki < kernel_size; ki++)
					for (kj = 0; kj < kernel_size; ++kj) {
						yp[ki * kernel_size + kj] =
								x[in_start + c * cin_length];
					}
			}
		}
}

void cacu_img2col_pad_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int channel,
		const int output_dim, const int pad, float_t *y) {
	int cin_length = input_dim * input_dim;
	int kernel_length = kernel_size * kernel_size;
	int output_size = output_dim * output_dim;
	float_t *yp;
	int out_start;
	int i, j, c, ki, kj;
	int input_w, input_h, output_w, output_h;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(i,j,c,ki,kj,yp,xp,out_start,in_start)
#endif
	for (i = 0; i < output_dim; ++i)
		for (j = 0; j < output_dim; ++j) {
			out_start = (i * output_dim + j);
			output_h = i * stride;
			output_w = j * stride;

			for (c = 0; c < channel; ++c) {
				yp = y + c * kernel_length * output_size;
				for (ki = 0; ki < kernel_size; ++ki)
					for (kj = 0; kj < kernel_size; ++kj) {
						input_h = output_h + ki;
						input_w = output_w + kj;
						if (input_w >= pad && input_w < input_dim + pad
								&& input_h >= pad && input_h < input_dim + pad)
							yp[(ki * kernel_size + kj) * output_size + out_start] =
									x[(input_h - pad) * input_dim + input_w
											- pad + c * cin_length];
					}
			}
		}
}

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *stride: stride move of the kernel
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_col2img_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int channel,
		const int output_dim, float_t *y) {
	int sd_out, sn_out;

	int block_size = kernel_size * kernel_size * channel;
	int k_size = kernel_size * kernel_size;
	int cout_length = output_dim * output_dim;
	int cin_length = input_dim * input_dim;
	float_t *yp;

	int row, col, c, ki, kj;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(row,col,c,ki,kj,yp,xp,sd_out,sn_out)
#endif
	//for output_dim's location
	for (row = 0; row < output_dim; ++row) {
		for (col = 0; col < output_dim; ++col) {
			sd_out = (row * output_dim + col) * block_size;
			sn_out = (row * input_dim + col) * stride;
			for (c = 0; c < channel; ++c) {
				yp = y + sn_out + c * cin_length;
				for (ki = 0; ki < kernel_size; ++ki)
					for (kj = 0; kj < kernel_size; ++kj) {
						yp[ki * input_dim + kj] += x[ki * kernel_size + kj
								+ sd_out + c * k_size];
					}
			}
		}
	}
}

/*
 *channel: channel of input data
 *kernel_size: pooling window size
 *stride: stride move of the kernel
 *input_dim: width of input data
 *output_dim: width of output data
 */
void cacu_col2img_pad_cpu(const float_t *x, const int kernel_size,
		const int stride, const int input_dim, const int channel,
		const int output_dim, const int pad, float_t *y) {
	int sd_out, sn_out;

	int block_size = kernel_size * kernel_size * channel;
	int k_size = kernel_size * kernel_size;
	int cout_length = output_dim * output_dim;
	int cin_length = input_dim * input_dim;
	float_t *yp;

	int row, col, c, ki, kj;
	int input_h, input_w, output_h, output_w;

#if __OPENMP__ == ON
#pragma omp parallel for default(shared) private(row,col,c,ki,kj,yp,xp,sd_out,sn_out)
#endif
	//for output_dim's location
	for (row = 0; row < output_dim; ++row) {
		for (col = 0; col < output_dim; ++col) {

			output_h = row * stride;
			output_w = col * stride;
			sd_out = (row * output_dim + col);
			for (c = 0; c < channel; ++c) {
				yp = y + c * cin_length;
				for (ki = 0; ki < kernel_size; ++ki)
					for (kj = 0; kj < kernel_size; ++kj) {
						input_h = output_h + ki;
						input_w = output_w + kj;
						if (input_w >= pad && input_w < input_dim + pad
								&& input_h >= pad && input_h < input_dim + pad)
							yp[(input_h - pad) * input_dim + input_w - pad] +=
									x[(ki * kernel_size + kj + c * k_size)
											* cout_length + sd_out];
					}
			}
		}
	}
}

void cacu_row_max_pooling_cpu(float_t *x, const int input_length,
		const int output_length, float_t *y) {
	sort(x, x + input_length);
	for (int i = 0; i < output_length; ++i) {
		y[i] = x[i];
	}
}

void cacu_row_max_pooling_index_cpu(float_t *x, const int input_length,
		const int output_length, float_t *y, unsigned int* index) {
	for (int i = 0; i < output_length; ++i) {
		for (int j = 0; j < input_length; ++j)
			if (x[j] == y[i]) {
				index[i] = j;
				break;
			}
	}
}

void cacu_row_max_pooling_grad_cpu(const float_t *x,
		const int output_length, float_t *y, const unsigned int* index) {
	for (int i = 0; i < output_length; ++i) {
		y[index[i]] = x[i];
	}
}

}