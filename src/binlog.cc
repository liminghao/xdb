// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <sys/stat.h>
#include "binlog.h"

namespace xdb {

BinLog::BinLog(std::string path)
    :path_(path), read_pos_file_("read_pos"), 
    current_file_("0.binlog")
{
}

BinLog::~BinLog()
{
    close(write_fd_);
    close(read_fd_);
}

// why need Start() function? why not fopen some files in constructor?
// because if fopen failed, user may not known.
int BinLog::Start()
{
    std::string current_file, read_pos_file;
    int ret;

    current_file.append(path_);
    current_file.append("/");
    current_file.append(current_file_);
    
    read_pos_file.append(path_);
    read_pos_file.append("/");
    read_pos_file.append(read_pos_file_);
    
    LOG_DEBUG << "Start binlog, current_file:" << current_file;
    LOG_DEBUG << "Start binlog, read_pos_file:" << read_pos_file;

    //ensure dir
    ret = access(path_.c_str(), F_OK);
    if (ret != 0) {
        LOG_INFO << path_ << " not exist, mkdir ...";
        mkdir(path_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    read_fd_ = open(current_file.c_str(), O_RDONLY|O_CREAT, 0644);
    if (read_fd_ == -1) {
        LOG_ERROR << "Open file " << current_file << " for read failed";
        return kBinLogRetFailed;
    }
    write_fd_ = open(current_file.c_str(), O_APPEND|O_CREAT, 0644);
    if (write_fd_ == -1) {
        LOG_ERROR << "Open file " << current_file << " for write failed";
        return kBinLogRetFailed;
    }

    read_pos_fd_ = open(read_pos_file.c_str(), O_RDWR|O_CREAT, 0644);
    if (read_pos_fd_ == -1) {
        LOG_ERROR << "Open file " << read_pos_file << " for read failed";
        return kBinLogRetFailed;
    }

    char buf[128];
    memset(buf, 0, sizeof(buf));
    ret = read(read_pos_fd_, buf, sizeof(buf)-1);
    if (ret < 0) {
        LOG_ERROR << "Read file " << read_pos_file << " failed, ret:" << ret;
        return kBinLogRetFailed;
    } else if (ret == 0) {
        // end of file
        read_pos_ = 0;
        int r = write(read_pos_fd_, "0", strlen("0"));
        LOG_DEBUG << "Write return " << r;
        if (r <= 0) {
            LOG_ERROR << "Write error";
            return kBinLogRetFailed;
        }

    } else {
        sscanf(buf, "%d", &read_pos_);
    }
    LOG_DEBUG << "read_pos: string[" << buf << "] int[" << read_pos_ << "]";

    ret = lseek(read_fd_, read_pos_, SEEK_SET);
    if (ret == -1) {
        LOG_ERROR << "Fseek error, " << "file:" << current_file 
            << " read_pos:" << read_pos_;
        return kBinLogRetFailed;
    }
    LOG_DEBUG << "Seek to " << current_file << ":" << read_pos_;

    return kBinLogRetSuccess;
}

// if not kv type, encode name and key to "key"
int BinLog::AppendRecord(BinLogType type, std::string &key, std::string &value)
{
    int r, type_int, size_int;
    switch (type) {
        kBinLogTypeKV:
            size_int = 100;
            r = write(write_fd_, &size_int, sizeof(int));       
            LOG_DEBUG << "Write size_int return " << r;
            if (r <= 0) {
                LOG_ERROR << "Write size_int error";
                return kBinLogRetFailed;
            }

            type_int = static_cast<int> (type);
            r = write(write_fd_, &type_int, sizeof(int));       
            LOG_DEBUG << "Write type_int return " << r;
            if (r <= 0) {
                LOG_ERROR << "Write type_int error";
                return kBinLogRetFailed;
            }

            r = write(write_fd_, key.c_str(), key.size());       
            LOG_DEBUG << "Write key return " << r;
            if (r <= 0) {
                LOG_ERROR << "Write key error";
                return kBinLogRetFailed;
            }

            r = write(write_fd_, value.c_str(), value.size());       
            LOG_DEBUG << "Write value return " << r;
            if (r <= 0) {
                LOG_ERROR << "Write value error";
                return kBinLogRetFailed;
            }
    
            break;
        default:
            LOG_ERROR << "Error BinLogType:" << type;
            break;
    }

    return kBinLogRetSuccess;
}

// get one record from read pos
int BinLog::KVGetRecord(std::string &key, std::string &value)
{

}

int BinLog::HashGetRecord(std::string &name, std::string &key, std::string &value)
{

}

} // namespace xdb

