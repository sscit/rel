/*
 * RsParser.cpp
 *
 *  Created on: 12.10.2020
 *      Author: Stefan_2
 */

#include "RsParser.h"

RsParser::RsParser(Logger const &logger) : Parser(logger) {
    // TODO Auto-generated constructor stub

}

RsType RsParser::GetType(RsRdIdentifier &type_ident) const
{
    auto search_result = all_types.find(type_ident.name);
    if (search_result != all_types.end())
    {
        return search_result->second;
    }
    else
    {
        throw TypeNotFoundException();
    }
}

void RsParser::CheckAllEnumTypes()
{
    std::map<std::string, RsType>::iterator it;
    l.LOG(LogLevel::DEBUG, "Checking that all enum types used are defined");
    for(it = all_types.begin(); it != all_types.end(); it++)
    {
        RsType &t = it->second;
        for(unsigned int j=0; j<t.type_elements.size(); j++)
        {
            RsTypeElement &te = t.type_elements[j];
            if(te.token_type == TokenType::ENUM)
            {
                auto search_result = all_enums.find(te.enum_definition.name.name);
                if(search_result != all_enums.end())
                {
                    te.enum_definition = search_result->second;
                }
                else
                {
                    l.LOG(LogLevel::ERROR, "Enum " + te.enum_definition.name.name + " was used but not defined");
                    throw RsTypeException(t);
                }
            }
        }
    }
}

template<class T>
void RsParser::EnsureToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt, T const e)
{
    while(tokens.token_list[index].GetTokenType() == TokenType::END_OF_LINE ||
          tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT ||
          tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START)
    {
        if(tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT)
        {
            LineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START)
        {
            MultiLineComment(tokens, index);
        }
        index++;
    }

    if(tokens.token_list[index].GetTokenType() != tt)
        throw e;
}

void RsParser::ParseTokens(FileTokenData const& tokens)
{
    for(unsigned int index=0; index<tokens.token_list.size(); ++index)
    {
        if(tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT)
        {
            LineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START)
        {
            MultiLineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::ENUM)
        {
            RsEnum e = EnumDefinition(tokens, index);
            all_enums.insert({e.name.name, e});
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::TYPE)
        {
            RsType t = TypeDefinition(tokens, index);
            all_types.insert({t.name.name, t});
        }
    }
}

RsEnum RsParser::EnumDefinition(FileTokenData const& tokens, unsigned int &index)
{
    l.LOG(LogLevel::DEBUG, "Parsing Enumeration");
    Token const &enum_token = tokens.token_list[index];
    RsEnum enumeration;

    index++;
    if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER)
    {
        enumeration.name = Identifier(tokens, index++);
        EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RsEnumException(tokens.token_list[index], "Wrong token, expected {"));
        index++;

        while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) )
        {
            EnsureToken(tokens, index, TokenType::IDENTIFIER, RsEnumException(tokens.token_list[index], "Wrong token, expected identifier"));
            // todo check that enum string is unique in this enum
            enumeration.enum_elements.push_back(Identifier(tokens, index++));
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
    else
    {
        throw RsEnumException(enum_token, "Wrong token, expected identifier");
    }

    l.LOG(LogLevel::DEBUG, "Enumeration " + enumeration.name.name + " (" + std::to_string(enumeration.enum_elements.size()) + " elements) has been created.");

    return enumeration;
}

RsType RsParser::TypeDefinition(FileTokenData const& tokens, unsigned int &index)
{
    l.LOG(LogLevel::DEBUG, "Parsing Type");
    Token const &type_token = tokens.token_list[index];
    RsType type;

    index++;
    if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER)
    {
        type.name = Identifier(tokens, index++);
        EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RsTypeException(tokens.token_list[index], "Wrong token, expected {"));
        index++;

        while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) )
        {
            EnsureToken(tokens, index, TokenType::IDENTIFIER, RsTypeException(tokens.token_list[index], "Wrong token, expected identifier"));

            RsTypeElement type_element;
            type_element.name = Identifier(tokens, index++);
            EnsureToken(tokens, index, TokenType::COLON, RsTypeException(tokens.token_list[index], "Wrong token, expected :"));
            index++;
            if(tokens.token_list[index].GetTokenType() == TokenType::ID ||
                 tokens.token_list[index].GetTokenType() == TokenType::STRING ||
               tokens.token_list[index].GetTokenType() == TokenType::LINK ||
               tokens.token_list[index].GetTokenType() == TokenType::INT)
            {
                type_element.token_type = tokens.token_list[index++].GetTokenType();
            }
            else if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER)
            {
                /* An identifier at this place can only identify an enum.
                 * Keep its name and check later, that it exists
                 */
                RsRdIdentifier enum_name = Identifier(tokens, index++);
                type_element.token_type = TokenType::ENUM;
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
    else
    {
        throw RsTypeException(type_token, "Wrong token, expected identifier");
    }

    l.LOG(LogLevel::DEBUG, "Type " + type.name.name + " (" + std::to_string(type.type_elements.size()) + " elements) has been created.");

    return type;
}




RsParser::~RsParser() {
    // TODO Auto-generated destructor stub
}
