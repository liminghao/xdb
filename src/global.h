// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_GLOBAL_H_
#define XDB_GLOBAL_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>

namespace xdb {

class XdbServer;

extern XdbServer *gXdbServer;

} // namespace xdb

#endif

