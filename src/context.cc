// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "context.h"
#include <string.h>

namespace xdb {

class XdbServer;

Context::Context(muduo::net::TcpConnection *conn, XdbServer *xdb_server)
    :xdb_server_(xdb_server)
{
    conn_ = conn;
}

//Context::Context(muduo::net::TcpConnection *conn, StoreEngine *store_engine)
//{
//    conn_ = conn;
//    store_engine_ = store_engine;
//}
    
Context::~Context()
{
    LOG_INFO << "context destroy ...";
}

int Context::Parse(muduo::net::Buffer* buf)
{
    int ret = redis_protocol_analyzer_.Parse(buf);
    
    // get engine by key
    store_engine_ = new StoreEngine();
    return ret;
}

void Context::PrintCmd()
{
    redis_protocol_analyzer_.PrintCmd();
}

int Context::ExecuteCmd()
{
    int ret;
    std::vector<Bytes> *parsed_bytes = redis_protocol_analyzer_.recv_bytes();
    std::string *parsed_cmd = redis_protocol_analyzer_.cmd();

    if (strcmp(parsed_cmd->c_str(), "set") == 0) {
        std::string key((*parsed_bytes)[1].data(), (*parsed_bytes)[1].size());
        std::string value((*parsed_bytes)[2].data(), (*parsed_bytes)[2].size());
        ret = store_engine_->Set(key, value);

        if (ret == 0) {
            conn_->send("+OK\r\n", strlen("+OK\r\n"));
        } else {
            conn_->forceClose();
        }

    } else if (strcmp(parsed_cmd->c_str(), "get") == 0) {
        std::string key((*parsed_bytes)[1].data(), (*parsed_bytes)[1].size());
        std::string value;
        ret = store_engine_->Get(key, value);
        LOG_INFO << "get value: " << value;
        
        if (ret == 0) {
            char resp[1024];
            memset(resp, 0, sizeof(resp));
            sprintf(resp, "$%d\r\n%s\r\n", value.length(), value.c_str());
            conn_->send(resp, strlen(resp));
        } else {
            conn_->forceClose();
        }

    } else if (strcmp(parsed_cmd->c_str(), "del") == 0) {
        std::string key((*parsed_bytes)[1].data(), (*parsed_bytes)[1].size());
        ret = store_engine_->Del(key);
        
        if (ret == 0) {
            conn_->send("+OK\r\n", strlen("+OK\r\n"));
        } else {
            conn_->forceClose();
        }
    
    } else if (strcmp(parsed_cmd->c_str(), "quit") == 0) {
        conn_->forceClose();
    } else {
        LOG_WARN << "unknown command: " << (*parsed_cmd);
        ret = -1;
    }

    return ret;
}

} // namespace xdb

