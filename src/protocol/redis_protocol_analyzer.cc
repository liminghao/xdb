// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <muduo/base/Logging.h>

#include "redis_protocol_analyzer.h"

namespace xdb {

RedisProtocolAnalyzer::RedisProtocolAnalyzer()
{

}

RedisProtocolAnalyzer::~RedisProtocolAnalyzer()
{

}

//int RedisProtocolAnalyzer::Parse(muduo::net::Buffer* buf)
//{
//
//}

void RedisProtocolAnalyzer::PrintCmd()
{
    LOG_INFO << "Cmd: " << "[" << cmd_ << "]";

    for (int i = 0; i < recv_bytes_.size(); ++i) {
        std::string s(recv_bytes_[i].data(), recv_bytes_[i].size());
        //printf("recv_bytes_[%d]: %s size:%d \n", i, s.c_str(), recv_bytes_[i].size());
        LOG_INFO << "recv_bytes_[" << i << "]: " << "[" << s << "]";
    }
    //printf("======== \n");
}

int RedisProtocolAnalyzer::Parse(muduo::net::Buffer* input)
{
	recv_bytes_.clear();

	int parsed = 0;
	
    //int size = input->size();
	//char *ptr = input->data();
    int size = input->readableBytes();
	char *ptr = (char*)(input->peek());
	
	//dump(ptr, size);
	
	if(ptr[0] != '*'){
		return -1;
	}

	int num_args = 0;	
	while(size > 0){
		char *lf = (char *)memchr(ptr, '\n', size);
		if(lf == NULL){
			break;
		}
		lf += 1;
		size -= (lf - ptr);
		parsed += (lf - ptr);
		
		int len = (int)strtol(ptr + 1, NULL, 10); // ptr + 1: skip '$' or '*'
		if(errno == EINVAL){
			return -1;
		}
		ptr = lf;
		if(len < 0){
			return -1;
		}
		if(num_args == 0){
			if(len <= 0){
				return -1;
			}
			num_args = len;
			continue;
		}
		
		if(len > size - 2){
			break;
		}
		
		recv_bytes_.push_back(Bytes(ptr, len));
		
		ptr += len + 2;
		size -= len + 2;
		parsed += len + 2;

		num_args --;
		if(num_args == 0){
			input->retrieve(parsed);

            cmd_ = recv_bytes_[0].String();
            strtolower(&cmd_);

			return 1;
		}
	}
	
	recv_bytes_.clear();
	return 0;
}

} // namespace xdb

