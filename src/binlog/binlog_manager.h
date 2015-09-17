// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_BINLOG_MANAGER_H_
#define XDB_BINLOG_MANAGER_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <binlog.h>
#include <map>

namespace xdb {

class XdbServer;

class BinlogManager: boost::noncopyable {
public:
    BinlogManager();
    ~BinlogManager();

    int Init();
    int Start();
    int Stop();

    BinLog* Add(
        std::string replica_name,
        BinLog* binlog);

    BinLog* Get(std::string replica_name);
    BinLog* Del(std::string replica_name);

private:
    XdbServer *xdb_server_;

    // replica_name -> binlog
    std::map<std::string, BinLog*> binlogs_;
};

} // namespace xdb

#endif

