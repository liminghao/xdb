// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_STORE_ENGINE_H_
#define XDB_STORE_ENGINE_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <leveldb/db.h>

namespace xdb {

class StoreEngine: boost::noncopyable {
public:
    StoreEngine(std::string path);
    ~StoreEngine();

    void Start();
    void Stop();
    leveldb::Status Set(std::string key, std::string value);
    leveldb::Status Get(std::string key, std::string &value);
    leveldb::Status Del(std::string key);

private:
    leveldb::DB* db_;
    std::string path_;
};

} // namespace xdb

#endif

