// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_CONFIGURE_H
#define XDB_CONFIGURE_H

#include <boost/noncopyable.hpp>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <map>

#include <muduo/base/Logging.h>
#include "tinyxml.h"

namespace xdb {

class Configure : boost::noncopyable {
public:
    Configure()
        :document_(NULL)
    {}

    void InitTest();
    void Init(std::string filename);
    uint16_t adminport() { return adminport_; }
    uint32_t worker_threadnum() { return worker_threadnum_; }
    uint32_t sleep_second() { return sleep_second_; }
    uint32_t wait_second() { return wait_second_; }
    std::string filename() { return filename_; }

    // new
    std::string *data_dir() { return &data_dir_; }

private:
    std::string filename_;      // config filename xml
    TiXmlDocument* document_;

    uint16_t adminport_;        // metaserver admin listening port
    uint32_t worker_threadnum_; // thread number in the admin thread pool
    uint32_t sleep_second_;
    uint32_t wait_second_;

    // new
    std::string data_dir_;
};

} // namespace xdb

#endif 
