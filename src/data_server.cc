// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <string>
#include <map>

#include "data_server.h"
#include "context.h"
#include "xdb_server.h"

namespace xdb {

void delete_context(void *p)
{
    Context *c = static_cast<Context*> (p);
    delete c;
}

DataServer::DataServer(XdbServer *xdb_server, 
    uint16_t port, std::string tablename)
    :loop_thread_num_(1), xdb_server_(xdb_server), 
    port_(port), tablename_(tablename)
{

}

DataServer::~DataServer()
{
    delete server_;
    delete loop_;
}

void DataServer::Init()
{
    muduo::net::InetAddress listen_addr(port_);
    loop_ = new muduo::net::EventLoop;
    server_ = new muduo::net::TcpServer(loop_, listen_addr, "DataServer");

    server_->setConnectionCallback(boost::bind(
        &DataServer::_OnConnection, this, _1));
    server_->setMessageCallback(boost::bind(
        &DataServer::_OnMessage, this, _1, _2, _3));
    server_->setThreadNum(loop_thread_num_);

    store_engine_manager_ = xdb_server_->store_engine_manager();
}

void DataServer::Start()
{
    server_->start();
    loop_->loop();
}

void DataServer::_OnConnection(
    const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "DataServer::_OnConnection "
        << conn->peerAddress().toIpPort() << " -> "
        << conn->localAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN") << " name: "
        << conn->name();

    // set context
    Context *c = new Context(conn.get(), xdb_server_);
    conn->set_private_data(c);
    conn->set_private_data_destroy(delete_context);
}

void DataServer::_OnMessage(
    const muduo::net::TcpConnectionPtr& conn,
    muduo::net::Buffer* buf, muduo::Timestamp time)
{
    int ret;

    LOG_INFO << "DataServer::_OnMessage"
        << " readable bytes: " << buf->readableBytes();

    Context *c = static_cast<Context*> (conn->private_data());
    ret = c->Parse(buf);
    if (ret != -1) {
        c->PrintCmd();
        c->ExecuteCmd();
        // need not close
    }
}

} // namespace xdb

