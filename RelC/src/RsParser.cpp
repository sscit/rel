/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RsParser.h"

TypeOrigin::TypeOrigin() { }

TypeOrigin::TypeOrigin(std::string const& u) : uri(u) { }


RsParser::RsParser(Logger &logger) : Parser(logger) { }

RsType RsParser::GetType(RsRdIdentifier &type_ident) const {
    auto search_result = all_types.find(type_ident.name);
    if (search_result != all_types.end()) {
        return search_result->second;
    }
    else {
        throw TypeNotFoundException();
    }
}

void RsParser::CheckAllEnumTypes() {
    std::map<std::string, RsType>::iterator it;
    l.LOG(LogLevel::DEBUG, "Checking that all enum types used are defined");
    for(it = all_types.begin(); it != all_types.end(); it++) {
        RsType &t = it->second;
        for(unsigned int j=0; j<t.type_elements.size(); j++) {
            RsTypeElement &te = t.type_elements[j];
            if(te.token_of_element.GetTokenType() == TokenType::ENUM) {
                auto search_result = all_enums.find(te.enum_definition.name.name);
                if(search_result != all_enums.end()) {
                    te.enum_definition = search_result->second;
                }
                else {
                    throw EnumUsedButNotDefinedException(te.token_of_element, "Enum " + te.enum_definition.name.name + " was used but not defined");
                }
            }
        }
    }
}

template<class T>
void RsParser::EnsureToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt, T const e) {
    while(tokens.token_list[index].GetTokenType() == TokenType::END_OF_LINE ||
          tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT ||
          tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) {
        if(tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        index++;
    }

    if(tokens.token_list[index].GetTokenType() != tt)
        throw e;
}

void RsParser::CleanupEnumDatabase(std::string const& uri) {
    // Clear old enum definitions originating from this file out of the data base
    for( auto const & to : enum_origin) {
        if(to.uri.compare(uri) == 0)
            all_enums.erase(to.type_name);
    }
    enum_origin.remove_if(
        [&](TypeOrigin &to){ 
            return (to.uri.compare(uri) == 0); 
        }
    );
}

void RsParser::CleanupTypeDatabase(std::string const& uri) {
    // Clear old type definitions originating from this file out of the data base
    for( auto const & to : type_origin) {
        if(to.uri.compare(uri) == 0)
            all_types.erase(to.type_name);
    }
    type_origin.remove_if(
        [&](TypeOrigin &to){
            return (to.uri.compare(uri) == 0);
        }
    );
}

void RsParser::AddEnumToDatabase(RsEnum const& enum_type, std::string const& uri) {
    all_enums.insert({enum_type.name.name, enum_type});

    TypeOrigin new_enum(uri);
    new_enum.type_name = enum_type.name.name;
    enum_origin.push_back(new_enum);
}

void RsParser::AddTypeToDatabase(RsType const& rs_type, std::string const& uri) {
    all_types.insert({rs_type.name.name, rs_type});

    TypeOrigin new_type(uri);
    new_type.type_name = rs_type.name.name;
    type_origin.push_back(new_type);
}

void RsParser::ParseTokens(FileTokenData const& tokens) {
    CleanupEnumDatabase(tokens.filepath);
    CleanupTypeDatabase(tokens.filepath);

    for(unsigned int index=0; index<tokens.token_list.size(); ++index) {
        Token const& current_token = tokens.token_list[index];

        if(current_token.GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::ENUM) {
            RsEnum e = EnumDefinition(tokens, index);
            AddEnumToDatabase(e, tokens.filepath);
        }
        else if(current_token.GetTokenType() == TokenType::TYPE) {
            RsType t = TypeDefinition(tokens, index);
            AddTypeToDatabase(t, tokens.filepath);
        }
        else if(current_token.GetTokenType() == TokenType::END_OF_LINE) { }
        else {
            throw WrongTokenException(current_token, "Unexpected token");
        }
    }
}

bool RsParser::IsEnumValueUnique(RsEnum const& enum_type, RsRdIdentifier const& enum_value) {
    bool ret = true;

    std::vector<RsRdIdentifier>::const_iterator iter = enum_type.enum_elements.begin();
    for(; iter<enum_type.enum_elements.end(); iter++) {
        if(iter->name.compare(enum_value.name) == 0) {
            ret = false;
            break;
        }
    }

    return ret;
}

RsEnum RsParser::EnumDefinition(FileTokenData const& tokens, unsigned int &index) {
    l.LOG(LogLevel::DEBUG, "Parsing Enumeration");
    Token const &enum_token = tokens.token_list[index];
    RsEnum enumeration;

    index++;
    if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER) {
        enumeration.name = Identifier(tokens, index++);
        EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RsEnumException(tokens.token_list[index], "Wrong token, expected {"));
        index++;

        while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
            EnsureToken(tokens, index, TokenType::IDENTIFIER, RsEnumException(tokens.token_list[index], "Wrong token, expected identifier"));
            RsRdIdentifier enum_value = Identifier(tokens, index);
            if(IsEnumValueUnique(enumeration, enum_value))
                enumeration.enum_elements.push_back(enum_value);
            else
                throw RsEnumException(tokens.token_list[index], "Enum value " + enum_value.name + " already defined");
            index++;

            EnsureToken(tokens, index, TokenType::COMMA, RsEnumException(tokens.token_list[index], "Wrong token, expected ,"));
            index++;
        }

        // Check error case that there are no enum elements
        if(enumeration.enum_elements.size() == 0)
            throw RsEnumException(enum_token, "No enum attributes defined");
        // Check that name hasn't been used before
        if(all_enums.find(enumeration.name.name) != all_enums.end())
            throw RsEnumException(enum_token, "Enum with this name already defined");
    }
    else {
        throw RsEnumException(enum_token, "Wrong token, expected identifier");
    }

    l.LOG(LogLevel::DEBUG, "Enumeration " + enumeration.name.name + " (" + std::to_string(enumeration.enum_elements.size()) + " elements) has been created.");

    return enumeration;
}

