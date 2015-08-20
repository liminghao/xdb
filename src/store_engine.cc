// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <assert.h>
#include "leveldb/db.h"
#include <muduo/base/Logging.h>

#include "store_engine.h"

namespace xdb {

StoreEngine::StoreEngine(std::string path)
    :path_(path)
{

}

StoreEngine::~StoreEngine()
{

}

void StoreEngine::Start()
{
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, path_.c_str(), &db_);
    assert(status.ok());
}

void StoreEngine::Stop()
{
    delete db_;
}

leveldb::Status StoreEngine::Set(std::string key, std::string value)
{
    leveldb::Status s;
    s = db_->Put(leveldb::WriteOptions(), key, value);
    if (!s.ok()) {
        LOG_WARN << "set failed " << key << " " << value;
        return s;
    }

    LOG_INFO << "set success " << key << " " << value;
    return s;
}

leveldb::Status StoreEngine::Get(std::string key, std::string &value)
{
    leveldb::Status s;
    s = db_->Get(leveldb::ReadOptions(), key, &value);
    if (!s.ok()) {
        LOG_WARN << "get failed " << key;
        return s;
    }

    LOG_INFO << "get success " << key << " " << value;
    return s;
    
}

leveldb::Status StoreEngine::Del(std::string key)
{
    leveldb::Status s;
    s = db_->Delete(leveldb::WriteOptions(), key);
    if (!s.ok()) {
        LOG_WARN << "del failed " << key;
        return s;
    }

    LOG_INFO << "del success " << key;
    return s;
}


} // namespace xdb

