// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "store_engine_manager.h"
#include "xdb_server.h"

namespace xdb {

StoreEngineManager::StoreEngineManager()
{

}

StoreEngineManager::~StoreEngineManager()
{

}

int StoreEngineManager::Init()
{

}

int StoreEngineManager::Start()
{

}

int StoreEngineManager::Stop()
{

}

StoreEngine* StoreEngineManager::AddStoreEngine(
    std::string replica_name,
    StoreEngine* store_engine)
{
    std::map<std::string, StoreEngine*>::const_iterator it = store_engines_.find(replica_name);
    if (it != store_engines_.end()) {
        LOG_ERROR << "StoreEngine for " << replica_name << " already exist";
        return NULL;
    }
    
    store_engines_.insert(std::make_pair(replica_name, store_engine));
    return store_engine;
}

StoreEngine* StoreEngineManager::GetStoreEngine(
    std::string replica_name)
{
    std::map<std::string, StoreEngine*>::const_iterator it = store_engines_.find(replica_name);
    if (it != store_engines_.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

StoreEngine* StoreEngineManager::DelStoreEngine(
    std::string replica_name)
{

}

} // namespace xdb