RsType RsParser::TypeDefinition(FileTokenData const& tokens, unsigned int &index) {
    l.LOG(LogLevel::DEBUG, "Parsing Type");
    Token const &type_token = tokens.token_list[index];
    RsType type;

    index++;
    if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER) {
        type.name = Identifier(tokens, index++);
        EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RsTypeException(tokens.token_list[index], "Wrong token, expected {"));
        index++;

        while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
            EnsureToken(tokens, index, TokenType::IDENTIFIER, RsTypeException(tokens.token_list[index], "Wrong token, expected identifier"));

            RsTypeElement type_element;
            type_element.name = Identifier(tokens, index++);
            EnsureToken(tokens, index, TokenType::COLON, RsTypeException(tokens.token_list[index], "Wrong token, expected :"));
            index++;
            if(tokens.token_list[index].GetTokenType() == TokenType::ID ||
                 tokens.token_list[index].GetTokenType() == TokenType::STRING ||
               tokens.token_list[index].GetTokenType() == TokenType::LINK ||
               tokens.token_list[index].GetTokenType() == TokenType::INT) {
                type_element.token_of_element = tokens.token_list[index++];
            }
            else if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER) {
                /* An identifier at this place can only identify an enum.
                 * Keep its details and check later, that it exists
                 */
                Token const &tmp = tokens.token_list[index];
                Token enum_token(tmp.GetTokenValue(), TokenType::ENUM, tmp.GetFilename(), tmp.GetLineNumberOfToken(), tmp.GetPositionInLineOfToken());
                type_element.token_of_element = enum_token;
                RsRdIdentifier enum_name = Identifier(tokens, index++);
                type_element.enum_definition.name = enum_name;
            }
            else
                throw RsTypeException(tokens.token_list[index], "Wrong type attribute definition");

            type.type_elements.push_back(type_element);

            EnsureToken(tokens, index, TokenType::COMMA, RsTypeException(tokens.token_list[index], "Wrong token, expected ,"));
            index++;
        }

        // Check error cases
        if(type.type_elements.size() == 0)
            throw RsTypeException(type_token, "Type definition empty");
        // Check that name hasn't been used before
        if(all_types.find(type.name.name) != all_types.end())
            throw RsTypeException(type_token, "Type with this name already defined");
    }
    else {
        throw RsTypeException(type_token, "Wrong token, expected identifier");
    }

    l.LOG(LogLevel::DEBUG, "Type " + type.name.name + " (" + std::to_string(type.type_elements.size()) + " elements) has been created.");

    return type;
}

RsParser::~RsParser() { }
