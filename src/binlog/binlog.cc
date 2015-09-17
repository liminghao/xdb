// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <errno.h>
#include <sys/stat.h>

#include "binlog/pb_binlog.pb.h"
#include "binlog.h"

namespace xdb {

BinLog::BinLog(std::string path)
    :path_(path), read_pos_file_("read_pos"), 
    current_file_("0.binlog")
{
}

BinLog::~BinLog()
{
    LOG_DEBUG << "BinLog::~BinLog()";
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
    LOG_DEBUG << "Open read_fd_:" << read_fd_;
    if (read_fd_ == -1) {
        LOG_ERROR << "Open file " << current_file << " for read failed";
        return kBinLogRetFailed;
    }
    write_fd_ = open(current_file.c_str(), O_WRONLY|O_APPEND|O_CREAT, 0644);
    LOG_DEBUG << "Open write_fd_:" << write_fd_; 
    if (write_fd_ == -1) {
        LOG_ERROR << "Open file " << current_file << " for write failed";
        return kBinLogRetFailed;
    }

    read_pos_fd_ = open(read_pos_file.c_str(), O_RDWR|O_CREAT, 0644);
    LOG_DEBUG << "Open read_pos_fd_:" << read_pos_fd_; 
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
            char tmpbuf[128];
            strerror_r(errno, tmpbuf, sizeof(tmpbuf));
            LOG_ERROR << "Write error, reason:" << tmpbuf << "errno:" << errno;
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

int BinLog::_PrepareRecordKV(std::string &key, std::string &value, std::string &s)
{
    binlog_proto::BinLogItemKV binlog_item_kv;
    std::string s_body;
    int size_int, type_int;
    s.clear();

    binlog_item_kv.set_key(key);
    binlog_item_kv.set_value(value);
    type_int = static_cast<int> (kBinLogTypeKV);
    size_int = binlog_item_kv.ByteSize();
    s.append(std::string((char*)&type_int, kblock_len));
    s.append(std::string((char*)&size_int, kblock_len));
    binlog_item_kv.SerializeToString(&s_body);
    s.append(s_body);
    
    return kBinLogRetSuccess;
}

int BinLog::_DoAppendRecord(std::string &record)
{
    int r;
    r = write(write_fd_, record.c_str(), record.size());
    LOG_DEBUG << "Write binlog_item_kv_str return " << r;
    if (r <= 0) {
        char tmpbuf[128];
        memset(tmpbuf, 0, sizeof(tmpbuf));
        strerror_r(errno, tmpbuf, sizeof(tmpbuf));
        LOG_ERROR << "Write binlog_item_kv_str error, reason:" << tmpbuf << "errno:" << errno;
        return -1;
    }
    return r;
}

// if not kv type, encode name and key to "key"
int BinLog::AppendRecord(BinLogType type, std::string &key, std::string &value)
{
    int r, type_int, size_int;
    std::string binlog_item_kv_str;

    switch (type) {
        case kBinLogTypeKV:
        {
            // maybe need endian change
            _PrepareRecordKV(key, value, binlog_item_kv_str);
            r = _DoAppendRecord(binlog_item_kv_str);
            if (r == -1) {
                LOG_ERROR << "_DoAppendRecord" << "failed";
                return kBinLogRetFailed;
            }
            break;
        }
        default:
        {
            LOG_ERROR << "Error BinLogType:" << type;
            break;
        }
    }

    return kBinLogRetSuccess;
}

int BinLog::_IncrReadPos(int i)
{
    char tmpbuf[16];
    int r;
    assert(i >= 0);
    read_pos_ += i;
    memset(tmpbuf, 0, sizeof(tmpbuf));
    snprintf(tmpbuf, sizeof(tmpbuf)-1, "%d", read_pos_);
    LOG_DEBUG << "write readpos:" << tmpbuf;
    lseek(read_pos_fd_, 0, SEEK_SET);
    r = write(read_pos_fd_, tmpbuf, strlen(tmpbuf));
    assert(r >= 0);
}

int BinLog::_ReadNBytesFromBinlog(char *buf, int n)
{
    assert(n > 0);    
    int ret;
    ret = read(read_fd_, buf, n);
    if (ret != n) {
        LOG_ERROR << "Read " << n << " bytes not enough, actually " << ret;
    }
    return ret;
}

// get one record from read pos
int BinLog::GetRecord(BinLogType &type, std::string &key, std::string &value)
{
    int size_int, type_int, ret;

    ret = _ReadNBytesFromBinlog((char*)&type_int, kblock_len);
    if (ret != kblock_len) {
        LOG_ERROR << "Read type_int error, ret:" << ret;
        return kBinLogRetFailed;
    }
    type = static_cast<BinLogType> (type_int);

    ret = _ReadNBytesFromBinlog((char*)&size_int, kblock_len);
    if (ret != kblock_len) {
        LOG_ERROR << "Read size_int error, ret:" << ret;
        return kBinLogRetFailed;
    }

    LOG_DEBUG << "Item type:" << type_int << ", Item size:" << size_int;
    char *tmpbuf = (char *)malloc(size_int);
    assert(tmpbuf != NULL);
    switch (type_int) {
        case kBinLogTypeKV:
        {
            ret = _ReadNBytesFromBinlog(tmpbuf, size_int);
            if (ret != size_int) {
                free(tmpbuf);
                LOG_ERROR << "Read size_int error, ret:" << ret;
                return kBinLogRetFailed;
            }
            binlog_proto::BinLogItemKV binlog_item_kv;
            binlog_item_kv.ParseFromString(std::string(tmpbuf));
            key = binlog_item_kv.key();
            value = binlog_item_kv.value();
            LOG_DEBUG << "key:" << key << " value:" << value;

            _IncrReadPos(kblock_len*2 + size_int);
            break;
        }
        default:
        {
            free(tmpbuf);
            LOG_ERROR << "Unknown binlog type";
            return kBinLogRetFailed;
        }
    }

    free(tmpbuf);
    return kBinLogRetSuccess;
}

int BinLog::GetRecordKV(std::string &input_key, std::string &input_value,
    std::string &output_key, std::string &output_value)
{
    output_key = input_key;
    output_value = input_value;
}

int BinLog::GetRecordHash(std::string &input_key, std::string &input_value,
    std::string &output_name, std::string &output_key, std::string &output_value)
{

}

} // namespace xdb

