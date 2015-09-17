// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_SERVER_H_
#define XDB_SERVER_H_

#include <boost/noncopyable.hpp>
#include <unistd.h>
#include <string>
#include <map>

#include "admin_server.h"
#include "data_server.h"
#include "context.h"
#include "configure.h"
#include "meta_manager.h"
#include "store_engine_manager.h"
#include "binlog_manager.h"

namespace xdb {

class DataServer;
class AdminServer;

class XdbServer : boost::noncopyable {
public:
    XdbServer();
    ~XdbServer();

    void TestInit();
    void Init();
    void Start();
    void Stop();

    Configure *conf() { return conf_; }    
    MetaManager *meta_manager() { return meta_manager_; }
    StoreEngineManager *store_engine_manager()
    { return store_engine_manager_; }
    DataServer *AddDataServer(std::string tablename, DataServer* d);
    BinlogManager *binlog_manager()
    { return binlog_manager_; }

private:

    int _EnsureDir();
    int _InitDataServer();
    int _StartDataServer();

    AdminServer *admin_server_;

    // tablename -> DataServer
    std::map<std::string, DataServer*> data_servers_; 
   
    Configure *conf_;
    MetaManager *meta_manager_;
    StoreEngineManager *store_engine_manager_;
    BinlogManager *binlog_manager_;
};

} // namespace xdb

#endif

