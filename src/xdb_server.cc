// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "xdb_server.h"

namespace xdb {

XdbServer::XdbServer()
{
    admin_server_ = new AdminServer(this, 39000);
}

XdbServer::~XdbServer()
{
    delete admin_server_;
    
    std::map<std::string, DataServer*>::iterator it;
    for (it = data_servers_.begin(); it != data_servers_.end(); ++it) {
        DataServer *d = it->second;
        delete d;
    }
}

void XdbServer::Init()
{
    LOG_INFO << "Configure initialize ...";
    conf_ = new Configure();
    conf_->InitTest();
    
    LOG_INFO << "MetaManager initialize ...";
    meta_manager_ = new MetaManager();
    meta_manager_->Init();
    meta_manager_->LogMeta();
    
    LOG_INFO << "StoreEngineManager initialize ...";
    store_engine_manager_ = new StoreEngineManager();
    store_engine_manager_->Init();

    LOG_INFO << "AdminServer initialize ...";
    admin_server_ = new AdminServer(this, conf_->adminport());
    admin_server_->Init();
    
    LOG_INFO << "DataServer initialize ...";
    _InitDataServer();
}

void XdbServer::Start()
{
    LOG_INFO << "StoreEngineManager start ...";
    store_engine_manager_->Start();

    LOG_INFO << "AdminServer start ...";
    admin_server_->Start();

    LOG_INFO << "DataServers start ...";
    _StartDataServer();
}

void XdbServer::Stop()
{

}

// init dataserver based on metadata
int XdbServer::_InitDataServer()
{

}

int XdbServer::_StartDataServer()
{

}

} // namespace xdb

