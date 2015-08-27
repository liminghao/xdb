// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <muduo/base/Logging.h>
#include "xdb_server.h"
#include "global.h"
#include "binlog.h"

//#define MAIN_SERVER_
#define BINLOG_TEST_

extern xdb::XdbServer *gXdbServer;

void Init();
void Start();
void TestMeta();

int main(int argc, char** argv)
{
    muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
    int ret;

#ifdef MAIN_SERVER_
    Init();
    Start();
    TestMeta();
#endif

#ifdef BINLOG_TEST_
    xdb::BinLog *binlog_handler = new xdb::BinLog("./testbinlog");
    ret = binlog_handler->Start();
    LOG_DEBUG << "binlog_handler->Start ret:" << ret;

    std::string key("testkey");
    std::string value("testvalue");
    std::string key1("testkey");
    std::string value1("testvalue");
    ret = binlog_handler->AppendRecord(xdb::kBinLogTypeKV, key, value);
    ret = binlog_handler->AppendRecord(xdb::kBinLogTypeKV, key1, value1);
    LOG_DEBUG << "binlog_handler->AppendRecord ret:" << ret;

    std::string k, v;
    ret = binlog_handler->GetRecord(k, v);
    ret = binlog_handler->GetRecord(k, v);

#endif

    sleep(1000000);
    exit(0);
}

void Init()
{
    xdb::gXdbServer = new xdb::XdbServer();
    xdb::gXdbServer->Init();
}

void Start()
{
    xdb::gXdbServer->Start();
}

void TestMeta()
{
    xdb::Node* n = xdb::AddNode("127.0.0.1", 39000);
    assert(n != NULL);

    xdb::Table* t = xdb::AddTable("test_table", 1, 10, 38882, "leveldb");
    assert(t != NULL);

    for (int i = 0; i < 10; ++i) {
        xdb::Replica *r = xdb::AddReplica("test_table", n->name(), i);
    }

    xdb::gXdbServer->meta_manager()->LogMeta();
}


