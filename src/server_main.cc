// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <muduo/base/Logging.h>
#include "xdb_server.h"
#include "global.h"

extern xdb::XdbServer *gXdbServer;

void Init();
void Start();
void TestMeta();

int main(int argc, char** argv)
{
    muduo::Logger::setLogLevel(muduo::Logger::DEBUG);

    Init();
    Start();
    TestMeta();

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


