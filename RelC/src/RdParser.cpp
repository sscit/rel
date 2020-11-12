/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RdParser.h"

RdParser::RdParser(Logger &logger, RsParser const & s) : Parser(logger), specification(s) { }

ParsingStatistic RdParser::GetParsingStatistics() const {
    return statistic;
}

template<class T>
void RdParser::EnsureToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt, T const e) {
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


void RdParser::CheckAllLinks() {
    l.LOG(LogLevel::DEBUG, "Checking that all links defined exist and point to valid ids");

    for(unsigned int i=0; i<database.size(); i++) {
        RdTypeInstance const &instance = database[i];

        for(unsigned int j=0; j<instance.type_elements_data.size(); j++) {
            RsRdIdentifier const &link = instance.type_elements_data[j].link;
            if(link.name.length() > 0) {
                auto const search = unique_ids.find(link.name);
                if(search == unique_ids.end()) {
                    throw RdTypeException(instance);
                }
            }
        }
    }
}

RdString RdParser::ReadString(FileTokenData const& tokens, unsigned int &index) {
    RdString result;
    Token const& begin_string = tokens.token_list[index];
    EnsureToken(tokens, index, TokenType::QUOTATION_MARK, RdStringException(begin_string, "Invalid token, expected \""));
    index++;

    bool suffix_has_been_added=false;
    while(tokens.token_list[index].GetTokenType() != TokenType::QUOTATION_MARK
          && index < tokens.token_list.size()) {
        suffix_has_been_added = false;
        std::string suffix = "";
        if(tokens.token_list[index].GetTokenType() != TokenType::END_OF_LINE) {
            suffix = " ";
            suffix_has_been_added = true;
        }

        result.value.append(tokens.token_list[index].GetTokenValue() + suffix);
        index++;
    }

    if(suffix_has_been_added) {
        result.value.pop_back();
    }

    if(index == tokens.token_list.size())
        throw RdStringException(begin_string, "String termination not found");

    return result;
}

RdInteger RdParser::Integer(FileTokenData const& tokens, unsigned int &index) {
    RdInteger integer_value;

    integer_value.value = std::stoi(tokens.token_list[index].GetTokenValue());

    return integer_value;
}

bool RdParser::EnumValueExists(std::vector<RsRdIdentifier> const &enum_values, RsRdIdentifier &enum_value) const {
    std::vector<RsRdIdentifier>::const_iterator iter = enum_values.begin();
    bool found = false;
    for(;iter != enum_values.end(); ++iter) {
        if(iter->name.compare(enum_value.name) == 0) {
            found = true;
            enum_value = *iter;
            break;
        }
    }

    return found;
}

bool RdParser::HasAttributeValueCorrectType(RsTypeElement const& type_element, TokenType const current_token) {
    return (type_element.token_of_element.GetTokenType() == current_token);
}

RdTypeInstance RdParser::TypeInstance(FileTokenData const& tokens, unsigned int& index) {
    Token const &type_token = tokens.token_list[index];
    RdTypeInstance type_instance;
    RsRdIdentifier type_name = Identifier(tokens, index++);
    RsType type_definition;
    l.LOG(LogLevel::DEBUG, "Parsing Type Instance of type " + type_name.name);

    try {
        type_definition = specification.GetType(type_name);
    }
    catch(TypeNotFoundException &e) {
        throw RdTypeException(type_token, "Type instance " + type_name.name + " not found in specification");
    }
    type_instance.type = type_definition;

    EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RdTypeException(tokens.token_list[index], "Wrong token, expected {"));
    index++;

    unsigned int number_of_elements = 0;
    while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
        l.LOG(LogLevel::DEBUG, "Starting to parse new data attribute");
        EnsureToken(tokens, index, TokenType::IDENTIFIER, RdTypeException(tokens.token_list[index], "Wrong token, expected attribute"));
        RsRdIdentifier attribute = Identifier(tokens, index);
        if( number_of_elements >= type_definition.type_elements.size() ||
            attribute.name.compare(type_definition.type_elements[number_of_elements].name.name) != 0) {
            throw RdTypeException(tokens.token_list[index], "Attribute not defined in type specification");
        }
        l.LOG(LogLevel::DEBUG, "Parsing data attribute named " + attribute.name);

        RdTypeInstanceElement type_instance_element;
        type_instance_element.name = type_definition.type_elements[number_of_elements];
        index++;

        EnsureToken(tokens, index, TokenType::COLON, RdTypeException(tokens.token_list[index], "Wrong token, expected :"));
        index++;
        if(tokens.token_list[index].GetTokenType() == TokenType::STRING_VALUE) {
            if(HasAttributeValueCorrectType(type_instance_element.name, TokenType::STRING)) {
                type_instance_element.string_value.value = tokens.token_list[index].GetTokenValue();
                l.LOG(LogLevel::DEBUG, "attribute value of type string parsed");
            }
            else {
                throw RdTypeException(tokens.token_list[index], "Attribute value has wrong type, expected string value");
            }
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::QUOTATION_MARK) {
            if(HasAttributeValueCorrectType(type_instance_element.name, TokenType::STRING)) {
                type_instance_element.string_value = ReadString(tokens, index);
                l.LOG(LogLevel::DEBUG, "attribute value of type string parsed");
            }
            else {
                throw RdTypeException(tokens.token_list[index], "Attribute value has wrong type, expected string value");
            }
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::INTEGER_VALUE) {
            if(HasAttributeValueCorrectType(type_instance_element.name, TokenType::INT)) {
                type_instance_element.integer_value = Integer(tokens, index);
                l.LOG(LogLevel::DEBUG, "attribute value of type integer parsed");
            }
            else {
                throw RdTypeException(tokens.token_list[index], "Attribute value has wrong type, expected integer value");
            }
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER) {
            l.LOG(LogLevel::DEBUG, "attribute value of type identifier identified");
            if(type_instance_element.name.token_of_element.GetTokenType() == TokenType::ENUM) {
                // Check that the enum value used exists
                type_instance_element.enum_value.name = tokens.token_list[index].GetTokenValue();
                if(!EnumValueExists(type_instance_element.name.enum_definition.enum_elements, type_instance_element.enum_value)) {
                    throw RdTypeException(tokens.token_list[index], "Enum value " + type_instance_element.enum_value.name + " not found in specification");
                }
                l.LOG(LogLevel::DEBUG, "attribute has enum value " + type_instance_element.enum_value.name + ", enum value exists");
            }
            else if(type_instance_element.name.token_of_element.GetTokenType() == TokenType::LINK) {
                type_instance_element.link = Identifier(tokens, index);
                l.LOG(LogLevel::DEBUG, "attribute of type link parsed");
            }
            else if(type_instance_element.name.token_of_element.GetTokenType() == TokenType::ID) {
                type_instance_element.string_value.value = tokens.token_list[index].GetTokenValue();
                l.LOG(LogLevel::DEBUG, "attribute of type id parsed");
            }
            else
                throw RdTypeException(tokens.token_list[index], "Unexpected token");
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::ID) {
            l.LOG(LogLevel::DEBUG, "attribute value of type id identified");
            if(HasAttributeValueCorrectType(type_instance_element.name, TokenType::ID)) {
                type_instance_element.string_value.value = tokens.token_list[index].GetTokenValue();
                l.LOG(LogLevel::DEBUG, "attribute value of type id parsed");
            }
            else {
                throw RdTypeException(tokens.token_list[index], "Attribute value has wrong type, expected id");
            }
        }
        else {
            throw RdTypeException(tokens.token_list[index], "Unexpected token");
        }

        // check for unique id, if this attribute defined an id
        if(type_definition.type_elements[number_of_elements].token_of_element.GetTokenType() == TokenType::ID) {
            if(unique_ids.find(type_instance_element.string_value.value) != unique_ids.end()) {
                throw RdTypeException(tokens.token_list[index], "ID already used");
            }
            else {
                unique_ids[type_instance_element.string_value.value] = type_instance_element.string_value;
            }
        }

        type_instance.type_elements_data.push_back(type_instance_element);
        index++;
        EnsureToken(tokens, index, TokenType::COMMA, RdTypeException(tokens.token_list[index], "Wrong token, expected ,"));
        index++;
        number_of_elements++;
    }

    // Check error cases
    if(type_instance.type_elements_data.size() == 0)
        throw RdTypeException(tokens.token_list[index], "No attributes defined in type instance");

    if(type_instance.type_elements_data.size() < type_instance.type.type_elements.size())
        throw RdTypeException(tokens.token_list[index], "Type instance does not match to type definition, missing attributes.");

    l.LOG(LogLevel::DEBUG, "Dataset " + type_instance.type.name.name + " (" + std::to_string(type_instance.type_elements_data.size()) + " elements) has been created.");

    return type_instance;
}

void RdParser::ParseTokens(FileTokenData const& tokens) {
    statistic.number_of_files++;
    for(unsigned int index=0; index<tokens.token_list.size(); ++index) {
        Token const& current_token = tokens.token_list[index];

        if(current_token.GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::IDENTIFIER) {
            statistic.number_of_type_instances++;
            RdTypeInstance t = TypeInstance(tokens, index);
            database.push_back(t);
        }
        else if(current_token.GetTokenType() == TokenType::END_OF_LINE) { }
        else {
            throw WrongTokenException(current_token, "Unexpected token");
        }
    }
}

RdParser::~RdParser() { }
