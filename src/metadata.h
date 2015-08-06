// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_METADATA_H_
#define XDB_METADATA_H_

#include <boost/noncopyable.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "store_engine.h"

namespace xdb {

class Replica;
class Table;
class Node;

class Table : boost::noncopyable {
public:
    Table(std::string name, int32_t replicate_num, int32_t partition_num, 
        uint16_t port, std::string engine_type="leveldb")
        :name_(name), replicate_num_(replicate_num), port_(port),
        partition_num_(partition_num), engine_type_(engine_type)
    {}

    std::string name() { return name_; }
    int32_t replicate_num() { return replicate_num_; }
    int32_t partition_num() { return partition_num_; }
    std::string engine_type() { return engine_type_; }
    uint16_t port() { return port_; }

    std::map<std::string, Replica*> *replicas()
    { return &replicas_; }

    ~Table()
    {}

private:
    std::string name_;
    int32_t replicate_num_;  // 3
    int32_t partition_num_;  // 1024
    std::string engine_type_;
    uint16_t port_;  //data port
    
    // replicas in the table
    std::map<std::string, Replica*> replicas_;
};

enum ReplicaStatus {
    kElecting = 0,
    kPrimary,
    kSecondary,
    kRebalance,
    kRecover,
    kError,
};

class Replica : boost::noncopyable {
public:
    Replica(int32_t id, std::string name)
        :id_(id), name_(name), status_(kElecting) 
    {}

    int32_t id() { return id_; }
    std::string name() { return name_; }
    Node *node() { return node_; }
    Table *table() { return table_; }
    int32_t status() { return status_; }
    void set_node(Node *n) { node_ = n; }
    void set_table(Table *t) { table_ = t; }
    void set_status(int32_t s) { status_ = s; }

private:
    int32_t id_;
    std::string name_;  // nodename#tablename#partitionid
    
    Node *node_;        // the replica belongs to the node
    Table *table_;      // the replica belongs to the table
    int32_t status_;    // kNormal, kRebalance, kRecover
};

class Node : boost::noncopyable {
public:
    Node(std::string ip, uint16_t port)
        :ip_(ip), port_(port)
    {
        char buf[128];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s:%d", ip_.c_str(), port_);
        name_ = buf;
    }

    ~Node()
    {}

    std::string ip() { return ip_; }
    uint16_t port() { return port_; }
    std::string name() { return name_; }
    std::map<std::string, Replica*> *replicas()
    { return &replicas_; }

private:
    std::string ip_;
    uint16_t port_; //command port
    std::string name_;

    // replicas in the node
    std::map<std::string, Replica*> replicas_;
};

} // namespace xdb

#endif

