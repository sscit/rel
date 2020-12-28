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
    l.LOG(LogLevel::DBUG, "Checking that all enum types used are defined");
    for(it = all_types.begin(); it != all_types.end(); it++) {
        RsType &t = it->second;
        for(unsigned int j=0; j<t.attributes.size(); j++) {
            RsTypeAttribute &te = t.attributes[j];
            if(te.token_of_attribute.GetTokenType() == TokenType::ENUM) {
                auto search_result = all_enums.find(te.enum_definition.Get());
                if(search_result != all_enums.end()) {
                    te.enum_definition = search_result->second;
                }
                else {
                    throw EnumUsedButNotDefinedException(te.token_of_attribute, "Enum " + te.enum_definition.Get() + " was used but not defined");
                }
            }
        }
    }
}

template<class T>
void RsParser::EnsureToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt, T const e) {
    while( index < tokens.token_list.size() && (
          tokens.token_list[index].GetTokenType() == TokenType::END_OF_LINE ||
          tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT ||
          tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) ) {
        if(tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        index++;
    }
    if( (index >= tokens.token_list.size()) || (tokens.token_list[index].GetTokenType() != tt) )
        throw e;
}

template<class T>
void RsParser::CleanupDatabase(std::string const& uri, std::map<std::string, T>& all,
                     std::list<TypeOrigin>& origin) {
    // Clear old definitions originating from this file out of the data base
    for( auto const & to : origin) {
        if(to.uri.compare(uri) == 0)
            all.erase(to.type_name);
    }
    origin.remove_if(
        [&](TypeOrigin &to){ 
            return (to.uri.compare(uri) == 0); 
        }
    );
}

template<class T>
void RsParser::AddToDatabase(T const& type, std::string const& uri,
                             std::map<std::string, T>& all,
                             std::list<TypeOrigin>& origin) {
    all.insert({type.name.name, type});

    TypeOrigin new_type(uri);
    new_type.type_name = type.name.name;
    origin.push_back(new_type);
}

void RsParser::ParseTokens(FileTokenData const& tokens) {
    CleanupDatabase(tokens.filepath, all_enums, enum_origin);
    CleanupDatabase(tokens.filepath, all_types, type_origin);

    for(unsigned int index=0; index<tokens.token_list.size(); ++index) {
        Token const& current_token = tokens.token_list[index];

        if(current_token.GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::ENUM) {
            try {
                RsEnum e = EnumDefinition(tokens, index);
                AddToDatabase(e, tokens.filepath, all_enums, enum_origin);
            }
            catch(std::out_of_range &e) {
                l.LOG(LogLevel::DBUG, "Enumeration not ready to be parsed completely, hit Array out of bounds.");
                throw RsEnumException(current_token, "Enum definition incomplete");
            }
        }
        else if(current_token.GetTokenType() == TokenType::TYPE) {
            try {
                RsType t = TypeDefinition(tokens, index);
                AddToDatabase(t, tokens.filepath, all_types, type_origin);
            }
            catch(std::out_of_range &e) {
                l.LOG(LogLevel::DBUG, "Type definition not ready to be parsed completely, hit Array out of bounds.");
                throw RsTypeException(current_token, "Type definition incomplete");
            }
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
    l.LOG(LogLevel::DBUG, "Parsing Enumeration");
    Token const &enum_token = tokens.token_list.at(index);
    RsEnum enumeration;

    index++;
    if(tokens.token_list.at(index).GetTokenType() == TokenType::IDENTIFIER) {
        enumeration.name = Identifier(tokens, index++);
        EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RsEnumException(tokens.token_list.at(index), "Wrong token, expected {"));
        index++;

        while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
            EnsureToken(tokens, index, TokenType::IDENTIFIER, RsEnumException(tokens.token_list.at(index), "Wrong token, expected identifier"));
            RsRdIdentifier enum_value = Identifier(tokens, index);
            if(IsEnumValueUnique(enumeration, enum_value))
                enumeration.enum_elements.push_back(enum_value);
            else
                throw RsEnumException(tokens.token_list.at(index), "Enum value " + enum_value.name + " already defined");
            index++;

            EnsureToken(tokens, index, TokenType::COMMA, RsEnumException(tokens.token_list.at(index), "Wrong token, expected ,"));
            index++;
        }

        // Check error case that there are no enum elements
        if(enumeration.enum_elements.size() == 0)
            throw RsEnumException(enum_token, "No enum attributes defined");
        // Check that name hasn't been used before
        if(all_enums.find(enumeration.Get()) != all_enums.end())
            throw RsEnumException(enum_token, "Enum with this name already defined");
    }
    else {
        throw RsEnumException(enum_token, "Wrong token, expected identifier");
    }

    l.LOG(LogLevel::DBUG, "Enumeration " + enumeration.Get() + " (" + std::to_string(enumeration.enum_elements.size()) + " elements) has been created.");

    return enumeration;
}

RsType RsParser::TypeDefinition(FileTokenData const& tokens, unsigned int &index) {
    l.LOG(LogLevel::DBUG, "Parsing Type");
    Token const &type_token = tokens.token_list.at(index);
    RsType type;

    index++;
    if(tokens.token_list.at(index).GetTokenType() == TokenType::IDENTIFIER) {
        type.name = Identifier(tokens, index++);
        EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RsTypeException(tokens.token_list.at(index), "Wrong token, expected {"));
        index++;

        while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
            EnsureToken(tokens, index, TokenType::IDENTIFIER, RsTypeException(tokens.token_list.at(index), "Wrong token, expected identifier"));

            RsTypeAttribute type_attribute;
            type_attribute.name = Identifier(tokens, index++);
            EnsureToken(tokens, index, TokenType::COLON, RsTypeException(tokens.token_list.at(index), "Wrong token, expected :"));
            index++;
            if(tokens.token_list.at(index).GetTokenType() == TokenType::ID ||
                 tokens.token_list.at(index).GetTokenType() == TokenType::STRING ||
               tokens.token_list.at(index).GetTokenType() == TokenType::LINK ||
               tokens.token_list.at(index).GetTokenType() == TokenType::INT) {
                type_attribute.token_of_attribute = tokens.token_list.at(index++);
            }
            else if(tokens.token_list.at(index).GetTokenType() == TokenType::IDENTIFIER) {
                /* An identifier at this place can only identify an enum.
                 * Keep its details and check later, that it exists
                 */
                Token const &tmp = tokens.token_list.at(index);
                Token enum_token(tmp.GetTokenValue(), TokenType::ENUM, tmp.GetFilename(), tmp.GetLineNumberOfToken(), tmp.GetPositionInLineOfToken());
                type_attribute.token_of_attribute = enum_token;
                RsRdIdentifier enum_name = Identifier(tokens, index++);
                type_attribute.enum_definition.name = enum_name;
            }
            else
                throw RsTypeException(tokens.token_list.at(index), "Wrong type attribute definition");

            type.attributes.push_back(type_attribute);

            EnsureToken(tokens, index, TokenType::COMMA, RsTypeException(tokens.token_list.at(index), "Wrong token, expected ,"));
            index++;
        }

        // Check error cases
        if(type.attributes.size() == 0)
            throw RsTypeException(type_token, "Type definition empty");
        // Check that name hasn't been used before
        if(all_types.find(type.Get()) != all_types.end())
            throw RsTypeException(type_token, "Type with this name already defined");
    }
    else {
        throw RsTypeException(type_token, "Wrong token, expected identifier");
    }

    l.LOG(LogLevel::DBUG, "Type " + type.Get() + " (" + std::to_string(type.attributes.size()) + " elements) has been created.");

    return type;
}

RsParser::~RsParser() { }
