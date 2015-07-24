// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "data_server.h"

namespace xdb {

DataServer::DataServer()
    :loop_thread_num_(1)
{

}

DataServer::~DataServer()
{
    delete server_;
    delete loop_;
}

void DataServer::Init()
{
    muduo::net::InetAddress listen_addr(38000);
    loop_ = new muduo::net::EventLoop;
    server_ = new muduo::net::TcpServer(loop_, listen_addr, "DataServer");

    server_->setConnectionCallback(boost::bind(
        &DataServer::_OnConnection, this, _1));
    server_->setMessageCallback(boost::bind(
        &DataServer::_OnMessage, this, _1, _2, _3));
    server_->setThreadNum(loop_thread_num_);
}

void DataServer::Start()
{
    server_->start();
    loop_->loop();
}

void DataServer::_OnConnection(
    const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "DataServer _OnConnection "
        << conn->peerAddress().toIpPort() << " -> "
        << conn->localAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
}

void DataServer::_OnMessage(
    const muduo::net::TcpConnectionPtr& conn,
    muduo::net::Buffer* buf, muduo::Timestamp time)
{
    LOG_INFO << "DataServer::_OnMessage"
        << " readable bytes: " << buf->readableBytes();
}

} // namespace xdb

