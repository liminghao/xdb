// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <assert.h>
#include "leveldb/db.h"
#include <muduo/base/Logging.h>

#include "store_engine.h"

namespace xdb {

StoreEngine::StoreEngine()
{

}

StoreEngine::~StoreEngine()
{

}

void StoreEngine::Start()
{
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db_);
    assert(status.ok());
}

void StoreEngine::Stop()
{
    delete db_;
}

int StoreEngine::Set(std::string key, std::string value)
{
    leveldb::Status s;
    s = db_->Put(leveldb::WriteOptions(), key, value);
    if (!s.ok()) {
        LOG_WARN << "set failed " << key << " " << value;
        return -1;
    }

    LOG_INFO << "set success " << key << " " << value;
    return 0;
}

int StoreEngine::Get(std::string key, std::string &value)
{
    leveldb::Status s;
    s = db_->Get(leveldb::ReadOptions(), key, &value);
    if (!s.ok()) {
        LOG_WARN << "get failed " << key << " " << value;
        return -1;
    }

    LOG_INFO << "get success " << key << " " << value;
    return 0;
    
}

int StoreEngine::Del(std::string key)
{
    leveldb::Status s;
    s = db_->Delete(leveldb::WriteOptions(), key);
    if (!s.ok()) {
        LOG_WARN << "del failed " << key;
        return -1;
    }

    LOG_INFO << "del success " << key;
    return 0;
}


} // namespace xdb
