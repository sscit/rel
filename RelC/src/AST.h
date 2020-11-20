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
    std::string name;
};

// AST elements used in .rs files
class RsEnum {
public:
    RsRdIdentifier name;
    std::vector<RsRdIdentifier> enum_elements;
};

// Represents an entry of a type definition
class RsTypeElement {
public:
    RsRdIdentifier name;
    /* Token that defines the type of this RsTypeElement,
       e.g. link, or the enum identifier */
    Token token_of_element;
    RsEnum enum_definition;
};

class RsType {
public:
    RsRdIdentifier name;
    std::vector<RsTypeElement> type_elements;
};

// AST elements used in .rd files
class RdString {
public:
    std::string value;
};

class RdInteger {
public:
    int value;
};

// represents one attribute of the type instance
class RdTypeInstanceElement {
public:
    RsTypeElement name;
    RdString string_value;
    RdInteger integer_value;
    RsRdIdentifier link;
    RsRdIdentifier enum_value;
    // Represents the token of the attribute's value 
    Token token_of_value;
};

class RdTypeInstance {
public:
    RsType type;
    // path to the file, where this type instance originates from
    std::string file_origin;
    std::vector<RdTypeInstanceElement> type_elements_data;
};

#endif
