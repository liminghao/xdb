// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <string>
#include <map>

#include "admin_server.h"

namespace xdb {

AdminServer::AdminServer(XdbServer *xdb_server, uint16_t port)
    :loop_thread_num_(1), xdb_server_(xdb_server), port_(port)
{

}

AdminServer::~AdminServer()
{
    delete server_;
    delete loop_;
}

void AdminServer::Init()
{
    muduo::net::InetAddress listen_addr(port_);
    loop_ = new muduo::net::EventLoop;
    server_ = new muduo::net::TcpServer(loop_, listen_addr, "AdminServer");

    server_->setConnectionCallback(boost::bind(
        &AdminServer::_OnConnection, this, _1));
    server_->setMessageCallback(boost::bind(
        &AdminServer::_OnMessage, this, _1, _2, _3));
    server_->setThreadNum(loop_thread_num_);
}

void AdminServer::Start()
{
    server_->start();
    loop_->loop();
}

void AdminServer::_OnConnection(
    const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "AdminServer::_OnConnection "
        << conn->peerAddress().toIpPort() << " -> "
        << conn->localAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN") << " name: "
        << conn->name();
}

void AdminServer::_OnMessage(
    const muduo::net::TcpConnectionPtr& conn,
    muduo::net::Buffer* buf, muduo::Timestamp time)
{
    int ret;
    LOG_INFO << "AdminServer::_OnMessage"
        << " readable bytes: " << buf->readableBytes();
}

} // namespace xdb

