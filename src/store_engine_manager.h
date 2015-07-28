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

class StoreEngineManager: boost::noncopyable {
public:
    StoreEngineManager();
    ~StoreEngineManager();

private:

};

} // namespace xdb

#endif

