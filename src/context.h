// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_CONTEXT_H_
#define XDB_CONTEXT_H_

#include <boost/noncopyable.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <muduo/net/TcpServer.h>
#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include "store_engine.h"
#include "redis_protocol_analyzer.h"

namespace xdb {

class Replica;
class Table;
class XdbServer;

class Context : boost::noncopyable {
public:
    //Context(muduo::net::TcpConnection *conn, StoreEngine *store_engine);
    Context(muduo::net::TcpConnection *conn, XdbServer *xdb_server, Table *table);
    ~Context();

    int Parse(muduo::net::Buffer* buf);
    void PrintCmd();
    int ExecuteCmd();
    
private:
    RedisProtocolAnalyzer redis_protocol_analyzer_;
    muduo::net::TcpConnection *conn_;
   
    Table *table_;
    StoreEngine *store_engine_;
    XdbServer *xdb_server_;
};

} // namespace xdb

#endif

