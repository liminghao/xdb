// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef XDB_BINLOG_H_
#define XDB_BINLOG_H_

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fcntl.h>

#include <muduo/base/Logging.h>

namespace xdb {

////////////////////////////////////////////////
//
//  record format:
//  | type   | size   | data            |
//  | 4bytes | 4bytes | variable length |
//
//  size means "data" length
//
////////////////////////////////////////////////

const int kblock_len = sizeof(int);

enum BinLogRet {
    kBinLogRetSuccess = 0,
    kBinLogRetFailed,
};

enum BinLogType {
    kBinLogTypeKV = 0,
    kBinLogTypeHash,
};

class BinLog: boost::noncopyable {
public:
    BinLog(std::string path);
    ~BinLog();
    
    int Start();

    // if not kv type, encode name and key to "key"
    int AppendRecord(BinLogType type, std::string &key, std::string &value);

    // get one record from read pos
    int GetRecord(BinLogType &type, std::string &key, std::string &value);
    int GetRecordKV(std::string &input_key, std::string &input_value,
        std::string &output_key, std::string &output_value);
    int GetRecordHash(std::string &input_key, std::string &input_value,
        std::string &output_name, std::string &output_key, std::string &output_value);

private:
    int _PrepareRecordKV(std::string &key, std::string &value, std::string &s);
    int _DoAppendRecord(std::string &record);
    int _IncrReadPos(int i);
    int _ReadNBytesFromBinlog(char *buf, int n);

    std::string path_;   
    std::string current_file_;  // simple, always one file
    std::string read_pos_file_;
    int read_pos_;
    
    // no need write_pos_, always write append
    // int record_num_;
    
    int write_fd_;
    int read_fd_;
    int read_pos_fd_;
};

} // namespace xdb

#endif

