// Copyright (c) 2015 The Xdb Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "configure.h"

namespace xdb {

void Configure::InitTest()
{

}

// read configure file(xml), get the parameter
void Configure::Init(std::string filename)
{
    bool ret;
    filename_ = filename;
    document_ = new TiXmlDocument();
    assert(document_ != NULL);

    ret = document_->LoadFile(filename_.c_str());
    if (!ret) {
        LOG_FATAL << "Load " << filename << " error";
    }

    // Default
    sleep_second_ = 300;
    wait_second_ = 10;

    TiXmlElement* root_element = document_->RootElement();
    TiXmlElement* server_element = root_element->FirstChildElement();
    TiXmlAttribute* attribuite_of_server = server_element->FirstAttribute();
    while (attribuite_of_server) {
        if (strcmp(attribuite_of_server->Name(), "admin_port") == 0) {
            uint32_t uport;
            sscanf(attribuite_of_server->Value(), "%u", &uport);
            adminport_ = uport;
            LOG_INFO << "Read key: " << "[" << attribuite_of_server->Name() << "]" 
                << " value: " << "[" << adminport_ << "]";
        } else if (strcmp(attribuite_of_server->Name(), "worker_threadnum") == 0) {
            sscanf(attribuite_of_server->Value(), "%u", &worker_threadnum_);
            LOG_INFO << "Read key: " << "[" << attribuite_of_server->Name() << "]" 
                << " value: " << "[" << worker_threadnum_ << "]";
        } else if (strcmp(attribuite_of_server->Name(), "sleep_second") == 0) {
            uint32_t second;
            sscanf(attribuite_of_server->Value(), "%u", &second);
            sleep_second_ = second;
            LOG_INFO << "Read key: " << "[" << attribuite_of_server->Name() << "]" 
                << " value: " << "[" << sleep_second_ << "]";
        } else if (strcmp(attribuite_of_server->Name(), "wait_second") == 0) {
            uint32_t second;
            sscanf(attribuite_of_server->Value(), "%u", &second);
            wait_second_ = second;
            LOG_INFO << "Read key: " << "[" << attribuite_of_server->Name() << "]" 
                << " value: " << "[" << wait_second_ << "]";
        }

        attribuite_of_server = attribuite_of_server->Next();
    }
}

} // namespace xdb
 
