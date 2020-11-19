/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef URI_H_
#define URI_H_

#include <string>

// Class is used to handle URIs provided by the client
class Uri {
public:
    Uri();
    Uri(std::string const&);
    ~Uri();

    // Factory to create an Uri object with scheme file from path provided
    static Uri CreateFileUriFromPath(std::string const&);

    std::string GetScheme() const;
    std::string GetPath() const;
    std::string GetUri() const;
    bool IsRequirementsSpecification() const;
    bool IsRequirementsData() const;

private:
    std::string uri;
    std::string scheme;
    std::string path;


};

#endif 
