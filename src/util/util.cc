// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util.h"

namespace xdb {

int simple_hash(const char* str, int M)
{
    int i, l = (strlen(str)+1) / 2;
    unsigned int ret = 0;
    unsigned short *s = (unsigned short*)str;
    for (i = 0; i < l; i++)
    {
        ret ^= (s[i] << (i & 0x0F));
    }
    return ret % M;
}

} // namespace xdb

