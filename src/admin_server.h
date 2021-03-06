// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_ADMIN_SERVER_H_
#define XDB_ADMIN_SERVER_H_

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <utility>
#include <stdio.h>
#include <unistd.h>

#include <muduo/net/TcpServer.h>
#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

namespace xdb {

class XdbServer;

class AdminServer : boost::noncopyable {
public:
	AdminServer(XdbServer *xdb_server, uint16_t port);
	~AdminServer();

    void Init();
    void Start();

private:
    void _OnConnection(const muduo::net::TcpConnectionPtr& conn);
    void _OnMessage(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf, muduo::Timestamp time);

    void _ThreadStartFunc();
    muduo::Thread main_thread_;

    muduo::net::EventLoop* loop_;
    muduo::net::TcpServer* server_;
    int loop_thread_num_;
    uint16_t port_;

    XdbServer *xdb_server_;
};

} // namespace xdb

#endif
