// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "context.h"

namespace xdb {

Context::Context()
{
}
    
Context::~Context()
{
    LOG_INFO << "context destroy ...";
}

} // namespace xdb

