// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_MAETA_MANAGER_H_
#define XDB_MAETA_MANAGER_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "metadata.h"

namespace xdb {

class MetaManager: boost::noncopyable {
public:
    MetaManager();
    ~MetaManager();

    int Init();
    Node* AddNode(Node *n);
    Table* AddTable(Table *t);
    Replica* AddReplica(Replica *p);
    Table* GetTable(std::string name);
    Node* GetNode(std::string name);
    Replica* GetReplica(std::string name);

private:

    std::map<std::string, Node*> nodes_;
    std::map<std::string, Replica*> replicas_;
    std::map<std::string, Table*> tables_;
};

} // namespace xdb

#endif

