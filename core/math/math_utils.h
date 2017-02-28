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

#include "parallel_config.h"
#include "../../tools/random.h"

using namespace std;

#define CHECK_OP(level, format, ...) \
	do{fprintf(stderr,"[%s][%s %s:%d] " format "\n", level, __TIME__, __FILE__, __LINE__,##__VA_ARGS__);}while(0)

#define CHECK_EQ_OP(x,y,format,...)					\
	if (x != y){							\
			CHECK_OP("FATAL", format,##__VA_ARGS__, x, y);	\
			exit(0);						\
			}

#define CHECK_LT_OP(x,y,format,...)					\
	if (x >= y){							\
			CHECK_OP("FATAL", format,##__VA_ARGS__, x, y);	\
			exit(0);						\
			}

#define CHECK_LE_OP(x,y,format,...)					\
	if (x > y){								\
			CHECK_OP("FATAL", format,##__VA_ARGS__, x, y);	\
			exit(0);						\
			}

#define CHECK_GT_OP(x,y,format,...)					\
	if (x <= y){							\
			CHECK_OP("FATAL", format,##__VA_ARGS__, x, y);  \
			exit(0);						\
			}

#define CHECK_GE_OP(x,y,format,...)					\
	if (x < y){								\
			CHECK_OP("FATAL", format,##__VA_ARGS__, x, y);	\
			exit(0);						\
			}

typedef enum {

	TRANS,NOTRANS

} TRANSPOSE;

typedef enum {

	BYWIDTH, BYHEIGHT

} SUM;





