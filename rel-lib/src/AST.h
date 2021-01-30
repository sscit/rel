/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef AST_H_
#define AST_H_

#include <vector>
#include <string>
#include "Token.h"

/* Data Structures of AST, used for .rs and .rd syntax */
class RsRdIdentifier {
public:
    // returns the name of the identifier
    std::string Get() const { return name; }
    std::string name;
};

// AST elements used in .rs files
class RsEnum {
public:
    // returns the name of the enum type
    std::string Get() const { return name.Get(); }
    RsRdIdentifier name;
    std::vector<RsRdIdentifier> enum_elements;
};

// Represents an entry of a type definition
class RsTypeAttribute {
public:
    // returns the name of the attribute
    std::string Get() const { return name.Get(); }
    RsRdIdentifier name;
    /* Token that defines the type of this RsTypeAttribute,
       e.g. link, or the enum identifier */
    Token token_of_attribute;
    // If attribute is an enum, enum definition is located here
    RsEnum enum_definition;
};

class RsType {
public:
    // returns the name of the type
    std::string Get() const { return name.Get(); }
    RsRdIdentifier name;
    std::vector<RsTypeAttribute> attributes;
};

// AST elements used in .rd files
class RdString {
public:
    // returns the string value
    std::string Get() const { return value; }
    std::string value;
};

class RdInteger {
public:
    RdInteger() : value(0), is_valid(false) { }
    RdInteger(int const v) : value(v), is_valid(true) { }
    /* returns the integer value
     * Req: dsl9
     */
    int Get() const { return value; }
    // returns true, if class contains valid data
    bool IsValid() const { return is_valid;}

private:
    int value;
    // true, if class contains valid data
    bool is_valid;
};

// represents one attribute of the type instance
class RdTypeInstanceAttribute {
public:
    // pointer to the definition of the attribute from type definition
    RsTypeAttribute const * name;
    // attribute's value
    RdString string_value;
    RdInteger integer_value;
    std::vector<RsRdIdentifier> link_value;
    RsRdIdentifier enum_value;
    // copy of the token, that contains the attribute's value
    Token token_of_value;
};

class RdTypeInstance {
public:
    RdTypeInstance() : type(nullptr) { }
    // pointer to the type specification
    RsType const * type;
    std::vector<RdTypeInstanceAttribute> attributes;
};

// contains all type instances of a file
class RdFile {
public:
    RdFile() { }
    std::string filename;
    std::vector<RdTypeInstance> type_instances;
};

#endif
