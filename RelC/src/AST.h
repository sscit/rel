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

// Represents an entry of a type
class RsTypeElement {
public:
    RsRdIdentifier name;
    TokenType token_type;
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

// represents one entry of the type instance
class RdTypeInstanceElement {
public:
    RsTypeElement name;
    RdString string_value;
    RdInteger integer_value;
    RsRdIdentifier link;
    RsRdIdentifier enum_value;
};

class RdTypeInstance {
public:
    RsType type;
    std::vector<RdTypeInstanceElement> type_elements_data;
};

#endif
