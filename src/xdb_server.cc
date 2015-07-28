// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "xdb_server.h"

namespace xdb {

XdbServer::XdbServer()
{
    admin_server_ = new AdminServer(this);
    data_server_ = new DataServer(this);
}

XdbServer::~XdbServer()
{
    delete data_server_;
    delete admin_server_;
}

void XdbServer::Init()
{
    LOG_INFO << "AdminServer initialize ...";
    admin_server_->Init();
    
    LOG_INFO << "DataServer initialize ...";
    data_server_->Init();
}

void XdbServer::Start()
{
    admin_server_->Start();
    data_server_->Start();
}

void XdbServer::Stop()
{

}

} // namespace xdb

