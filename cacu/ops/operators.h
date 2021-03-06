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

#include "../config.h"

#include "../utils/check_utils.h"
#include "../utils/log.h"

#include "../math/math_functions.hpp"
#include "../math/activation_functions.hpp"
#include "../math/batch_functions.hpp"
#include "../math/pooling_functions.hpp"
#include "../math/loss_functions.hpp"
#include "../math/norm_functions.hpp"

#include "../tensor/utils/device_data_utils.hpp"
#include "../tensor/utils/cpu_data_utils.hpp"

#include "../utils/data_printer.h"

#include "operator_base.h"

#include "inner_product_op.h"
#include "convolution_op.h"
#include "batch_normalize_op.h"
#include "dropout_op.h"

#include "sum_elemwise_op.h"
#include "feature_combine_op.h"
#include "split_op.h"

#include "average_pooling_op.h"
#include "max_pooling_op.h"

#include "relu_op.h"
#include "prelu_op.h"
#include "leaky_relu_op.h"
#include "softmax_op.h"
#include "tanh_op.h"
#include "sigmoid_op.h"

#include "softmax_with_loss_op.h"
#include "mse_loss_op.h"

//#include "hinge_loss_op.h"

/***********************************/
/*        user operators	       */
/***********************************/

#include "../user_ops/p_inner_product_op.h"
#include "../user_ops/deconvolution_op.h"
#include "../user_ops/row_max_pooling_op.h"
//#include "../user_ops/local_conv_op.h"
#include "../user_ops/normalization_op.h"

#include "../user_ops/roi_pooling_op.h"
#include "../user_ops/injector_op.h"
#include "../user_ops/primary_vector_op.h"
