// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <string.h>

#include "util.h"
#include "context.h"
#include "metadata.h"
#include "xdb_server.h"

namespace xdb {

Context::Context(muduo::net::TcpConnection *conn, 
    XdbServer *xdb_server, Table *table)
    :xdb_server_(xdb_server), table_(table)
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
    return ret;
}

void Context::PrintCmd()
{
    redis_protocol_analyzer_.PrintCmd();
}

int Context::ExecuteCmd()
{
    int ret;
    leveldb::Status s;
    std::vector<Bytes> *parsed_bytes = redis_protocol_analyzer_.recv_bytes();
    std::string *parsed_cmd = redis_protocol_analyzer_.cmd();

    if (strcmp(parsed_cmd->c_str(), "set") == 0) {
        std::string key((*parsed_bytes)[1].data(), (*parsed_bytes)[1].size());
        std::string value((*parsed_bytes)[2].data(), (*parsed_bytes)[2].size());
        
        // get engine by key
        int replica_id = simple_hash(key.c_str(), table_->partition_num());
        Replica *r = xdb_server_->meta_manager()
            ->GetPrimaryReplica(table_->name(), replica_id);
        if (r == NULL) {
            LOG_WARN << "No primary replica, replica_id:" << replica_id 
                << "partition_num:" << table_->partition_num() << "table_name:" << table_->name();
            conn_->forceClose();
            return -1;
        }
        store_engine_ = xdb_server_->store_engine_manager()
            ->GetStoreEngine(r->name());
        assert(store_engine_ != NULL);

        s = store_engine_->Set(key, value);
        if (s.ok()) {
            conn_->send("+OK\r\n", strlen("+OK\r\n"));
        } else {
            conn_->forceClose();
            return -1;
        }

    } else if (strcmp(parsed_cmd->c_str(), "get") == 0) {
        std::string key((*parsed_bytes)[1].data(), (*parsed_bytes)[1].size());
        std::string value;

        // get engine by key
        int replica_id = simple_hash(key.c_str(), table_->partition_num());
        Replica *r = xdb_server_->meta_manager()
            ->GetPrimaryReplica(table_->name(), replica_id);
        if (r == NULL) {
            LOG_WARN << "No primary replica, replica_id:" << replica_id 
                << "partition_num:" << table_->partition_num() << "table_name:" << table_->name();
            conn_->forceClose();
            return -1;
        }
        store_engine_ = xdb_server_->store_engine_manager()
            ->GetStoreEngine(r->name());
        assert(store_engine_ != NULL);

        s = store_engine_->Get(key, value);
        LOG_DEBUG << "get value: " << value;
        if (s.ok()) {
            char resp[1024];
            memset(resp, 0, sizeof(resp));
            sprintf(resp, "$%d\r\n%s\r\n", value.length(), value.c_str());
            conn_->send(resp, strlen(resp));
        } else if (s.IsNotFound()) {
            conn_->send("(nil)\r\n", strlen("(nil)\r\n"));
        } else {
            conn_->forceClose();
            return -1;
        }

    } else if (strcmp(parsed_cmd->c_str(), "del") == 0) {
        std::string key((*parsed_bytes)[1].data(), (*parsed_bytes)[1].size());

        // get engine by key
        int replica_id = simple_hash(key.c_str(), table_->partition_num());
        Replica *r = xdb_server_->meta_manager()
            ->GetPrimaryReplica(table_->name(), replica_id);
        if (r == NULL) {
            LOG_WARN << "No primary replica, replica_id:" << replica_id 
                << "partition_num:" << table_->partition_num() << "table_name:" << table_->name();
            conn_->forceClose();
            return -1;
        }
        store_engine_ = xdb_server_->store_engine_manager()
            ->GetStoreEngine(r->name());
        assert(store_engine_ != NULL);

        s = store_engine_->Del(key);
        if (s.ok()) {
            conn_->send("(integer) 1\r\n", strlen("(integer) 1\r\n"));
        } else {
            conn_->send("(integer) 0\r\n", strlen("(integer) 0\r\n"));
            return -1;
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

