// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "binlog_manager.h"

namespace xdb {

BinlogManager::BinlogManager()
{

}

BinlogManager::~BinlogManager()
{

}

int BinlogManager::Init()
{

}

int BinlogManager::Start()
{

}

int BinlogManager::Stop()
{

}

BinLog* BinlogManager::Add(
    std::string replica_name,
    BinLog* binlog)
{
    std::map<std::string, BinLog*>::const_iterator it = binlogs_.find(replica_name);
    if (it != binlogs_.end()) {
            LOG_ERROR << "Binlog for " << replica_name << " already exist";
            return NULL;
    }
    
    binlogs_.insert(std::make_pair(replica_name, binlog));
    return binlog;
}

BinLog* BinlogManager::Get(std::string replica_name)
{
    std::map<std::string, BinLog*>::const_iterator it = binlogs_.find(replica_name);
    if (it != binlogs_.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

BinLog* BinlogManager::Del(std::string replica_name)
{

}

} // namespace xdb

