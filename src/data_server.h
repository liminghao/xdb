// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_DATA_SERVER_H_
#define XDB_DATA_SERVER_H_

#include <muduo/net/TcpServer.h>
#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <utility>
#include <stdio.h>
#include <unistd.h>

namespace xdb {

class DataServer : boost::noncopyable {
public:
	DataServer();
	~DataServer();

    void Init();
    void Start();

private:
    void _OnConnection(const muduo::net::TcpConnectionPtr& conn);
    void _OnMessage(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf, muduo::Timestamp time);

    muduo::net::EventLoop* loop_;
    muduo::net::TcpServer* server_;
    int loop_thread_num_;
};

} // namespace xdb

#endif
