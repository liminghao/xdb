// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "meta_manager.h"

namespace xdb {

MetaManager::MetaManager()
{

}

MetaManager::~MetaManager()
{

}

int MetaManager::Init()
{

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

}

Node* MetaManager::GetNode(std::string name)
{

}

Replica* MetaManager::GetReplica(std::string name)
{

}

Replica* MetaManager::GetPrimaryReplica(
    std::string table_name, int32_t partitionid)
{

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
        LOG_INFO << "Name: " << n->name()
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
        LOG_INFO << "Name: " << r->name()
            << " BelongsToTable:" << r->table()->name()
            << " ID:" << r->id()
            << " Status:" << r->status()
            << " BelongsToNode:" << r->node()->name();
    }
}

} // namespace xdb

