/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Uri.h"

Uri::Uri() {}

Uri::Uri(std::string const&u) : uri(u) {
    scheme = uri.substr(0, uri.find(":"));

    int begin_auth = uri.find("//")+2;
    std::string tmp = uri.substr(begin_auth);
    int begin_path = tmp.find("/");
    path = tmp.substr(begin_path);
}

Uri::~Uri() {}

Uri Uri::CreateFileUriFromPath(std::string const& path) {
    std::string uri = "file://" + path;

    Uri ret(uri);

    return ret;
}

DataType Uri::GetDataType() const {
    if(IsRequirementsSpecification()) {
        return DataType::RequirementsSpecification;
    }
    else if(IsRequirementsData()) {
        return DataType::RequirementsData;
    }
    else return DataType::Unknown;
}

bool Uri::IsRequirementsSpecification() const {
    if (path.find(".rs") != std::string::npos)
        return true;
    else return false;
}

bool Uri::IsRequirementsData() const {
    if (uri.find(".rd") != std::string::npos)
        return true;
    else return false;
}

std::string Uri::GetUri() const {
    return uri;
}

std::string Uri::GetScheme() const {
    return scheme;
}

std::string Uri::GetPath() const {
    return path;
}

