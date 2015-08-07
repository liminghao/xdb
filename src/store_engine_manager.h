// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_STORE_ENGINE_MANAGER_H_
#define XDB_STORE_ENGINE_MANAGER_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "store_engine.h"

namespace xdb {

class XdbServer;

class StoreEngineManager: boost::noncopyable {
public:
    StoreEngineManager();
    ~StoreEngineManager();

    int Init();
    int Start();
    int Stop();
    
    StoreEngine* AddStoreEngine(
        std::string replica_name, 
        StoreEngine* store_engine);

    StoreEngine* GetStoreEngine(std::string replica_name);
    StoreEngine* DelStoreEngine(std::string replica_name);

private:
    XdbServer *xdb_server_;

    // replica name -> StoreEngine
    std::map<std::string, StoreEngine*> store_engines_;
};

} // namespace xdb

#endif

