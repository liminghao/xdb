// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_UTIL_H_
#define XDB_UTIL_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace xdb {

int simple_hash(const char* str, int M);


} // namespace xdb

#endif

