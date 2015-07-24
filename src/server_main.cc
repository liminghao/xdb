// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <muduo/net/TcpServer.h>
#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>
#include <utility>
#include <stdio.h>
#include <unistd.h>

#include "xdb_server.h"

int main(int argc, char** argv)
{
    xdb::XdbServer server;
    server.Init();
    server.Start();

    sleep(1000000);
    exit(0);
}


