// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <sys/stat.h>
#include <sys/types.h>
#include "xdb_server.h"

namespace xdb {

XdbServer::XdbServer()
{

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

int XdbServer::_EnsureDir()
{
    // data_dir
    std::string s;
    std::string data_dir = *(conf_->data_dir());
    int ret = access(data_dir.c_str(), F_OK);
    if (ret != 0) {
        LOG_INFO << "DataDir:" << data_dir << " not exist, create it";
        mkdir(data_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        s = data_dir;
        s.append("/store_engine");
        mkdir(s.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        s = data_dir;
        s.append("/binlog");
        mkdir(s.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        LOG_INFO << "DataDir:" << data_dir << " exist";
    }
}

void XdbServer::Init()
{
    conf_ = new Configure();
    meta_manager_ = new MetaManager(this);
    store_engine_manager_ = new StoreEngineManager();

    LOG_INFO << "Configure initialize ...";
    conf_->Init();

    LOG_INFO << "Ensure dir ...";
    _EnsureDir();

    LOG_INFO << "MetaManager initialize ...";
    meta_manager_->Init();
    meta_manager_->LogMeta();
    
    LOG_INFO << "StoreEngineManager initialize ...";
    store_engine_manager_->Init();

    // add engine, load engine

    LOG_INFO << "AdminServer initialize ...";
    admin_server_ = new AdminServer(this, conf_->adminport());
    admin_server_->Init();
    
    LOG_INFO << "DataServer initialize ...";
    _InitDataServer();
}

void XdbServer::TestInit()
{
    LOG_DEBUG << "For test, init something ...";
    
    Node *n = new Node("127.0.0.1", 39000);
    meta_manager_->AddNode(n);

    Table *t = new Table("test_table", 1, 10, 38882, "leveldb");
    meta_manager_->AddTable(t);

//    for (int i = 0; i < 9; ++i) {
//        std::string replica_name;
//        _ReplicaName(n->name(), t->name(), i, replica_name);
//        Replica *r = new Replica(i, replica_name);
//        r->set_node(n);
//        r->set_table(t);
//        r->set_status(kPrimary);
//        AddReplica(r);
//        AddPrimaryReplica(r);
//
//        char path[256];
//        memset(path, 0, sizeof(path));
//        sprintf(path, "%s/store_engine/%d",
//                        xdb_server_->conf()->data_dir()->c_str(), r->id());
//        StoreEngine *s = new StoreEngine(path);
//        s->Start();
//        xdb_server_->store_engine_manager()->AddStoreEngine(r->name(), s);
//    }
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

// init dataserver by metadata
int XdbServer::_InitDataServer()
{
    std::map<std::string, Table*>::iterator it;
    for (it = meta_manager_->tables()->begin(); 
        it != meta_manager_->tables()->end(); ++it) {
        Table *t = it->second;
        DataServer *d = new DataServer(this, t->port(), t->name());
        AddDataServer(t->name(), d);
    }
}

int XdbServer::_StartDataServer()
{
    std::map<std::string, DataServer*>::const_iterator it;
    for (it = data_servers_.begin(); it != data_servers_.end(); ++it) {
        DataServer *d = it->second;
        LOG_INFO << "DataServer for table:" << *(d->tablename()) << " start ...";
        d->Start();
    }
}

DataServer *XdbServer::AddDataServer(std::string tablename, DataServer* d)
{
    std::map<std::string, DataServer*>::const_iterator it = data_servers_.find(tablename);
    if (it != data_servers_.end()) {
        LOG_ERROR << "DataServer " << *(it->second->tablename()) << " already exist";
        return NULL;
    }
    
    LOG_INFO << "Add DataServer for " << tablename;
    data_servers_.insert(std::make_pair(tablename, d));
    return d;
}

} // namespace xdb

