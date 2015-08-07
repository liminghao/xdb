// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "meta_manager.h"
#include "store_engine.h"
#include "xdb_server.h"

namespace xdb {

MetaManager::MetaManager(XdbServer *xdb_server)
    :xdb_server_(xdb_server)
{

}

MetaManager::~MetaManager()
{

}

int MetaManager::Init()
{

}

void MetaManager::TestMeta()
{
    // for test
    Node *n = new Node("127.0.0.1", 39000);
    AddNode(n);

    Table *t = new Table("test_table", 1, 10, 38882, "leveldb");
    AddTable(t);

    for (int i = 0; i < 9; ++i) {
        std::string replica_name;
        _ReplicaName(n->name(), t->name(), i, replica_name);
        Replica *r = new Replica(i, replica_name);
        r->set_node(n);
        r->set_table(t);
        r->set_status(kPrimary);
        AddReplica(r);

        // create store engine
        char path[256];
        memset(path, 0, sizeof(path));
        sprintf(path, "%s/store_engine/%d",
                        xdb_server_->conf()->data_dir()->c_str(), r->id());
        StoreEngine *s = new StoreEngine(path);
        s->Start();
        xdb_server_->store_engine_manager()->AddStoreEngine(r->name(), s);
    }
}

Node* MetaManager::AddNode(Node *n)
{
    std::map<std::string, Node*>::const_iterator it = nodes_.find(n->name());
    if (it != nodes_.end()) {
        LOG_ERROR << "Node " << it->second->name() << " already exist";
        return NULL;
    }
    
    nodes_.insert(std::make_pair(n->name(), n));
    return n;
}

Table* MetaManager::AddTable(Table *t)
{
    std::map<std::string, Table*>::const_iterator it = tables_.find(t->name());
    if (it != tables_.end()) {
        LOG_ERROR << "Table " << it->second->name() << " already exist";
        return NULL;
    }
    
    tables_.insert(std::make_pair(t->name(), t));
    return t;
}

Replica* MetaManager::AddReplica(Replica *r)
{
    std::map<std::string, Replica*>::const_iterator it = replicas_.find(r->name());
    if (it != replicas_.end()) {
        LOG_ERROR << "Replica " << it->second->name() << " already exist";
        return NULL;
    }
    
    // update the node data, table data,
    // since the partition belongs to one node, one table
    r->node()->replicas()->insert(std::make_pair(r->name(), r));
    r->table()->replicas()->insert(std::make_pair(r->name(), r));
    replicas_.insert(std::make_pair(r->name(), r));
    return r;
}

Table* MetaManager::GetTable(std::string name)
{
    std::map<std::string, Table*>::const_iterator it = tables_.find(name);
    if (it == tables_.end()) {
        return NULL;
    }
    return it->second;
}

Node* MetaManager::GetNode(std::string name)
{

}

Replica* MetaManager::GetReplica(std::string name)
{

}

Replica* MetaManager::GetPrimaryReplica(
    std::string table_name, int32_t replica_id)
{
    // ugly
    std::string replica_name;

    std::map<std::string, Node*>::const_iterator nit;
    std::map<std::string, Replica*>::const_iterator rit;
    for (nit = nodes_.begin(); nit != nodes_.end(); nit++) {
        std::string node_name = nit->second->name();
        _ReplicaName(node_name, table_name, replica_id, replica_name);

LOG_INFO << "GetPrimaryReplica: " << replica_name;

        rit = replicas_.find(replica_name);
        if (rit != replicas_.end()) {
            return rit->second;
        }
    }
    return NULL;
}

void MetaManager::LogMeta()
{
    LogTable();
    LogNode();
    LogReplica();
}

void MetaManager::LogTable()
{
    LOG_INFO << "There are " << tables_.size() << " tables.";
    std::map<std::string, Table*>::iterator it;
    for (it = tables_.begin(); it != tables_.end(); ++it) {
        Table *t = it->second;
        LOG_INFO << "TableName:" << t->name() 
            << " ReplicateNum:" << t->replicate_num()
            << " ReplicaNum:" << t->partition_num()
            << " EngineType:" << t->engine_type()
            << " ListenPort:" << t->port();
    }
}

void MetaManager::LogNode()
{
    LOG_INFO << "There are " << nodes_.size() << " nodes.";
    std::map<std::string, Node*>::iterator it;
    for (it = nodes_.begin(); it != nodes_.end(); ++it) {
        Node *n = it->second;
        LOG_INFO << "NodeName:" << n->name()
            << " Host:" << n->ip()
            << " CmdPort:" << n->port();
    }
}

void MetaManager::LogReplica()
{
    LOG_INFO << "There are " << replicas_.size() << " replicas.";
    std::map<std::string, Replica*>::iterator it;
    for (it = replicas_.begin(); it != replicas_.end(); ++it) {
        Replica *r = it->second;
        LOG_INFO << "ReplicaName:" << r->name()
            << " BelongsToTable:" << r->table()->name()
            << " ID:" << r->id()
            << " Status:" << r->status()
            << " BelongsToNode:" << r->node()->name();
    }
}

void MetaManager::_ReplicaName(std::string node_name, std::string table_name,
    int32_t replica_id, std::string &replica_name)
{
    char buf[32];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf)-1, "%d", replica_id);

    // node_name#table_name#replica_id
    replica_name.clear();
    replica_name.append(node_name);
    replica_name.append("#");
    replica_name.append(table_name);
    replica_name.append("#");
    replica_name.append(buf);
    
}

} // namespace xdb

