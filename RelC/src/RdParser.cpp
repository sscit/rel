/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RdParser.h"

RdParser::RdParser(Logger const &logger, RsParser const & s) : Parser(logger), specification(s) { }

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

RdTypeInstance RdParser::TypeInstance(FileTokenData const& tokens, unsigned int& index) {
    Token const &type_token = tokens.token_list[index];
    RdTypeInstance dataset;
    RsRdIdentifier type_name = Identifier(tokens, index++);
    RsType type_definition;
    l.LOG(LogLevel::DEBUG, "Parsing Type Instance of type " + type_name.name);

    try {
        type_definition = specification.GetType(type_name);
    }
    catch(TypeNotFoundException &e) {
        throw RdTypeException(type_token, "Type instance " + type_name.name + " not found in specification");
    }
    dataset.type = type_definition;

    EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RdTypeException(tokens.token_list[index], "Wrong token, expected {"));
    index++;

    int number_of_elements = 0;
    while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
        l.LOG(LogLevel::DEBUG, "Starting to parse new data attribute");
        EnsureToken(tokens, index, TokenType::IDENTIFIER, RdTypeException(tokens.token_list[index], "Wrong token, expected attribute"));
        RsRdIdentifier attribute = Identifier(tokens, index);
        if(attribute.name.compare(dataset.type.type_elements[number_of_elements].name.name) != 0) {
            throw RdTypeException(tokens.token_list[index], "Attribute not defined in type specification");
        }
        l.LOG(LogLevel::DEBUG, "Parsing data attribute named " + attribute.name);

        RdTypeInstanceElement type_element;
        type_element.name = dataset.type.type_elements[number_of_elements];
        index++;

        EnsureToken(tokens, index, TokenType::COLON, RdTypeException(tokens.token_list[index], "Wrong token, expected :"));
        index++;
        if(tokens.token_list[index].GetTokenType() == TokenType::STRING_VALUE) {
            type_element.string_value.value = tokens.token_list[index].GetTokenValue();
            l.LOG(LogLevel::DEBUG, "attribute value of type string parsed");
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::QUOTATION_MARK) {
            type_element.string_value = ReadString(tokens, index);
            l.LOG(LogLevel::DEBUG, "attribute value of type string parsed");
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::INTEGER_VALUE) {
            type_element.integer_value = Integer(tokens, index);
            l.LOG(LogLevel::DEBUG, "attribute value of type integer parsed");
        }
        else if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER) {
            l.LOG(LogLevel::DEBUG, "attribute value of type identifier identified");
            if(type_element.name.token_type == TokenType::ENUM) {
                // Check that the enum value used exists
                type_element.enum_value.name = tokens.token_list[index].GetTokenValue();
                if(!EnumValueExists(type_element.name.enum_definition.enum_elements, type_element.enum_value)) {
                    throw RdTypeException(tokens.token_list[index], "Enum value " + type_element.enum_value.name + " not found in specification");
                }
                l.LOG(LogLevel::DEBUG, "attribute has enum value " + type_element.enum_value.name + ", enum value exists");
            }
            else if(type_element.name.token_type == TokenType::LINK) {
                type_element.link = Identifier(tokens, index);
                l.LOG(LogLevel::DEBUG, "attribute of type link parsed");
            }
            else
                throw RdTypeException(tokens.token_list[index], "Unexpected token");
        }
        else
            throw RdTypeException(tokens.token_list[index], "Unexpected token");

        // check for unique id, if this attribute defined an id
        if(dataset.type.type_elements[number_of_elements].token_type == TokenType::ID) {
            if(unique_ids.find(type_element.string_value.value) != unique_ids.end()) {
                throw RdTypeException(tokens.token_list[index], "ID already used");
            }
            else {
                unique_ids[type_element.string_value.value] = type_element.string_value;
            }
        }

        dataset.type_elements_data.push_back(type_element);
        index++;
        EnsureToken(tokens, index, TokenType::COMMA, RdTypeException(tokens.token_list[index], "Wrong token, expected ,"));
        index++;
        number_of_elements++;
    }

    // Check error cases
    if(dataset.type_elements_data.size() == 0)
        throw RdTypeException(tokens.token_list[index], "No attributes defined in type instance");

    l.LOG(LogLevel::DEBUG, "Dataset " + dataset.type.name.name + " (" + std::to_string(dataset.type_elements_data.size()) + " elements) has been created.");

    return dataset;
}

void RdParser::ParseTokens(FileTokenData const& tokens) {
    statistic.number_of_files++;
    for(unsigned int index=0; index<tokens.token_list.size(); ++index) {
        if(tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::IDENTIFIER) {
            statistic.number_of_type_instances++;
            RdTypeInstance t = TypeInstance(tokens, index);
            database.push_back(t);
        }
    }
}

RdParser::~RdParser() { }
