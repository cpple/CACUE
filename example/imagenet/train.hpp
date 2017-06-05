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


#include <time.h>
#include <sys/time.h>
#include "../../mycnn.h"

#include "../../tools/imageio_utils.h"

#include "alex_net.h"
#include "vgg_net.h"
#include "data_proc.h"
#include "resnet_18.h"
#include "resnet_50.h"
#include "mobilenet.h"

void train_net()
{
	int batch_size = 64;

	int max_iter = 200000;

	int test_iter = 10;

	//set gpu device if training by gpu
#if __PARALLELTYPE__ == __CUDA__
	cuda_set_device(0);
#endif

	//log output
	std::ofstream logger("/home/seal/4T/cacue/imagenet/mobilenet.log", ios::binary);
	logger.precision(std::numeric_limits<mycnn::float_t>::digits10);

	//log output
	std::ofstream precious_logger("/home/seal/4T/cacue/imagenet/mobilenet_precious.log", ios::binary);
	precious_logger.precision(std::numeric_limits<mycnn::float_t>::digits10);

	network *net = create_mobilenet(batch_size,train);//create_mobilenet(batch_size,train);//create_vgg_16_net(batch_size,train);//create_alexnet(batch_size,train);

	net->load_weights("/home/seal/4T/cacue/imagenet/mobilenet.model");	//net->load_weights("/home/seal/4T/cacue/imagenet/alex_net_20000.model");
	//net->check();
	sgd_solver *sgd = new sgd_solver(net);

	sgd->set_lr(0.00001f);
	sgd->set_weight_decay(0.0005f);
	sgd->set_regularize(L2);

	string datapath = "/home/seal/4T/imagenet/224X224_train/";
	string trainlist = "/home/seal/4T/imagenet/train_list.txt";
	string valdatapath = "/home/seal/4T/imagenet/224X224_val/";
	string vallist = "/home/seal/4T/imagenet/val_list";
	string meanfile = "/home/seal/4T/imagenet/224X224_mean.binproto";

	vector<string> full_data;
	vector<vec_i> full_label;

	vector<string> full_data_test;
	vector<vec_i> full_label_test;

	/**
	 * load mean data
	 */
	blob *mean_ = cacu_allocator::create_blob(1,3,224,224,test);
#if __PARALLELTYPE__ == __CUDA__
	imageio_utils::load_mean_file_gpu(mean_->s_data(),meanfile);
#else
	imageio_utils::load_mean_file(mean_->s_data(),meanfile);
#endif
	/**
	 * read train list data into local memory
	 */
	ifstream is = ifstream(trainlist);
	is.precision(numeric_limits<float>::digits10);
	if(!is)
		LOG_FATAL("file %s cannot be opened!",trainlist.c_str());
	string file_ = "";
	while(getline(is,file_)){
		vector<string> vec = split(file_," ");
		full_data.push_back(datapath + vec[0]);
		vec_i label(1);
		label[0] = strtoul(vec[1].c_str(), NULL, 10);
		full_label.push_back(label);
	}
	is.close();
	/**
	 * read test list data into local memory
	 */
	is = ifstream(vallist);
	is.precision(numeric_limits<float>::digits10);
	if(!is)
		LOG_FATAL("file %s cannot be opened!",vallist.c_str());
	file_ = "";
	while(getline(is,file_)){
		vector<string> vec = split(file_," ");
		full_data_test.push_back(valdatapath + vec[0]);
		vec_i label(1);
		label[0] = strtoul(vec[1].c_str(), NULL, 10);
		full_label_test.push_back(label);
	}
	is.close();
	int ALL_DATA_SIZE = full_data.size();
	int ALL_TEST_DATA_SIZE = full_data_test.size();
	/**
	 * read data for training
	 */
	blob *input_data = (blob*)net->input_blobs()->at(0);
	bin_blob *input_label = (bin_blob*)net->input_blobs()->at(1);

	/**
	 * read data for testing
	 */
	blob *output_data = net->output_blob();

	int step_index = 0;
	int step_index_test = 0;
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	unsigned int max_index;
	float_t count = 0;

	vec_i compare_label(batch_size);
	for (int i = 1 ; i <= max_iter; ++i)
	{
		if(i % test_iter == 0)
		{
			count = 0;

			gettimeofday(&start,NULL);
			for (int j = 0 ; j < batch_size ; ++j)
			{
				if (step_index_test == ALL_TEST_DATA_SIZE)
					step_index_test = 0;
				//load image data
				readdata(full_data_test[step_index_test],input_data->p_data(j),mean_->s_data());
				input_label->copy_data_io(full_label_test[step_index_test],j);
				compare_label[j] = full_label_test[step_index_test][0];
				step_index_test += 1;
			}
			net->predict();

			for(int j = 0 ; j < batch_size ; ++j)
			{
				max_index = argmax(output_data->p_data(j),output_data->length());
				if(max_index == compare_label[j]){
					count += 1.0;
				}
			}

			gettimeofday(&end,NULL);
			LOG_INFO("test accuracy: %f", count / batch_size);
			precious_logger << count / batch_size << endl;
			precious_logger.flush();
		}

		else
		{
			gettimeofday(&start,NULL);
			for (int j = 0 ; j < batch_size ; ++j)
			{
				if (step_index == ALL_DATA_SIZE)
					step_index = 0;
				//load image data
				readdata(full_data[step_index],input_data->p_data(j),mean_->s_data());
				input_label->copy_data_io(full_label[step_index],j);
				step_index += 1;
			}
			//net->predict();
			sgd->train_iter();

			gettimeofday(&end,NULL);
		}

		if(i % 1 == 0){
			diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
			LOG_INFO("iter_%d, lr: %f, %ld ms/iter", i,sgd->lr(),diff/1000);
			((softmax_with_loss_op*)net->get_op(net->op_count()-1))->echo();
		}
		logger << ((softmax_with_loss_op*)net->get_op(net->op_count()-1))->loss() << endl;
		logger.flush();

		if(i == 50000)
			sgd->set_lr_iter(0.1f);
		if(i % 150000 == 0)
			sgd->set_lr_iter(0.1f);
		if(i % 10000 == 0){
			ostringstream oss;
			oss << "/home/seal/4T/cacue/imagenet/mobilenet_" << i << ".model";
			net->save_weights(oss.str());
		}
	}

	logger.close();
	precious_logger.close();

	ostringstream oss;
	oss << "/home/seal/4T/cacue/imagenet/mobilenet_" << max_iter << ".model";
	net->save_weights(oss.str());
	LOG_INFO("optimization is done!");
	for(int i = 0; i < full_label.size(); ++i)
	{
		vec_i().swap(full_label[i]);

	}
	for(int i = 0; i < full_label_test.size(); ++i)
	{
		vec_i().swap(full_label_test[i]);
	}
	vector<string>().swap(full_data);
	vector<string>().swap(full_data_test);
	delete mean_;
	delete net;

#if __PARALLELTYPE__ == __CUDA__
	cuda_release();
#endif
}
