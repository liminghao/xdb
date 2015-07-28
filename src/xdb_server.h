// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_SERVER_H_
#define XDB_SERVER_H_

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

#include "admin_server.h"
#include "data_server.h"
#include "context.h"

namespace xdb {

class DataServer;
class AdminServer;

class XdbServer : boost::noncopyable {
public:
    XdbServer();
    ~XdbServer();

    void Init();
    void Start();
    void Stop();

private:
    DataServer *data_server_;
    AdminServer *admin_server_;
};

} // namespace xdb

#endif

